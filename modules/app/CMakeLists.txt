file(GLOB SRC_FILES src/*)
add_executable(HardTimesCoreNativeApp ${SRC_FILES})

target_include_directories(HardTimesCoreNativeApp PRIVATE ${PROJECT_SOURCE_DIR}/modules/main/src)

target_link_libraries(HardTimesCoreNativeApp HardTimesCoreNative)
