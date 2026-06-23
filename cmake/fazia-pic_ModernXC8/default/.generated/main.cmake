include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(fazia_pic_ModernXC8_default_library_list )

# Handle files with suffix (s|as|asm|AS|ASM|As|aS|Asm), for group default-XC8
if(fazia_pic_ModernXC8_default_default_XC8_FILE_TYPE_assemble)
add_library(fazia_pic_ModernXC8_default_default_XC8_assemble OBJECT ${fazia_pic_ModernXC8_default_default_XC8_FILE_TYPE_assemble})
    fazia_pic_ModernXC8_default_default_XC8_assemble_rule(fazia_pic_ModernXC8_default_default_XC8_assemble)
    list(APPEND fazia_pic_ModernXC8_default_library_list "$<TARGET_OBJECTS:fazia_pic_ModernXC8_default_default_XC8_assemble>")

endif()

# Handle files with suffix S, for group default-XC8
if(fazia_pic_ModernXC8_default_default_XC8_FILE_TYPE_assemblePreprocess)
add_library(fazia_pic_ModernXC8_default_default_XC8_assemblePreprocess OBJECT ${fazia_pic_ModernXC8_default_default_XC8_FILE_TYPE_assemblePreprocess})
    fazia_pic_ModernXC8_default_default_XC8_assemblePreprocess_rule(fazia_pic_ModernXC8_default_default_XC8_assemblePreprocess)
    list(APPEND fazia_pic_ModernXC8_default_library_list "$<TARGET_OBJECTS:fazia_pic_ModernXC8_default_default_XC8_assemblePreprocess>")

endif()

# Handle files with suffix [cC], for group default-XC8
if(fazia_pic_ModernXC8_default_default_XC8_FILE_TYPE_compile)
add_library(fazia_pic_ModernXC8_default_default_XC8_compile OBJECT ${fazia_pic_ModernXC8_default_default_XC8_FILE_TYPE_compile})
    fazia_pic_ModernXC8_default_default_XC8_compile_rule(fazia_pic_ModernXC8_default_default_XC8_compile)
    list(APPEND fazia_pic_ModernXC8_default_library_list "$<TARGET_OBJECTS:fazia_pic_ModernXC8_default_default_XC8_compile>")

endif()


# Main target for this project
add_executable(fazia_pic_ModernXC8_default_image_LYyqep8W ${fazia_pic_ModernXC8_default_library_list})

set_target_properties(fazia_pic_ModernXC8_default_image_LYyqep8W PROPERTIES
    OUTPUT_NAME "default"
    SUFFIX ".elf"
    ADDITIONAL_CLEAN_FILES "${output_extensions}"
    RUNTIME_OUTPUT_DIRECTORY "${fazia_pic_ModernXC8_default_output_dir}")
target_link_libraries(fazia_pic_ModernXC8_default_image_LYyqep8W PRIVATE ${fazia_pic_ModernXC8_default_default_XC8_FILE_TYPE_link})

# Add the link options from the rule file.
fazia_pic_ModernXC8_default_link_rule( fazia_pic_ModernXC8_default_image_LYyqep8W)


