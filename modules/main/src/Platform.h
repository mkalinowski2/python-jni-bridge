#pragma once

#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
#define OS_WIN
#endif

#if defined(OS_WIN)
#define WINDOWS_DLLEXPORT __declspec(dllexport)
#define WINDOWS_DLLIMPORT __declspec(dllimport)
#include "windows.h"
#define PLATFORM_SETVBUF_TYPE _IONBF
#define PLATFORM_PATH_SEPARATOR ;
#define PLATFORM_DIR_SEPARATOR \\\

void loadPythonLibraryGlobally(const char* path) {
    // There is no need to load python library globally as on UNIX
}

#else
#define WINDOWS_DLLEXPORT 
#define WINDOWS_DLLIMPORT 
#include <dlfcn.h>
#define PLATFORM_SETVBUF_TYPE _IOLBF
#define PLATFORM_PATH_SEPARATOR :
#define PLATFORM_DIR_SEPARATOR /

void loadPythonLibraryGlobally(const char* path) {
    static void* handle = nullptr;
    if (handle == NULL) {
        std::stringstream pythonLibPathStream;
        pythonLibPathStream << path << "/libpython3.11.so";
        handle = dlopen(pythonLibPathStream.str().c_str(), RTLD_LAZY | RTLD_GLOBAL);
        if (!handle) {
            fprintf(stderr, "%s\n", dlerror());
            exit(EXIT_FAILURE);
    }
  }
}

#endif