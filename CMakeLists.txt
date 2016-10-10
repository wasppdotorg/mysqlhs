
cmake_minimum_required (VERSION 3.0)

set (TARGET mysqlhs)
project (${TARGET})

aux_source_directory (. Source_DIRECTORIES)
add_executable (${TARGET} ${Source_DIRECTORIES})

include_directories (/usr/include /usr/local/include)

find_library (POSIXThread_LIBRARIES NAMES pthread)
find_package (Boost 1.58.0 COMPONENTS system)

if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
	set (CMAKE_COMPILER_IS_CLANGCXX 1)
endif()

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGCXX)
	set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -g -fexceptions -O2 -DNDEBUG")
endif()

target_link_libraries (${TARGET} ${POSIXThread_LIBRARIES})
target_link_libraries (${TARGET} ${Boost_LIBRARIES})

