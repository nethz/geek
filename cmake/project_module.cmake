
macro(geek_setup_compiler)
	
endmacro(geek_setup_compiler)
#增加新的组件
macro(add_component component)
	option(${component}_component "enable ${component}_component" ON)
	if(${${component}_component})
		list(APPEND component_list ${component})
		set(component_dir "${CMAKE_CURRENT_SOURCE_DIR}/components/${component}")
		set(test_dir "${component_dir}/test")
		
		if(NOT EXISTS "${component_dir}/media") 
			file(MAKE_DIRECTORY "${component_dir}/media")
		endif(NOT EXISTS "${component_dir}/media")
		
		list(APPEND ${LIB_NAME}_FILES ${dir}${var})
        add_subdirectory(${component_dir})
	    if(${unit_test})
			if(EXISTS ${test_dir}) 
				add_subdirectory(${test_dir})
				message(${component}_component_test ${component}_component)
				add_dependencies(${component}_component_test ${component}_component)
			endif(EXISTS ${test_dir})
    	endif(${unit_test})
	endif(${${component}_component})
endmacro(add_component)

macro(add_project_file group dir)
	foreach(var ${ARGN})
		list(APPEND ${LIB_NAME}_FILES ${dir}/${var})
		source_group(${group} FILES ${dir}/${var})
	endforeach()
endmacro(add_project_file)



macro(set_project_files group dir match)

	file(GLOB project_files  ${dir}/*${match})
	foreach(f ${project_files})
		get_filename_component(f ${f} NAME_WE)
		add_project_file(${group} ${dir} "${f}${match}")
	endforeach()
	
endmacro(set_project_files)

# create vcproj.user file for Visual Studio to set debug working directory
function(create_vcproj_userfile target)
    if (MSVC)
		file(TO_CMAKE_PATH  ${CMAKE_BINARY_DIR} MY_PROJECT_BINARY_DIR)
		file(TO_CMAKE_PATH  ${CMAKE_INSTALL_PREFIX} MY_PROJECT_WORK_DIR)
        configure_file(
	       ${GEEK_CMAKE_MODULE_PATH}/templates/VisualStudioUserFile.vcproj.user.in
	       ${CMAKE_CURRENT_BINARY_DIR}/${target}.vcproj.user
	       @ONLY
	    )
        configure_file(
	       ${GEEK_CMAKE_MODULE_PATH}/templates/VisualStudioUserFile.vcxproj.user.in
	       ${CMAKE_CURRENT_BINARY_DIR}/${target}.vcxproj.user
	       @ONLY
	    )
   endif (MSVC)
endfunction(create_vcproj_userfile)

#项目开始时候配置
macro( project_begin )
#	include(AutoCreate)
	include(precompiled_header)
	get_filename_component(${PROJECT_NAME}_ROOT_DIR "${PROJECT_SOURCE_DIR}" ABSOLUTE)
	
	#设置输出目录
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
	set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

	
	#开启与编译头
	option(precompiled_header "enable precompiled header" ON)
	#开启单元测试
	option(unit_test "enable unit test" ON)
	option(enable_example "enable example" ON)
	if(build_ios)
		unset(build_static CACHE)
		set(build_static ON)
	else()
		option(build_static "build static library" OFF)
	endif(build_ios)
	
	#是否安装media
	option(install_media "install media" OFF)
	
	
	option(enable_package "enable package" OFF)
	if (WIN32)
	
		#是否采用Win32模式
		option(enable_win32 "enable win32 mode" OFF)
		#是Win32 还是控制台
		if(enable_win32)
			add_definitions( -D_NOCONSOLE )
		else(enable_win32)
			add_definitions( -D_CONSOLE )
		endif(enable_win32)
	else(WIN32)
		unset(enable_win32 CACHE)
		set(enable_win32, OFF)
	endif (WIN32)
endmacro( project_begin )


#项目结束时候配置
macro( project_end )
	
endmacro( project_end )

#子项目开始
macro( sub_project_begin target)
	set( LIB_NAME ${target} )
	set( EXE_NAME ${target} )
	string(TOUPPER ${LIB_NAME} LIB_NAME_TOUPPER)
	include_directories(src)
endmacro( sub_project_begin )



#子项目结束
macro( sub_project_end )

	install(TARGETS ${LIB_NAME} 
		RUNTIME DESTINATION bin/debug
		CONFIGURATIONS Debug
		LIBRARY DESTINATION lib/debug
		CONFIGURATIONS Debug
		ARCHIVE DESTINATION lib/debug
		CONFIGURATIONS Debug
	)

	install(TARGETS ${LIB_NAME} 
		RUNTIME DESTINATION bin/release
		CONFIGURATIONS Release
		LIBRARY DESTINATION lib/release 
		CONFIGURATIONS Release
		ARCHIVE DESTINATION lib/release
		CONFIGURATIONS Release
	)
	if(build_ios)
		set_target_properties(${LIB_NAME} PROPERTIES XCODE_ATTRIBUTE_GCC_THUMB_SUPPORT "NO")
		set_target_properties(${LIB_NAME} PROPERTIES XCODE_ATTRIBUTE_GCC_UNROLL_LOOPS "YES")
		set_target_properties(${LIB_NAME} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
		set_target_properties(${LIB_NAME} PROPERTIES XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER "YES")
	endif(build_ios)
	create_vcproj_userfile(${LIB_NAME})
endmacro( sub_project_end )



#组件项目开始
macro( component_begin target)
	#设置组件名称
	set( COMPONENT_NAME ${target} )
	add_definitions( -D_COMPONENT_NAME=\"${COMPONENT_NAME}\" )
	sub_project_begin(${target}_component)
	
	set_project_files(interface\\\\\\\\common ${PROJECT_SOURCE_DIR}/interface/common/ .hpp)
	set_project_files(interface\\\\\\\\${COMPONENT_NAME} ${PROJECT_SOURCE_DIR}/interface/${COMPONENT_NAME}/ .hpp)
	set_project_files(include src/ .hpp)
	set_project_files(src src/ .cpp)
	set_project_files(def src/ .def)#考虑自动生成 to do
	
endmacro( component_begin )



#组件项目结束
macro( component_end )

 
	
	sub_project_end()

endmacro( component_end )



#单元测试项目开始
macro( test_component_begin target)
	#设置组件名称
	set( COMPONENT_NAME ${target} )
	set( TEST_COMPONENT_NAME ${target} )
	sub_project_begin(${target}_component_test)

	
endmacro( test_component_begin )




#单元测试项目结束
macro( test_component_end )
	sub_project_end()
endmacro( test_component_end )

#macro(geek_find name dir file)
	
#	find_path(
#		${name}_DIR
#		${file}
#		PATHS 
#		${Dependencies_DIR}/${dir}
#		NO_DEFAULT_PATH
#		)
	



#	if(NOT ${name}_DIR)
#		message("[You have to select the directory of the ${name}!]  ")
#	endif(NOT ${name}_DIR)
	
#endmacro(geek_find)

