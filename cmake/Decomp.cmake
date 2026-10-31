# Helper functions of the ACNL decompilation build.

# Every target sees every public header: the game, the libraries and the CRO modules
# include each other freely, just like the original source tree did.
function(decomp_global_includes)
    set(dirs "${CMAKE_SOURCE_DIR}/include")
    file(GLOB lib_includes LIST_DIRECTORIES true "${CMAKE_SOURCE_DIR}/lib/*/include")
    file(GLOB mod_includes LIST_DIRECTORIES true "${CMAKE_SOURCE_DIR}/modules/*/include")
    list(APPEND dirs ${lib_includes} ${mod_includes})
    include_directories(${dirs})
    # the C and C++ library headers of the program (lib/clib) come before the system headers: the decompiled code
    # uses the library ARMCC linked in, never the one of GCC
    include_directories(BEFORE SYSTEM "${CMAKE_SOURCE_DIR}/lib/clib/include/libc")
endfunction()

# decomp_add_unit(<target> <source dir> <kind>)
# One object library per unit (a library, the game or a CRO module). Object libraries keep
# one .o per source file, which is what tools/decomp/check.py compares with the original.
function(decomp_add_unit target src_dir kind)
    file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${src_dir}/*.cpp" "${src_dir}/*.c")
    if(NOT sources)
        return()
    endif()
    # PICA200 shaders (<name>.v.pica, optionally with <name>.g.pica): assembled with picasso into a C++ array
    file(GLOB_RECURSE shaders CONFIGURE_DEPENDS "${src_dir}/*.v.pica")
    foreach(vsh ${shaders})
        if(NOT DECOMP_PICASSO)
            message(FATAL_ERROR "picasso (devkitPro's PICA200 shader assembler) is needed for ${vsh}")
        endif()
        string(REGEX REPLACE "\\.v\\.pica$" "" base "${vsh}")
        set(inputs "${vsh}")
        if(EXISTS "${base}.g.pica")
            list(APPEND inputs "${base}.g.pica")
        endif()
        get_filename_component(name "${base}" NAME)
        set(out "${CMAKE_BINARY_DIR}/shaders/${target}/${name}.shbin.cpp")
        add_custom_command(OUTPUT "${out}"
            COMMAND Python3::Interpreter "${CMAKE_SOURCE_DIR}/tools/build/pica_build.py"
                    --picasso "${DECOMP_PICASSO}" -o "${out}" ${inputs}
            DEPENDS ${inputs} "${CMAKE_SOURCE_DIR}/tools/build/pica_build.py"
            COMMENT "Assembling the shader ${name}")
        list(APPEND sources "${out}")
    endforeach()
    add_library(${target} OBJECT ${sources})
    set_target_properties(${target} PROPERTIES DECOMP_KIND ${kind})
    target_compile_definitions(${target} PRIVATE DECOMP_UNIT_${kind}=1)
    set_property(GLOBAL APPEND PROPERTY DECOMP_UNITS ${target})
endfunction()

# decomp_add_module(<name>)
# A CRO module (modules/<name>). The objects are built like any other unit. Packing them
# into <name>.cro needs CRO tools (a linker with the module's symbol table and a CRO
# packer), which are not part of this project; set DECOMP_MAKECRO to such a tool/script
# to enable the <name>_cro target.
function(decomp_add_module name)
    decomp_add_unit(module_${name} "${CMAKE_SOURCE_DIR}/modules/${name}/src" MODULE)
    if(NOT TARGET module_${name})
        return()
    endif()
    if(DECOMP_MAKECRO)
        add_custom_target(${name}_cro
            COMMAND ${DECOMP_MAKECRO}
                --module ${name}
                --objects "$<TARGET_OBJECTS:module_${name}>"
                --symbols "${DECOMP_CONFIG}/modules/${name}.json"
                --original "${DECOMP_ORIG}/cro/${name}.cro"
                --output "${CMAKE_BINARY_DIR}/cro/${name}.cro"
            COMMAND_EXPAND_LISTS
            DEPENDS module_${name}
            COMMENT "Building ${name}.cro")
        set_property(GLOBAL APPEND PROPERTY DECOMP_CRO_TARGETS ${name}_cro)
    endif()
endfunction()
