cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project( bulk VERSION ${PROJECT_VERSION})

enable_testing()

add_subdirectory(lib)
add_subdirectory(app)
add_subdirectory(tests)


