
cmake_minimum_required (VERSION 3.0)

set (TARGET mysqlhs)
project (${TARGET})

aux_source_directory (. Source_DIRECTORIES)
add_executable (${TARGET} ${Source_DIRECTORIES})

include_directories (/usr/include /usr/local/include)

find_package (Boost 1.55.0 COMPONENTS system)
find_library (PThread_LIBRARIES NAMES pthread)


find_package (Threads)

if (CMAKE_COMPILER_IS_GNUCXX)
	set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -g -fexceptions -O2 -DNDEBUG")
endif()

target_link_libraries (${TARGET} ${Boost_LIBRARIES})
target_link_libraries (${TARGET} ${PThread_LIBRARIES})

