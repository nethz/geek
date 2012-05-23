
message(STATUS "setup toolkits begin" )

set(geek_toolkits_name "log_system" "exception" "singleton" "dyn_lib_manager" "concurrent" "timer_manager" "component" "platform" "key_value_server" "auto_list" "pool" "task_manager")


set(toolkits_path ${PROJECT_SOURCE_DIR}/toolkits)
set(toolkits_include_path ${toolkits_path}/geek/toolkits)
set(generation_toolkits_files_path ${CMAKE_BINARY_DIR}/generation/toolkits)

set(toolkits_template_path ${GEEK_CMAKE_MODULE_PATH}/templates/toolkits)
set(toolkits_include_template_path ${toolkits_template_path}/geek/toolkits)
set(toolkit_include_template_path ${toolkits_include_template_path}/toolkit)
	




macro(create_toolkit_include toolkit)
	set(toolkit_include_path ${toolkits_include_path}/${toolkit})
	
	set(toolkit ${toolkit})
	string(TOUPPER ${toolkit} toolkit_toupper)
	
	configure_file(
	   ${toolkits_include_template_path}/toolkit.hpp.in
	   ${toolkits_include_path}/${toolkit}.hpp
	   @ONLY
	)


	configure_file(
	   ${toolkit_include_template_path}/config.hpp.in
	   ${toolkit_include_path}/config.hpp
	   @ONLY
	)
	
	configure_file(
	   ${toolkit_include_template_path}/toolkit.hpp.in
	   ${toolkit_include_path}/${toolkit}.hpp
	   @ONLY
	)
	
endmacro(create_toolkit_include)

macro(create_toolkit_src toolkit)
	set(toolkit ${toolkit})
	string(TOUPPER ${toolkit} toolkit_toupper)
	set(toolkit_src_template_path ${toolkits_template_path}/libs/toolkit/src)
	
	
	set(toolkit_lib_path ${toolkits_path}/libs/${toolkit})
	set(toolkit_src_path ${toolkit_lib_path}/src)
	configure_file(
	   ${toolkit_src_template_path}/stable_headers.hpp.in 
	   ${toolkit_src_path}/${toolkit}_stable_headers.hpp
	   @ONLY
	)


	#configure_file(
	#   ${toolkit_src_template_path}/precompiled_headers.cpp.in
	#   ${toolkit_src_path}/precompiled_headers.cpp
	#   @ONLY
	#)
	
endmacro(create_toolkit_src)



macro(create_toolkit_test toolkit)
	set(toolkit ${toolkit})
	string(TOUPPER ${toolkit} toolkit_toupper)
	set(toolkit_test_template_path ${toolkits_template_path}/libs/toolkit/test)

	set(toolkit_lib_path ${toolkits_path}/libs/${toolkit})
	set(toolkit_test_path ${toolkit_lib_path}/test)
	configure_file(
	   ${toolkit_test_template_path}/test.hpp.in 
	   ${toolkit_test_path}/test.hpp
	   @ONLY
	)
	
	configure_file(
	   ${toolkit_test_template_path}/test.cpp.in 
	   ${toolkit_test_path}/test.cpp
	   @ONLY
	)
	
	configure_file(
	   ${toolkit_test_template_path}/CMakeLists.txt.in
	   ${toolkit_test_path}/CMakeLists.txt
	   @ONLY
	)
	
	
		
	configure_file(
	   ${toolkit_test_template_path}/stable_headers.hpp.in
	   ${toolkit_test_path}/stable_headers.hpp
	   @ONLY
	)
	
	configure_file(
	   ${toolkit_test_template_path}/precompiled_headers.cpp.in
	   ${toolkit_test_path}/precompiled_headers.cpp
	   @ONLY
	)

endmacro(create_toolkit_test)

macro(create_toolkit_doc toolkit)
	message(STATUS "create_toolkit_doc" )
endmacro(create_toolkit_doc)

macro(create_toolkit_example toolkit)
	
	set(toolkit ${toolkit})
	string(TOUPPER ${toolkit} toolkit_toupper)
	set(toolkit_example_template_path ${toolkits_template_path}/libs/toolkit/example)

	set(toolkit_lib_path ${toolkits_path}/libs/${toolkit})
	set(toolkit_example_path ${toolkit_lib_path}/example)
	configure_file(
	   ${toolkit_example_template_path}/example.hpp.in 
	   ${toolkit_example_path}/example.hpp
	   @ONLY
	)
	
	configure_file(
	   ${toolkit_example_template_path}/example.cpp.in 
	   ${toolkit_example_path}/example.cpp
	   @ONLY
	)
	
	configure_file(
	   ${toolkit_example_template_path}/CMakeLists.txt.in
	   ${toolkit_example_path}/CMakeLists.txt
	   @ONLY
	)
	
	
		
	configure_file(
	   ${toolkit_example_template_path}/stable_headers.hpp.in
	   #${toolkit_example_path}/${toolkit}_stable_headers.hpp
	   ${toolkit_example_path}/stable_headers.hpp // nous: ?
	   @ONLY
	)
	
	configure_file(
	   ${toolkit_example_template_path}/precompiled_headers.cpp.in
	   ${toolkit_example_path}/precompiled_headers.cpp
	   @ONLY
	)
