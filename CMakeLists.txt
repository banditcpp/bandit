cmake_minimum_required(VERSION 2.8)

project(bandit)

include_directories("${PROJECT_SOURCE_DIR}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wfatal-errors -Wall -W -Werror -Wfloat-equal -Wundef -Wendif-labels")

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-stdlib=libc++ HAS_STDLIB_FLAG)
if (HAS_STDLIB_FLAG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

FILE(GLOB BanditSpecSourceFiles specs/*.cpp specs/**/*.cpp)
add_executable(bandit-specs ${BanditSpecSourceFiles} )

add_custom_command(TARGET bandit-specs
                   POST_BUILD
                   COMMAND bandit-specs
                   WORKING_DIRECTORY ./bin)
