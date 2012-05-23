
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

project_begin()

add_subdirectory(${PROJECT_SOURCE_DIR}/toolkits)

project_end()




	
	

