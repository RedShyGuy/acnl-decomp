#include "asr/asr_Search.h"
#include "asr/asr_Frontend.h"
#include <cstring>

// The distances to the codebook and the Viterbi search over the network.
namespace asr {
namespace {
// the score distance of the beam
const s32 BEAM_WIDTH = 2400;
const u16 WORD_NONE = 0xFFFF;
const u32 NODE_NEXT_NONE = 0x7FFF;

DECOMP_ALWAYS_INLINE s32 CalcStateScore(Model* model, u32 state)
{
    const u16* range = &model->stateRanges[state * 2];
    s32 count = range[0];
    const u16* senones = model->senones + range[1];
    const s16* weights = model->weights + range[1];
    s32 best = SCORE_NONE;
    for (s32 i = count; i > 0; i--) {
        u32 senone = *senones++;
        s32 score = model->senoneScores[senone];
        if (score == 0) {
            u32 c0;
            u32 c1;
            u32 c2;
            if (model->codewordNum == 256) {
                const u8* codewords = static_cast<const u8*>(model->senoneCodewords) + senone * 3;
                c0 = codewords[0];
                c1 = codewords[1];
                c2 = codewords[2];
            } else {
                const u16* codewords = static_cast<const u16*>(model->senoneCodewords) + senone * 3;
                c0 = codewords[0];
                c1 = codewords[1];
                c2 = codewords[2];
            }
            s32 sum = model->distances[c0 * 3] + model->distances[c1 * 3 + 1] + model->distances[c2 * 3 + 2];
            score = -(sum >> 1);
            model->senoneScores[senone] = score;
        }
        score += *weights++;
        if (best <= score) {
            best = score;
        }
    }
    return best;
}
} // namespace

// 0x0014875C
void ModelCalcDistances(Model* model, const s16* frame, s32 from, s32 to)
{
    s32 streamNum = model->streamNum;
    s32 num = model->codewordNum < to ? model->codewordNum : to;
    const s16* means = model->means + from * VECTOR_NUM;
    const u16* precisions = model->precisions + from * VECTOR_NUM;
    s32* out = model->distances + from * streamNum;
    for (s32 i = from; i < num; i++) {
        const s16* input = frame;
        for (s32 stream = 0; stream < streamNum; stream++) {
            s32 sum = 0;
            for (s32 j = 0; j < COEFFICIENT_NUM; j++) {
                s32 difference = *input++ - *means++;
                difference = (*precisions++ * difference) >> 9;
                sum += difference * difference;
            }
            *out++ = sum >> 12;
        }
    }
}

// 0x001474BC
void ModelStartDistances(const s16* frame, Model* model)
{
    memset(model->senoneScores, 0, model->senoneNum * sizeof(s32));
    memset(model->stateScores, 0, model->stateNum * sizeof(s32));
    ModelCalcDistances(model, frame, 0, model->codewordNum * 7 / 10);
}

// 0x00147490
void ModelFinishDistances(const s16* frame, Model* model)
{
    ModelCalcDistances(model, frame, model->codewordNum * 7 / 10, model->codewordNum);
}

// 0x00147450
s32 ModelBestStateScore(s32, s32, const Model* model)
{
    s32 best = SCORE_NONE;
    for (s32 i = model->stateNum - 1; i >= 0; i--) {
        s32 score = model->stateScores[i];
        if (score != 0 && best <= score) {
            best = score;
        }
    }
    return best;
}

// 0x00147BE4
void SearchReset(Search* search)
{
    Network* network = search->network;
    Node* end = network->nodes + network->nodeNum;
    search->threshold = SCORE_NONE;
    if (search->kind != 1) {
        network->nodes[0].link = end;
        for (s32 i = 1; i <= network->nodeNum; i++) {
            network->nodes[i].score = SCORE_NONE;
        }
    }
    for (s32 i = 0; i < network->depth; i++) {
        search->lists[i + 1] = end;
    }
    search->listNum = 1;
    search->lists[0] = network->nodes;
    network->nodes[0].score = 0;
    network->nodes[0].word = 0;
    search->frameNum = 0;
    search->activeSum = 0;
    search->busyFrames = 0;
}

// 0x00147544
void SearchStep(const s16*, Search* search)
{
    Network* network = search->network;
    s32 threshold = search->threshold;
    Node* nodes = network->nodes;
    Node* end = nodes + network->nodeNum;
    s32 best = SCORE_NONE;
    s32 active = 0;

    // the first node starts all words
    if (nodes[0].score != SCORE_NONE) {
        Node* node = nodes + 1;
        search->lists[1] = node;
        while (!(node->isLast)) {
            node->link = node + 1;
            if (node->score < nodes[0].score) {
                node->score = nodes[0].score;
                node->word = nodes[0].word;
            }
            node++;
        }
        if (node->score < nodes[0].score) {
            node->score = nodes[0].score;
            node->word = nodes[0].word;
        }
        node->link = end;
        nodes[0].score = SCORE_NONE;
        nodes[0].word = 0;
    }
    if (++search->listNum > network->depth) {
        search->listNum--;
    }

    for (s32 list = search->listNum - 1; list > 0; list--) {
        Node* node = search->lists[list];
        Node* target = search->lists[list + 1];
        Node* tail = network->trash;
        Node* targetTail = end;
        while (node != end) {
            if (threshold > node->score) {
                if (search->lists[list] == node) {
                    search->lists[list] = node->link;
                }
                node->score = SCORE_NONE;
                node->word = 0;
                node = node->link;
                network->trash->link = node;
                continue;
            }
            active++;
            tail->link = node;
            tail = node;
            s32* stateScore = &search->model->stateScores[node->state];
            s32 score = *stateScore;
            if (score == 0) {
                score = CalcStateScore(search->model, node->state);
                *stateScore = score;
            }
            score += node->score;
            node->score = score;
            u16 word = node->word;
            if (best <= score) {
                best = score;
            }
            u32 next = node->next;
            if (next != NODE_NEXT_NONE) {
                Node* follower = nodes + next;
                if (follower <= search->lists[list + 1]) {
                    search->lists[list + 1] = follower;
                }
                while (true) {
                    if (follower->score < score) {
                        follower->score = score;
                        follower->word = word;
                    }
                    if (best <= follower->score) {
                        best = follower->score;
                    }
                    if (follower == target) {
                        target = target->link;
                    } else if (follower > target) {
                        // the list of the next group is sorted by address
                        targetTail->link = target;
                        targetTail = target;
                        target = target->link;
                        while (target <= follower) {
                            targetTail = target;
                            target = target->link;
                        }
                        if (targetTail == follower && follower->score < score) {
                            follower->score = score;
                            follower->word = word;
                        }
                    }
                    if (search->lists[list + 1] > follower) {
                        search->lists[list + 1] = follower;
                    }
                    targetTail->link = follower;
                    targetTail = follower;
                    if (best <= follower->score) {
                        best = follower->score;
                    }
                    if (follower->isLast) {
                        break;
                    }
                    follower++;
                }
            }
            node = node->link;
        }
        tail->link = node;
        targetTail->link = target;
    }
    search->threshold = best - BEAM_WIDTH;
    search->frameNum++;
    search->activeSum += active;
    if (active > 20) {
        search->busyFrames++;
    }
}

// 0x001479FC
void SearchGetBest(s16* words, s32* scores, s32* num, Search* search)
{
    s32 frameNum = search->frameNum;
    if (frameNum != 0) {
        search->activeSum = search->activeSum / frameNum;
        search->busyFrames = search->busyFrames * 100 / frameNum;
    }
    if (frameNum < 3) {
        *num = 0;
        return;
    }
    s32 count = 0;
    s32 bestScore;
    do {
        Network* network = search->network;
        Node* nodes = network->nodes;
        Node* best = nodes + network->nodeNum;
        bestScore = SCORE_NONE;
        for (s32 i = 0; i < network->nodeNum; i++) {
            Node* node = &nodes[i];
            if (node->wordId == WORD_NONE || node->score <= bestScore) {
                continue;
            }
            s32 j;
            for (j = 0; j < count; j++) {
                if (words[j] == static_cast<s16>(node->wordId)) {
                    break;
                }
            }
            if (j >= count) {
                best = node;
                bestScore = node->score;
            }
        }
        if (bestScore == SCORE_NONE) {
            break;
        }
        words[count] = best->wordId;
        if (scores != NULL) {
            scores[count] = bestScore;
        }
        count++;
    } while (count < search->params->searchResults);
    if (count == 0 && bestScore == SCORE_NONE) {
        if (words != NULL) {
            words[0] = -1;
        }
        if (scores != NULL) {
            scores[0] = SCORE_NONE;
        }
        *num = 1;
        return;
    }
    *num = count;
}

// 0x001466B8
void GetResults(Recognizer* rec, s16* words, s32* scores, s32* num, s32* outScores)
{
    SearchGetBest(words, scores, num, &rec->search);
    if (outScores != NULL && scores != NULL && *num > 0) {
        s32 difference;
        if (*num > 1) {
            difference = (scores[0] - scores[1]) * 100;
        } else {
            s32 score = scores[0];
            difference = (score >= 0 ? score : -score) * 100;
        }
        difference *= 10;
        for (s32 i = 0; i < *num; i++) {
            s32 start = rec->startScore;
            s32 divisor = start != 0 ? (start >= 0 ? start : -start) : 1;
            s32 relative = (scores[i] - start) * 100 / divisor + 200;
            s32 first = scores[0];
            s32 firstDivisor = first != 0 ? (first >= 0 ? first : -first) : 1;
            s32 separation = difference / firstDivisor - 275;
            s32 level = rec->vad.relativeLevel * 10;
            s32 quality = rec->search.busyFrames * -100 / rec->wordNum + 350;
            s32 value = quality + level + relative + separation - 525;
            if (value >= 32768) {
                value = 32767;
            } else if (value <= -32768) {
                value = -32768;
            }
            outScores[i] = value;
        }
    }
    UpdateChannelStats(rec);
}
} // namespace asr
