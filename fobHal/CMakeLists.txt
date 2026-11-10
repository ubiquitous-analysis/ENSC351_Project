# DO NOT FORGET TO REMOVE THIS CMAKE FILE!!


cmake_minimum_required(VERSION 3.10)
project(hal)

# Gather all source files
file(GLOB MY_SOURCES "src/*.c")

# Create the static library
add_library(hal STATIC ${MY_SOURCES})

# Expose the include directory publicly using target-specific commands
target_include_directories(hal PUBLIC include)

# Find and link libgpiod via pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(GPIOD REQUIRED IMPORTED_TARGET libgpiod)

target_link_libraries(hal PRIVATE PkgConfig::GPIOD)

add_executable(testFob src/main.c)
target_link_libraries(testFob PRIVATE hal)
set_target_properties(testFob PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "~/ensc351/public"
)