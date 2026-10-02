# Helper functions of the ACNL decompilation build.

# Every target sees every public header: the game, the libraries and the CRO modules
# include each other freely, just like the original source tree did.
function(decomp_global_includes)
    set(dirs "${CMAKE_SOURCE_DIR}/include")
    file(GLOB lib_includes LIST_DIRECTORIES true "${CMAKE_SOURCE_DIR}/lib/*/include")
    file(GLOB mod_includes LIST_DIRECTORIES true "${CMAKE_SOURCE_DIR}/modules/*/include")
    list(APPEND dirs ${lib_includes} ${mod_includes})
    include_directories(${dirs})
endfunction()

# decomp_add_unit(<target> <source dir> <kind>)
# One object library per unit (a library, the game or a CRO module). Object libraries keep
# one .o per source file, which is what tools/decomp/check.py compares with the original.
function(decomp_add_unit target src_dir kind)
    file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${src_dir}/*.cpp" "${src_dir}/*.c")
    if(NOT sources)
        return()
    endif()
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
