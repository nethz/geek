#用此文件定义dependencies目录

include(MacroLogFeature)
include(FindPkgMacros)

macro(geek_setup_dependencies)

	
	option(install_dependencies "install dependencies" OFF)
	
	include(find_geek_dependencies)
	include_directories(${geek_dependencies_path}/include)
	message(${geek_dependencies_path}/include)
	if(WIN32)
		set(geek_dependencies_lib_path ${geek_dependencies_path}/${compiler}/lib)
		set(geek_dependencies_bin_path ${geek_dependencies_path}/${compiler}/bin)
	else(WIN32)
		set(geek_dependencies_lib_path ${geek_dependencies_path}/lib)
		set(geek_dependencies_bin_path ${geek_dependencies_path}/bin)
	endif(WIN32)
	
	link_directories(${geek_dependencies_lib_path})
	set(Boost_ADDITIONAL_VERSIONS "1.50" "1.50.0"  "1.49" "1.49.0" "1.48" "1.48.0" "1.47" "1.47.0" "1.46" "1.46.0" "1.46.1" "1.44" "1.44.0" "1.42" "1.42.0" "1.41.0" "1.41" "1.40.0" "1.40" "1.39.0" "1.39" "1.38.0" "1.38" "1.37.0" "1.37" )
	set(Boost_USE_STATIC_LIBS        ON)
	set(Boost_USE_MULTITHREADED      ON)
	set(Boost_USE_STATIC_RUNTIME    OFF)

	
	set(BOOST_INCLUDEDIR    "${geek_dependencies_path}/include")

	set(BOOST_LIBRARYDIR    "${geek_dependencies_lib_path}")
	
	find_package(Boost COMPONENTS "date_time" "system" "filesystem" "iostreams" "log_setup" "log" "regex" "unit_test_framework" "zlib" "thread" "context" "chrono")
	if(install_dependencies)
		install(DIRECTORY ${geek_dependencies_path}/include DESTINATION dependencies PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE)
		install(DIRECTORY ${geek_dependencies_lib_path} DESTINATION dependencies PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE)
	endif(install_dependencies)
	
	
	if(install_media)
		install(DIRECTORY ${geek_dependencies_path}/media DESTINATION . PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE)
	endif(install_media)
	install(DIRECTORY ${geek_dependencies_bin_path}/release DESTINATION bin CONFIGURATIONS Release PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE)
	install(DIRECTORY ${geek_dependencies_bin_path}/debug DESTINATION bin CONFIGURATIONS Debug PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE)
	install(DIRECTORY ${GEEK_CMAKE_MODULE_PATH} DESTINATION . PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE)
endmacro(geek_setup_dependencies)
