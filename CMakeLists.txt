# To the extent possible under law, the author(s) have dedicated all
# copyright and related and neighboring rights to this software to
# the public domain worldwide. This software is distributed without
# any warranty.
#
# For details, see <http://creativecommons.org/publicdomain/zero/1.0/>.

set(XXHASH_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

file(STRINGS "${XXHASH_DIR}/xxhash.h" XXHASH_VERSION_MAJOR REGEX "^#define XXH_VERSION_MAJOR +([0-9]+) *$")
string(REGEX REPLACE "^#define XXH_VERSION_MAJOR +([0-9]+) *$" "\\1" XXHASH_VERSION_MAJOR "${XXHASH_VERSION_MAJOR}")
file(STRINGS "${XXHASH_DIR}/xxhash.h" XXHASH_VERSION_MINOR REGEX "^#define XXH_VERSION_MINOR +([0-9]+) *$")
string(REGEX REPLACE "^#define XXH_VERSION_MINOR +([0-9]+) *$" "\\1" XXHASH_VERSION_MINOR "${XXHASH_VERSION_MINOR}")
file(STRINGS "${XXHASH_DIR}/xxhash.h" XXHASH_VERSION_RELEASE REGEX "^#define XXH_VERSION_RELEASE +([0-9]+) *$")
string(REGEX REPLACE "^#define XXH_VERSION_RELEASE +([0-9]+) *$" "\\1" XXHASH_VERSION_RELEASE "${XXHASH_VERSION_RELEASE}")
set(XXHASH_VERSION_STRING "${XXHASH_VERSION_MAJOR}.${XXHASH_VERSION_MINOR}.${XXHASH_VERSION_RELEASE}")
set(XXHASH_LIB_VERSION ${XXHASH_VERSION_STRING})
set(XXHASH_LIB_SOVERSION "${XXHASH_VERSION_MAJOR}")
mark_as_advanced(XXHASH_VERSION_MAJOR XXHASH_VERSION_MINOR XXHASH_VERSION_RELEASE XXHASH_VERSION_STRING XXHASH_LIB_VERSION XXHASH_LIB_SOVERSION)

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN TRUE)

if ("${CMAKE_VERSION}" VERSION_LESS "3.0")
  project(XXHASH C)
else ()
  cmake_policy(SET CMP0048 NEW)
  project(XXHASH
    VERSION ${XXHASH_VERSION_STRING}
    LANGUAGES C)
endif ()

cmake_minimum_required(VERSION 3.3)

include_directories("${XXHASH_DIR}")

# libxxhash
add_library(xxhash "${XXHASH_DIR}/xxhash.c")
target_compile_definitions(xxhash PUBLIC XXH_EXPORT)

set_target_properties(xxhash PROPERTIES
  SOVERSION "${XXHASH_VERSION_STRING}"
  VERSION "${XXHASH_VERSION_STRING}"
  )

# Extra warning flags
include(CheckCCompilerFlag)
foreach (flag
  -Wall -Wextra -Wcast-qual -Wcast-align -Wshadow
  -Wstrict-aliasing=1 -Wswitch-enum -Wdeclaration-after-statement
  -Wstrict-prototypes -Wundef)
  # Because https://gcc.gnu.org/wiki/FAQ#wnowarning
  string(REGEX REPLACE "\\-Wno\\-(.+)" "-W\\1" flag_to_test "${flag}")
  string(REGEX REPLACE "[^a-zA-Z0-9]+" "_" test_name "CFLAG_${flag_to_test}")

  check_c_compiler_flag("${ADD_COMPILER_FLAGS_PREPEND} ${flag_to_test}" ${test_name})

  if (${test_name})
    set(CMAKE_C_FLAGS "${flag} ${CMAKE_C_FLAGS}")
  endif ()

  unset(test_name)
  unset(flag_to_test)
endforeach (flag)

include(GNUInstallDirs)

install(TARGETS xxhash EXPORT xxhashConfig
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FRAMEWORK DESTINATION ${CMAKE_INSTALL_PREFIX}
  )
install(FILES "${XXHASH_DIR}/xxhash.h"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/xxhash"
  )
install(
  EXPORT xxhashConfig
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/xxhash
)
export(
  TARGETS xxhash
  FILE xxhashConfig.cmake
)
