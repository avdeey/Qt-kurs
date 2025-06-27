# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\kurs_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\kurs_autogen.dir\\ParseCache.txt"
  "kurs_autogen"
  )
endif()
