#include <cstring>

#include "mw/qrd/qrd_Internal.h"

namespace mw {
namespace qrd {
namespace detail {

// (sigma, omega) of RunEuclideanAlgorithm
// 0x00AF514C
Polynomial s_SigmaOmega[2];
// 0x00AF5164
GaloisField s_GaloisField;
// 0x00AF5968
Polynomial s_ZeroPolynomial;
// 0x00AF5974
Polynomial s_OnePolynomial;

namespace {

// 0x0097F8C8
s32 s_IsGaloisFieldReady;

inline s32 EvaluatePolynomial(const s32* coefficients, s32 length, s32 a)
{
    if (a == 0)
    {
        return coefficients[length - 1];
    }
    if (a == 1)
    {
        s32 result = 0;
        for (s32 i = 0; i < length; i++)
        {
            result ^= coefficients[i];
        }
        return result;
    }
    s32 result = coefficients[0];
    for (s32 i = 1; i < length; i++)
    {
        result = GfMultiply(a, result) ^ coefficients[i];
    }
    return result;
}

inline bool BuildMonomial(Polynomial* polynomial, s32 degree, s32 coefficient)
{
    if (degree < 0)
    {
        return false;
    }
    s32 capacity = (degree + 1 + 31) & ~31;
    s32* coefficients = static_cast<s32*>(Allocate(capacity * sizeof(s32)));
    if (coefficients == NULL)
    {
        return false;
    }
    std::memset(coefficients, 0, capacity * sizeof(s32));
    polynomial->length = degree + 1;
    polynomial->capacity = capacity;
    if (polynomial->coefficients != NULL)
    {
        Free(polynomial->coefficients);
    }
    polynomial->coefficients = coefficients;
    coefficients[0] = coefficient;
    return true;
}

// removes the leading zero coefficients (false for an empty polynomial)
inline bool StripPolynomial(Polynomial* polynomial)
{
    s32* coefficients = polynomial->coefficients;
    if (polynomial->length == 0)
    {
        return false;
    }
    if (polynomial->length > 1 && coefficients[0] == 0)
    {
        s32 first = 1;
        while (first < polynomial->length && coefficients[first] == 0)
        {
            first++;
        }
        if (first == polynomial->length)
        {
            polynomial->coefficients = s_ZeroPolynomial.coefficients;
            return true;
        }
        polynomial->length -= first;
        std::memcpy(polynomial->coefficients, coefficients + first, polynomial->length * sizeof(s32));
        return true;
    }
    std::memcpy(polynomial->coefficients, coefficients, polynomial->length * sizeof(s32));
    return true;
}

inline bool MultiplyByScalar(Polynomial* dst, const Polynomial* src, s32 scalar)
{
    Yield();
    if (scalar == 0)
    {
        return InitializePolynomial(dst, 0);
    }
    if (scalar == 1)
    {
        return CopyPolynomial(dst, src);
    }
    s32* product = static_cast<s32*>(Allocate(src->capacity * sizeof(s32)));
    if (product == NULL)
    {
        return false;
    }
    for (s32 i = 0; i < src->length; i++)
    {
        product[i] = GfMultiply(src->coefficients[i], scalar);
    }
    dst->capacity = src->capacity;
    dst->length = src->length;
    if (dst->coefficients != NULL)
    {
        Free(dst->coefficients);
    }
    dst->coefficients = product;
    return true;
}

} // namespace

// 0x0016F410
s32 DecodeReedSolomon(s32* received, s32 length, s32 ecNum)
{
    if (!s_IsGaloisFieldReady)
    {
        // GF(256) with the primitive polynomial x^8 + x^4 + x^3 + x^2 + 1
        s32 x = 1;
        Yield();
        s_GaloisField.primitive = 0x11D;
        for (s32 i = 0; i < 256; i++)
        {
            s_GaloisField.exp[i] = x;
            x <<= 1;
            if (x >= 256)
            {
                x ^= s_GaloisField.primitive;
            }
        }
        for (s32 i = 0; i < 255; i++)
        {
            s_GaloisField.log[s_GaloisField.exp[i]] = i;
        }
        s_IsGaloisFieldReady = 1;
    }

    bool hasNoError = true;
    s32 capacity = (ecNum + 31) & ~31;
    s32* syndromes = static_cast<s32*>(Allocate(capacity * sizeof(s32)));
    if (syndromes == NULL)
    {
        return 0;
    }
    for (s32 i = 0; i < ecNum; i++)
    {
        s32 value = EvaluatePolynomial(received, length, s_GaloisField.exp[i]);
        syndromes[ecNum - 1 - i] = value;
        if (value != 0)
        {
            hasNoError = false;
        }
    }
    if (hasNoError)
    {
        Free(syndromes);
        return 1;
    }

    Polynomial syndrome;
    syndrome.capacity = capacity;
    syndrome.coefficients = syndromes;
    syndrome.length = ecNum;
    if (!StripPolynomial(&syndrome))
    {
        Free(syndromes);
        return 0;
    }
    if (syndrome.coefficients[0] == 0)
    {
        FinalizePolynomial(&syndrome);
        return 1;
    }

    Polynomial monomial;
    monomial.coefficients = NULL;
    monomial.length = 0;
    monomial.capacity = 0;
    if (!BuildMonomial(&monomial, ecNum, 1))
    {
        FinalizePolynomial(&syndrome);
        return 0;
    }

    Polynomial* sigmaOmega = RunEuclideanAlgorithm(&monomial, &syndrome, ecNum);
    if (sigmaOmega == NULL)
    {
        FinalizePolynomial(&syndrome);
        FinalizePolynomial(&monomial);
        return 0;
    }
    Polynomial* sigma = &sigmaOmega[0];
    Polynomial* omega = &sigmaOmega[1];

    // the roots of the error locator (Chien search)
    s32 errorNum = sigma->length - 1;
    s32* locations;
    Yield();
    if (errorNum == 1)
    {
        locations = static_cast<s32*>(Allocate(sizeof(s32)));
        if (locations == NULL)
        {
            goto failLocations;
        }
        locations[0] = sigma->coefficients[sigma->length - 2];
    }
    else
    {
        locations = static_cast<s32*>(Allocate(errorNum * sizeof(s32)));
        if (locations == NULL)
        {
            goto failLocations;
        }
        std::memset(locations, 0, errorNum * sizeof(s32));
        s32 found = 0;
        for (s32 i = 0; i < 256; i++)
        {
            if (found >= errorNum)
            {
                break;
            }
            if (EvaluatePolynomial(sigma->coefficients, sigma->length, i) == 0)
            {
                if (i == 0)
                {
                    break;
                }
                locations[found] = GfInverse(i);
                found++;
            }
        }
        if (found != errorNum)
        {
            Free(locations);
            goto failLocations;
        }
    }
    FinalizePolynomial(sigma);

    {
        // the error values (Forney)
        s32* magnitudes = static_cast<s32*>(Allocate(errorNum * sizeof(s32)));
        Yield();
        if (magnitudes == NULL)
        {
            goto failMagnitudes;
        }
        for (s32 i = 0; i < errorNum; i++)
        {
            s32 inverse = GfInverse(locations[i]);
            if (inverse == -1)
            {
                Free(magnitudes);
                goto failMagnitudes;
            }
            s32 denominator = 1;
            for (s32 j = 0; j < errorNum; j++)
            {
                if (i != j)
                {
                    denominator = GfMultiply(denominator, GfMultiply(locations[j], inverse) ^ 1);
                }
            }
            if (denominator == 0)
            {
                Free(magnitudes);
                goto failMagnitudes;
            }
            magnitudes[i] = GfMultiply(EvaluatePolynomial(omega->coefficients, omega->length, inverse),
                                       GfInverse(denominator));
        }
        FinalizePolynomial(omega);

        for (s32 i = 0; i < errorNum; i++)
        {
            s32 log = GfLog(locations[i]);
            s32 position = length - 1 - log;
            if (log == -1 || position >= length)
            {
                Free(magnitudes);
                Free(locations);
                FinalizePolynomial(&monomial);
                FinalizePolynomial(&syndrome);
                return 0;
            }
            received[position] ^= magnitudes[i];
        }
        Free(magnitudes);
        Free(locations);
        FinalizePolynomial(&monomial);
        FinalizePolynomial(&syndrome);
        return 1;
    }

failMagnitudes:
    Free(locations);
    FinalizePolynomial(omega);
    FinalizePolynomial(&monomial);
    FinalizePolynomial(&syndrome);
    return 0;

failLocations:
    FinalizePolynomial(sigma);
    FinalizePolynomial(omega);
    FinalizePolynomial(&monomial);
    FinalizePolynomial(&syndrome);
    return 0;
}

// 0x001716A4
bool MultiplyPolynomials(Polynomial* dst, const Polynomial* a, const Polynomial* b)
{
    const s32* aCoefficients = a->coefficients;
    if (aCoefficients[0] == 0)
    {
        return InitializePolynomial(dst, 0);
    }
    const s32* bCoefficients = b->coefficients;
    if (bCoefficients[0] == 0)
    {
        return InitializePolynomial(dst, 0);
    }

    s32 aLength = a->length;
    s32 bLength = b->length;
    s32 length = aLength + bLength - 1;
    s32 capacity = (length + 31) & ~31;
    s32* product = static_cast<s32*>(Allocate(capacity * sizeof(s32)));
    if (product == NULL)
    {
        return false;
    }
    std::memset(product, 0, capacity * sizeof(s32));
    for (s32 i = 0; i < aLength; i++)
    {
        s32 coefficient = aCoefficients[i];
        for (s32 j = 0; j < bLength; j++)
        {
            product[i + j] ^= GfMultiply(coefficient, bCoefficients[j]);
        }
    }
    dst->capacity = capacity;
    dst->length = length;
    if (dst->coefficients != NULL)
    {
        Free(dst->coefficients);
    }
    dst->coefficients = product;
    return true;
}

// 0x00178C80
Polynomial* RunEuclideanAlgorithm(Polynomial* a, Polynomial* b, s32 r)
{
    Yield();
    s_SigmaOmega[1].coefficients = NULL;
    s_SigmaOmega[0].coefficients = NULL;
    Polynomial s = {};
    Polynomial sLast = {};
    Polynomial tLast = {};
    Polynomial t = {};

    // a has to have the higher degree
    if (a->length - 1 < b->length - 1)
    {
        Polynomial temp = *a;
        *a = *b;
        *b = temp;
    }
    Polynomial* rLast = a;
    Polynomial* rCurrent = b;

    if (!InitializePolynomial(&s, 0) || !InitializePolynomial(&sLast, 1) || !InitializePolynomial(&tLast, 0) ||
        !InitializePolynomial(&t, 1))
    {
        goto cleanup;
    }

    while (rCurrent->length - 1 >= r / 2)
    {
        if (!EuclideanStep(rCurrent, rLast, &s, &sLast, &t, &tLast))
        {
            goto cleanup;
        }
    }

    {
        s32 sigmaTildeAtZero = t.coefficients[t.length - 1];
        if (sigmaTildeAtZero == 0)
        {
            goto cleanup;
        }
        s32 inverse = GfInverse(sigmaTildeAtZero);
        if (!MultiplyByScalar(&s_SigmaOmega[0], &t, inverse))
        {
            goto cleanup;
        }
        if (!MultiplyByScalar(&s_SigmaOmega[1], rCurrent, inverse))
        {
            FinalizePolynomial(&s_SigmaOmega[0]);
            goto cleanup;
        }
    }
    FinalizePolynomial(&t);
    FinalizePolynomial(&tLast);
    FinalizePolynomial(&sLast);
    FinalizePolynomial(&s);
    return s_SigmaOmega;

cleanup:
    FinalizePolynomial(&t);
    FinalizePolynomial(&tLast);
    FinalizePolynomial(&sLast);
    FinalizePolynomial(&s);
    return NULL;
}

// 0x0017A030
DECOMP_NOINLINE bool InitializePolynomial(Polynomial* polynomial, s32 value)
{
    if (polynomial == NULL)
    {
        return false;
    }
    s32* coefficients = static_cast<s32*>(Allocate(32 * sizeof(s32)));
    if (coefficients == NULL)
    {
        return false;
    }
    coefficients[0] = value;
    polynomial->length = 1;
    polynomial->capacity = 32;
    if (polynomial->coefficients != NULL)
    {
        Free(polynomial->coefficients);
    }
    polynomial->coefficients = coefficients;
    return true;
}

// 0x0017A07C
bool AddPolynomials(Polynomial* dst, const Polynomial* a, const Polynomial* b)
{
    if (a->coefficients[0] == 0)
    {
        return CopyPolynomial(dst, b);
    }
    if (b->coefficients[0] == 0)
    {
        return CopyPolynomial(dst, a);
    }

    const s32* smaller = a->coefficients;
    const s32* larger = b->coefficients;
    s32 length = b->length;
    s32 difference = b->length - a->length;
    if (a->length > b->length)
    {
        smaller = b->coefficients;
        larger = a->coefficients;
        length = a->length;
        difference = a->length - b->length;
    }

    s32 capacity = (length + 31) & ~31;
    s32* sum = static_cast<s32*>(Allocate(capacity * sizeof(s32)));
    if (sum == NULL)
    {
        return false;
    }
    s32 i;
    for (i = 0; i < difference; i++)
    {
        sum[i] = larger[i];
    }
    for (; i < length; i++)
    {
        sum[i] = smaller[i - difference] ^ larger[i];
    }
    dst->length = length;
    dst->capacity = capacity;
    if (dst->coefficients != NULL)
    {
        Free(dst->coefficients);
    }
    dst->coefficients = sum;
    return StripPolynomial(dst);
}

// one step of the extended Euclidean algorithm: divides r(i-2) by r(i-1)
// 0x0017DA6C
bool EuclideanStep(Polynomial* r, Polynomial* rLast, Polynomial* s, Polynomial* sLast, Polynomial* t,
                   Polynomial* tLast)
{
    Polynomial product = {};
    Polynomial tLastLast = {};
    Polynomial sLastLast = {};
    Polynomial rLastLast = {};
    Polynomial q = {};
    bool result = false;

    if (!CopyPolynomial(&rLastLast, rLast) || !CopyPolynomial(&sLastLast, sLast) ||
        !CopyPolynomial(&tLastLast, tLast) || !CopyPolynomial(rLast, r) || !CopyPolynomial(sLast, s) ||
        !CopyPolynomial(tLast, t))
    {
        goto cleanup;
    }
    if (rLast->coefficients[0] == 0)
    {
        goto cleanup;
    }
    if (!CopyPolynomial(r, &rLastLast) || !InitializePolynomial(&q, 0))
    {
        goto cleanup;
    }

    {
        s32 inverse = GfInverse(rLast->coefficients[0]);
        if (inverse == -1)
        {
            goto cleanup;
        }

        while (r->length - 1 >= rLast->length - 1 && r->coefficients[0] != 0)
        {
            s32 degreeDifference = (r->length - 1) - (rLast->length - 1);
            s32 scale = GfMultiply(r->coefficients[0], inverse);
            Polynomial monomial = {};
            Polynomial sum = {};

            // q += scale * x^degreeDifference
            if (!BuildMonomial(&monomial, degreeDifference, scale) || !AddPolynomials(&sum, &q, &monomial) ||
                !CopyPolynomial(&q, &sum))
            {
                FinalizePolynomial(&monomial);
                FinalizePolynomial(&sum);
                goto cleanup;
            }
            FinalizePolynomial(&monomial);

            // r += scale * x^degreeDifference * rLast
            Yield();
            if (scale == 0)
            {
                if (!InitializePolynomial(&monomial, 0))
                {
                    FinalizePolynomial(&monomial);
                    FinalizePolynomial(&sum);
                    goto cleanup;
                }
            }
            else
            {
                s32 length = rLast->length + degreeDifference;
                s32 capacity = (length + 31) & ~31;
                s32* coefficients = static_cast<s32*>(Allocate(capacity * sizeof(s32)));
                if (coefficients == NULL)
                {
                    FinalizePolynomial(&monomial);
                    FinalizePolynomial(&sum);
                    goto cleanup;
                }
                std::memset(coefficients, 0, capacity * sizeof(s32));
                for (s32 i = 0; i < rLast->length; i++)
                {
                    coefficients[i] = GfMultiply(rLast->coefficients[i], scale);
                }
                monomial.capacity = capacity;
                monomial.length = length;
                if (monomial.coefficients != NULL)
                {
                    Free(monomial.coefficients);
                }
                monomial.coefficients = coefficients;
            }
            if (!AddPolynomials(&sum, r, &monomial) || !CopyPolynomial(r, &sum))
            {
                FinalizePolynomial(&monomial);
                FinalizePolynomial(&sum);
                goto cleanup;
            }
            FinalizePolynomial(&monomial);
            FinalizePolynomial(&sum);
        }
    }

    // s = q * sLast + sLastLast, t = q * tLast + tLastLast
    if (MultiplyPolynomials(&product, &q, sLast) && AddPolynomials(s, &product, &sLastLast) &&
        MultiplyPolynomials(&product, &q, tLast) && AddPolynomials(t, &product, &tLastLast))
    {
        result = true;
    }

cleanup:
    FinalizePolynomial(&product);
    FinalizePolynomial(&tLastLast);
    FinalizePolynomial(&sLastLast);
    FinalizePolynomial(&rLastLast);
    FinalizePolynomial(&q);
    return result;
}

// 0x0017F1C4
bool CopyPolynomial(Polynomial* dst, const Polynomial* src)
{
    if (src == NULL || dst == NULL || src->length <= 0)
    {
        return false;
    }
    s32 length = src->length;
    s32 capacity = (length + 31) & ~31;
    s32* coefficients = static_cast<s32*>(Allocate(capacity * sizeof(s32)));
    if (coefficients == NULL)
    {
        return false;
    }
    std::memset(coefficients, 0, capacity * sizeof(s32));
    dst->length = length;
    dst->capacity = capacity;
    if (dst->coefficients != NULL)
    {
        Free(dst->coefficients);
    }
    dst->coefficients = coefficients;
    std::memcpy(coefficients, src->coefficients, src->length * sizeof(s32));
    return true;
}

} // namespace detail
} // namespace qrd
} // namespace mw
