cmake_minimum_required(VERSION 2.8) 
project(bandit)

include_directories("${PROJECT_SOURCE_DIR}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 ")
else()
  # Assume GCC-style arguments
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfatal-errors -Wall -W -Werror -Wfloat-equal -Wundef -Wendif-labels")

  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.7")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    endif()
  endif()

  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  endif()
endif()

#
# If we're on Mac OS we assume we have libc++, otherwise we assume
# we don't need it. (TODO: make this check more sofisticated)
#
if (CMAKE_HOST_APPLE AND (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

FILE(GLOB BanditSpecSourceFiles specs/*.cpp specs/**/*.cpp)
add_executable(bandit-specs ${BanditSpecSourceFiles} )

add_custom_command(TARGET bandit-specs
                   POST_BUILD
                   COMMAND bandit-specs --no-color --reporter=dots
                   WORKING_DIRECTORY ./bin)
