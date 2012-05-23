

#规定cmake程序的最低版本
cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)


# CMake 2.8.2 has a bug that creates unusable Xcode projects when using ARCHS_STANDARD_32_BIT
# to specify both armv6 and armv7.
if(OGRE_BUILD_PLATFORM_IPHONE AND (CMAKE_VERSION VERSION_EQUAL 2.8.2) AND (CMAKE_GENERATOR STREQUAL "Xcode"))
	message(FATAL_ERROR "CMake 2.8.2 cannot create compatible Xcode projects for iOS, please download the latest version or an older release from http://www.cmake.org/files/")
endif()


#project 名称
project(geek)

include(CMakeDependentOption)
include(CheckCXXCompilerFlag)

# 包含物理处理器（核）数量命令
include(ProcessorCount)

#此处还使用了一个CMAKE_POLICY命令来设置版本的兼容性，以避免一些不必要的警告信息。
if( COMMAND CMAKE_POLICY )
	CMAKE_POLICY( SET CMP0003 NEW )
endif( COMMAND CMAKE_POLICY )

#这个变量用来定义自己的 cmake 模块所在的路径。
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

#寻找公共cmake_modle
include(find_geek_cmake_modle)
#添加公共model路径
list(APPEND CMAKE_MODULE_PATH ${GEEK_CMAKE_MODULE_PATH})

#包含项目主要module
include(project_module)
include(dependencies)
include(toolkits)
include(view)
include(framework)
include(game_made_in_geeks)
include(select_compiler)
include(UseDoxygen)


include(script)

set(CMAKE_DEBUG_POSTFIX "_d")
add_definitions(-D_DEBUG_POSTFIX="_d" -DBOOST_ALL_NO_LIB)

ProcessorCount(pcount)
if(NOT pcount EQUAL 0)
  add_definitions(-DGEEK_PROCESSOR_COUNT=${pcount})
else()
  add_definitions(-DGEEK_PROCESSOR_COUNT=1)
endif()

#
if (WIN32)
  include(windows)
endif ()
if(APPLE)	
  include(apple)
endif(APPLE)
project_begin()
	list(APPEND geek_include_path ${PROJECT_SOURCE_DIR})
	
	if(build_static)
		set(TOOLKITS_NAME "libgeek_tooklits")
		set(FRAMEWORK_NAME "libgeek_framework")
		add_definitions(-DGEEK_STATIC_LINK)
	else(build_static)
		set(TOOLKITS_NAME "geek_toolkits")
		set(FRAMEWORK_NAME "geek_framework")
		add_definitions(-DGEEK_DYN_LINK)
	endif(build_static)
	list(APPEND geek_librarys ${TOOLKITS_NAME})
	list(APPEND geek_librarys ${FRAMEWORK_NAME})
	
	geek_setup_dependencies()
	geek_setup_toolkits()
	geek_setup_view()
	geek_setup_script()
	
	
	include_directories(${CMAKE_SOURCE_DIR})
	add_subdirectory(${PROJECT_SOURCE_DIR}/toolkits)
	
	add_subdirectory(${PROJECT_SOURCE_DIR}/framework)
	add_subdirectory(${PROJECT_SOURCE_DIR}/executable)
	
	
	if(unit_test)
		geek_setup_toolkits_test()
		
		if(toolkit_dyn_lib_manager)
			add_subdirectory(${PROJECT_SOURCE_DIR}/toolkits/libs/dyn_lib_manager/test_plugin)
		endif(toolkit_dyn_lib_manager)
	endif(unit_test)
	
	if(enable_example)
		geek_setup_toolkits_example()
	endif(enable_example)
	
	foreach(include_path ${geek_include_path})
		install(DIRECTORY ${include_path}/geek DESTINATION . PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE)
	endforeach(include_path)
	
	geek_made_in_geeks()
	
project_end()




	
	

