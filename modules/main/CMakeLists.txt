file(GLOB SRC_FILES src/* src/jni/*)
add_library(HardTimesCoreNative SHARED ${SRC_FILES})
add_library(python3 SHARED IMPORTED)

IF (WIN32)
    set_target_properties(python3 PROPERTIES IMPORTED_IMPLIB ${PROJECT_SOURCE_DIR}/hardtimes-client-core-platform/python/lib/windows-x64/python311.lib)
ELSE()
    set_target_properties(python3 PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/hardtimes-client-core-platform/python/lib/linux-x64/libpython3.11.so)
ENDIF()


find_package(JNI REQUIRED)
target_include_directories(HardTimesCoreNative PRIVATE
        ${JNI_INCLUDE_DIRS}
        src
        )

target_include_directories(HardTimesCoreNative PUBLIC ${PROJECT_SOURCE_DIR}/hardtimes-client-core-platform/python/include)
IF (WIN32)
    target_include_directories(HardTimesCoreNative PUBLIC ${PROJECT_SOURCE_DIR}/hardtimes-client-core-platform/python/lib/windows-x64/include)
ELSE()
    target_include_directories(HardTimesCoreNative PUBLIC ${PROJECT_SOURCE_DIR}/hardtimes-client-core-platform/python/lib/linux-x64/include)
ENDIF()

target_link_libraries(HardTimesCoreNative python3)

IF (WIN32)
    add_custom_command(
        TARGET HardTimesCoreNative POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_SOURCE_DIR}/hardtimes-client-core-platform/python/lib/windows-x64/python311.dll
            ${CMAKE_BINARY_DIR}
    )
ENDIF()