endmacro(create_toolkit_example)

#…˙≥…‘§±‡“ÎÕ∑
macro(generate_toolkits_files)
	
	if(NOT EXISTS ${generation_toolkits_files_path})
		file(MAKE_DIRECTORY ${generation_toolkits_files_path})
	endif()
	
	
	foreach(toolkit ${geek_toolkits_name})
	
		if(toolkit_${toolkit})
			set(include_toolkits_stable_headers ${include_toolkits_stable_headers}\#include\ <libs/${toolkit}/src/${toolkit}_stable_headers.hpp>\n)
		endif(toolkit_${toolkit})
	endforeach(toolkit)
	

	set(toolkits_template_path ${GEEK_CMAKE_MODULE_PATH}/templates/toolkits)
	
	configure_file(
	   ${toolkits_template_path}/stable_headers.hpp.in
	   ${generation_toolkits_files_path}/stable_headers.hpp
	   @ONLY
	)


	configure_file(
	   ${toolkits_template_path}/precompiled_headers.cpp.in
	   ${generation_toolkits_files_path}/precompiled_headers.cpp
	   @ONLY
	)
	
endmacro(generate_toolkits_files)


macro(geek_setup_toolkit_files toolkit)

	
	
	set(toolkit_include_path ${toolkits_path}/geek/toolkits/${toolkit})
	
	set(toolkit_lib_path ${toolkits_path}/libs/${toolkit})
	set(toolkit_src_path ${toolkit_lib_path}/src)
	set(toolkit_test_path ${toolkit_lib_path}/test)
	set(toolkit_doc_path ${toolkit_lib_path}/doc)
	set(toolkit_example_path ${toolkit_lib_path}/example)
	
	
	if(NOT EXISTS ${toolkit_include_path})
		file(MAKE_DIRECTORY ${toolkit_include_path})
		create_toolkit_include(${toolkit})
	endif()
	
	
	if(NOT EXISTS ${toolkit_src_path})
		file(MAKE_DIRECTORY ${toolkit_src_path})
		create_toolkit_src(${toolkit})
	endif()
	
	
	if(NOT EXISTS ${toolkit_test_path})
		file(MAKE_DIRECTORY ${toolkit_test_path})
		create_toolkit_test(${toolkit})
	endif()
	
	
	if(NOT EXISTS ${toolkit_doc_path})
		file(MAKE_DIRECTORY ${toolkit_doc_path})
		create_toolkit_doc(${toolkit})
	endif()
	
	if(NOT EXISTS ${toolkit_example_path})
		file(MAKE_DIRECTORY ${toolkit_example_path})
		create_toolkit_example(${toolkit})
	endif()
	
	
endmacro(geek_setup_toolkit_files)

macro(geek_setup_toolkits)
	
	list(APPEND geek_include_path ${toolkits_path})
	generate_toolkits_files()
	
	foreach(toolkit ${geek_toolkits_name})
		option(toolkit_${toolkit} "enable toolkit_${toolkit}" ON)
		geek_setup_toolkit_files(${toolkit})
	endforeach(toolkit)
endmacro(geek_setup_toolkits)



macro(geek_setup_toolkits_sub_add_files )
endmacro(geek_setup_toolkits_sub_add_files)

macro(geek_setup_toolkits_sub)

	foreach(toolkit ${geek_toolkits_name})
		
			
		if(toolkit_${toolkit})
			
			
			set(toolkit_include_path ${toolkits_path}/geek/toolkits/${toolkit})
			
			set(toolkit_lib_path ${toolkits_path}/libs/${toolkit})
			set(toolkit_src_path ${toolkit_lib_path}/src)
			
			set_project_files(libs\\\\\\\\${toolkit} ${toolkit_src_path} .cpp)
			set_project_files(libs\\\\\\\\${toolkit} ${toolkit_src_path} .hpp)
			
			
			set_project_files(include\\\\\\\\${toolkit} ${toolkit_include_path} .hpp)
			set_project_files(include ${toolkits_path}/geek/toolkits .hpp)
		endif(toolkit_${toolkit})
	endforeach(toolkit)
endmacro(geek_setup_toolkits_sub)

macro(geek_setup_toolkits_test)
	foreach(toolkit ${geek_toolkits_name})
		if(toolkit_${toolkit})
			set(toolkit_lib_path ${toolkits_path}/libs/${toolkit})
			set(toolkit_test_path ${toolkit_lib_path}/test)
			add_subdirectory(${toolkit_test_path})
		endif(toolkit_${toolkit})
	endforeach(toolkit)
	
endmacro(geek_setup_toolkits_test)


macro(geek_setup_toolkits_example)
	foreach(toolkit ${geek_toolkits_name})
		if(toolkit_${toolkit})
			set(toolkit_lib_path ${toolkits_path}/libs/${toolkit})
			set(toolkit_example_path ${toolkit_lib_path}/example)
			add_subdirectory(${toolkit_example_path})
		endif(toolkit_${toolkit})
	endforeach(toolkit)
	
endmacro(geek_setup_toolkits_example)



