# The following variables contains the files used by the different stages of the build process.
set(FrontEnd_SSC_default_default_XC8_FILE_TYPE_assemble)
set_source_files_properties(${FrontEnd_SSC_default_default_XC8_FILE_TYPE_assemble} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${FrontEnd_SSC_default_default_XC8_FILE_TYPE_assemble})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(FrontEnd_SSC_default_default_XC8_FILE_TYPE_assemblePreprocess)
set_source_files_properties(${FrontEnd_SSC_default_default_XC8_FILE_TYPE_assemblePreprocess} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${FrontEnd_SSC_default_default_XC8_FILE_TYPE_assemblePreprocess})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(FrontEnd_SSC_default_default_XC8_FILE_TYPE_compile
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/cbuffer.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/frame.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/functions.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/isr.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/main_old.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/myfunc/Tsensor.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/myfunc/ads8332.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/myfunc/analog.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/myfunc/dac8568.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/myfunc/display.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/myfunc/maths.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/myfunc/spi.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/myfunc/wr_eeprom.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/setup.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/uartbuf.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/utils.c")
set_source_files_properties(${FrontEnd_SSC_default_default_XC8_FILE_TYPE_compile} PROPERTIES LANGUAGE C)
set(FrontEnd_SSC_default_default_XC8_FILE_TYPE_link)
set(FrontEnd_SSC_default_image_name "default.elf")
set(FrontEnd_SSC_default_image_base_name "default")

# The output directory of the final image.
set(FrontEnd_SSC_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/FrontEnd-SSC")

# The full path to the final image.
set(FrontEnd_SSC_default_full_path_to_image ${FrontEnd_SSC_default_output_dir}/${FrontEnd_SSC_default_image_name})

# Potential output file extensions
set(output_extensions
    .hex
    .hxl
    .mum
    .o
    .sdb
    .sym
    .cmf)
list(TRANSFORM output_extensions PREPEND "${FrontEnd_SSC_default_output_dir}/${FrontEnd_SSC_default_image_base_name}")
