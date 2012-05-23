find_path(geek_sdk_DIR NAMES Cocoa.h HINTS ${Cocoa_INC_SEARCH_PATH} ${Cocoa_PKGC_INCLUDE_DIRS} PATH_SUFFIXES Cocoa)
include(view)
message(${geek_view_name})
find_path(
	geek_sdk_DIR	
	geek/framework/component_framework.hpp
	PATHS 		
	${GEEK_CMAKE_MODULE_PATH}/..
	NO_DEFAULT_PATH
	)
link_directories(${geek_sdk_DIR}/lib)
link_directories(${geek_sdk_DIR}/dependencies/lib)
include_directories(${geek_sdk_DIR}/dependencies/include)
include_directories(${geek_sdk_DIR})
if(WIN32)	
	list(APPEND geek_libraries "optimized;geek_toolkits;debug;geek_toolkits_d;")
	list(APPEND geek_libraries "optimized;geek_framework;debug;geek_framework_d;")
else()
	list(APPEND geek_libraries "geek_toolkits")
	list(APPEND geek_libraries "geek_framework")
endif(WIN32)

foreach(view_name ${geek_view_name})
	if(WIN32)	
		list(APPEND geek_libraries "optimized;geek_view_${view_name};debug;geek_view_${view_name}_d;")
	else()
		list(APPEND geek_libraries "geek_view_${view_name}")
	endif(WIN32)
endforeach(view_name)
set(geek_dependencies_DIR "${geek_sdk_DIR}/dependencies")

set(Boost_ADDITIONAL_VERSIONS "1.50" "1.50.0"  "1.49" "1.49.0" "1.48" "1.48.0" "1.47" "1.47.0" "1.46" "1.46.0" "1.46.1" "1.44" "1.44.0" "1.42" "1.42.0" "1.41.0" "1.41" "1.40.0" "1.40" "1.39.0" "1.39" "1.38.0" "1.38" "1.37.0" "1.37" )
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
set(BOOST_INCLUDEDIR    "${geek_dependencies_DIR}/include")
set(BOOST_LIBRARYDIR    "${geek_dependencies_DIR}/lib")

find_package(Boost COMPONENTS "date_time" "system" "filesystem" "iostreams" "log_setup" "log" "regex" "unit_test_framework" "zlib" "thread" "context" "chrono")
	
install(DIRECTORY ${geek_sdk_DIR}/bin/release CONFIGURATIONS Release DESTINATION bin  PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE  PATTERN "*.exe" EXCLUDE PATTERN "*.cfg" EXCLUDE PATTERN "*.xml" EXCLUDE )
install(DIRECTORY ${geek_sdk_DIR}/bin/debug CONFIGURATIONS Debug DESTINATION bin  PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE  PATTERN "*.exe" EXCLUDE  PATTERN "*.cfg" EXCLUDE PATTERN "*.xml" EXCLUDE )
#set(geek_libraries "optimized;F:/disk/install/dependencies/vc90/lib/libboost_filesystem-vc90-mt-1_47.lib;debug;F:/disk/install/dependencies/vc90/lib/libboost_filesystem-vc90-mt-gd-1_47.lib")


include(ogre_cfg)


message(${geek_sdk_DIR}==${geek_libraries})	
