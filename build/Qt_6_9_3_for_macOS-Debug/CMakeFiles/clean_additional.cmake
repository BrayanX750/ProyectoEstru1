# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/ProyectoFinder_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/ProyectoFinder_autogen.dir/ParseCache.txt"
  "ProyectoFinder_autogen"
  )
endif()
