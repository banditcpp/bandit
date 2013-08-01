cmake_minimum_required(VERSION 2.8)

project(bandit)

include_directories("${PROJECT_SOURCE_DIR}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

if (MSVC)
  add_definitions(-wd4800)  # Suppress ''type' : forcing value to bool 'true' or 'false' (performance warning)'
else()
  # Assume GCC-style arguments
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wfatal-errors -Wall -W -Werror -Wfloat-equal -Wundef -Wendif-labels")
endif()

#
# If we're on Mac OS we assume we have libc++, otherwise we assume
# we don't need it. (TODO: make this check more sofisticated)
#
if (CMAKE_HOST_APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

FILE(GLOB BanditSpecSourceFiles specs/*.cpp specs/**/*.cpp)
add_executable(bandit-specs ${BanditSpecSourceFiles} )

add_custom_command(TARGET bandit-specs
                   POST_BUILD
                   COMMAND bandit-specs --no-color --reporter=singleline
                   WORKING_DIRECTORY ./bin)
