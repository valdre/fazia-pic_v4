include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(FrontEnd_SSC_default_library_list )

# Handle files with suffix (s|as|asm|AS|ASM|As|aS|Asm), for group default-XC8
if(FrontEnd_SSC_default_default_XC8_FILE_TYPE_assemble)
add_library(FrontEnd_SSC_default_default_XC8_assemble OBJECT ${FrontEnd_SSC_default_default_XC8_FILE_TYPE_assemble})
    FrontEnd_SSC_default_default_XC8_assemble_rule(FrontEnd_SSC_default_default_XC8_assemble)
    list(APPEND FrontEnd_SSC_default_library_list "$<TARGET_OBJECTS:FrontEnd_SSC_default_default_XC8_assemble>")

endif()

# Handle files with suffix S, for group default-XC8
if(FrontEnd_SSC_default_default_XC8_FILE_TYPE_assemblePreprocess)
add_library(FrontEnd_SSC_default_default_XC8_assemblePreprocess OBJECT ${FrontEnd_SSC_default_default_XC8_FILE_TYPE_assemblePreprocess})
    FrontEnd_SSC_default_default_XC8_assemblePreprocess_rule(FrontEnd_SSC_default_default_XC8_assemblePreprocess)
    list(APPEND FrontEnd_SSC_default_library_list "$<TARGET_OBJECTS:FrontEnd_SSC_default_default_XC8_assemblePreprocess>")

endif()

# Handle files with suffix [cC], for group default-XC8
if(FrontEnd_SSC_default_default_XC8_FILE_TYPE_compile)
add_library(FrontEnd_SSC_default_default_XC8_compile OBJECT ${FrontEnd_SSC_default_default_XC8_FILE_TYPE_compile})
    FrontEnd_SSC_default_default_XC8_compile_rule(FrontEnd_SSC_default_default_XC8_compile)
    list(APPEND FrontEnd_SSC_default_library_list "$<TARGET_OBJECTS:FrontEnd_SSC_default_default_XC8_compile>")

endif()


# Main target for this project
add_executable(FrontEnd_SSC_default_image_leWfiYlS ${FrontEnd_SSC_default_library_list})

set_target_properties(FrontEnd_SSC_default_image_leWfiYlS PROPERTIES
    OUTPUT_NAME "default"
    SUFFIX ".elf"
    ADDITIONAL_CLEAN_FILES "${output_extensions}"
    RUNTIME_OUTPUT_DIRECTORY "${FrontEnd_SSC_default_output_dir}")
target_link_libraries(FrontEnd_SSC_default_image_leWfiYlS PRIVATE ${FrontEnd_SSC_default_default_XC8_FILE_TYPE_link})

# Add the link options from the rule file.
FrontEnd_SSC_default_link_rule( FrontEnd_SSC_default_image_leWfiYlS)


