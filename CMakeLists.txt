cmake_minimum_required(VERSION 2.8)

project(bandit)

include_directories("${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/snowhouse")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ -Wfatal-errors -Wall -W -Werror -Wfloat-equal -Wundef -Wendif-labels -Wshadow -pedantic-errors")

FILE(GLOB BanditSpecSourceFiles specs/*.cpp specs/**/*.cpp)
add_executable(bandit-specs ${BanditSpecSourceFiles} )

add_custom_command(TARGET bandit-specs
                   POST_BUILD
                   COMMAND bandit-specs
                   WORKING_DIRECTORY ./bin)
