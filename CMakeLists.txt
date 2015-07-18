cmake_minimum_required(VERSION 2.8)
project(bandit)

option(BANDIT_BUILD_SPECS   "Build the Bandit specs"                ON)
option(BANDIT_RUN_SPECS     "Run the Bandit specs"                  ON)
option(SNOWHOUSE_IS_CPP11   "Build Snowhouse with C++11 settings"   ON)

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(cotire/CMake/cotire)

include_directories("${PROJECT_SOURCE_DIR}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /MP ")
else()
  # Assume GCC-style arguments
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdeprecated -Wdeprecated-declarations -Wshadow -Wall -W -Werror -Wfloat-equal -Wundef -Wendif-labels")

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

if (BANDIT_BUILD_SPECS)
    FILE(GLOB BanditSpecSourceFiles specs/*.cpp specs/**/*.cpp)
    add_executable(bandit-specs ${BanditSpecSourceFiles} )
    set_target_properties(bandit-specs PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "specs/specs.h")
    set_target_properties(bandit-specs PROPERTIES COTIRE_ADD_UNIT_BUILD FALSE)
    cotire(bandit-specs)
endif()

add_subdirectory(bandit/assertion_frameworks/snowhouse)

if (BANDIT_BUILD_SPECS AND BANDIT_RUN_SPECS)
    add_custom_command(TARGET bandit-specs
                       POST_BUILD
                       COMMAND bandit-specs --no-color --reporter=dots
                       WORKING_DIRECTORY ./bin)
elseif (BANDIT_RUN_SPECS)
    message(WARNING "Unable to run Bandit specs - set:\n  option(BANDIT_BUILD_SPECS, \"Build the Bandit specs\" ON)\nand clear your CMake cache")
endif()

