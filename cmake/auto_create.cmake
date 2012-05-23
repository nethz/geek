
function(get_function in n result_var)
	set ( ret_n ${n})
	math( EXPR name_n "${n}+1" ) 
	math( EXPR parameters_n "${n}+2" ) 
	list(GET in ${ret_n} ret)
	list(GET in ${name_n} name)
	list(GET in ${parameters_n} parameters)
	set(${result_var} "boost::function<${ret}(${parameters})> ${name}" PARENT_SCOPE)
endfunction(get_function)

function(get_component_hpp_function in n interface result_var)
	set ( ret_n ${n})
	math( EXPR name_n "${n}+1" ) 
	math( EXPR parameters_n "${n}+2" ) 
	list(GET in ${ret_n} ret)
	list(GET in ${name_n} name)
	list(GET in ${parameters_n} parameters)
	set(${result_var} "${ret} ${interface}_${name}(${parameters});" PARENT_SCOPE)
endfunction(get_component_hpp_function)

function(get_component_cpp_function component in n interface result_var)
	set ( ret_n ${n})
	math( EXPR name_n "${n}+1" ) 
	math( EXPR parameters_n "${n}+2" ) 
	list(GET in ${ret_n} ret)
	list(GET in ${name_n} name)
	list(GET in ${parameters_n} parameters)
	
	set(${result_var} "${ret} ${component}_component::${interface}_${name}(${parameters}){/*add return value;*/}" PARENT_SCOPE)
endfunction(get_component_cpp_function)


function(get_component_cpp_bind component in n interface result_var)
	set ( ret_n ${n})
	math( EXPR name_n "${n}+1" ) 
	math( EXPR parameters_n "${n}+2" ) 
	list(GET in ${ret_n} ret)
	list(GET in ${name_n} name)
	list(GET in ${parameters_n} parameters)
	set(${result_var} "${interface}_interface_->${name} = boost::bind(&${component}_component::${interface}_${name}, this /*, _1, _2, _3, _4 ~ */);" PARENT_SCOPE)
	#message("m_${interface}_interface->${name} = boost::bind(&${component}_component::${interface}_${name}, this);")
endfunction(get_component_cpp_bind)


function(auto_create_component_split_parameters group interface)
#	message(${group}, ${interface}) 
	
	list(LENGTH ARGN argn_length)
	math( EXPR argn_range "(${argn_length}-3)  " ) 
	if(${argn_range} GREATER -1)
		set(functions "")
		foreach(loop_var RANGE 0 ${argn_range} 3)
			get_function("${ARGN}" ${loop_var} result)
			set(src ${src} ${result} "." )
		endforeach()
	endif()
	
	
endfunction(auto_create_component_split_parameters)



function( auto_create_component_hpp_include result_var group interface)
	#message("#include \"${group}/${interface}_interface.hpp\"")
	set(${result_var} "#include <${group}/${interface}_interface.hpp>" PARENT_SCOPE)
endfunction(auto_create_component_hpp_include)


function( auto_create_component_hpp_interfaces result_var group interface)
	#message("boost::scoped_ptr<${interface}_interface> m_${interface}_interface;")
	set(${result_var} "boost::scoped_ptr<${interface}_interface> ${interface}_interface_;" PARENT_SCOPE)
endfunction(auto_create_component_hpp_interfaces)

function( auto_create_component_cpp_interfaces result_var group interface)
	#message("m_${interface}_interface(new ${interface}_interface())")
	set(${result_var} "${interface}_interface_(new ${interface}_interface())" PARENT_SCOPE)
endfunction(auto_create_component_cpp_interfaces)



function( auto_create_component_cpp_add result_var group interface)
	#message("_query.add(m_${interface}_interface.get());")
	set(${result_var} "query_.add(${interface}_interface_.get());" PARENT_SCOPE)
endfunction(auto_create_component_cpp_add)

function(auto_create_component_hpp_functions result_var group interface)
	
	list(LENGTH ARGN argn_length)
	math( EXPR argn_range "(${argn_length}-3)  " ) 
	if(${argn_range} GREATER -1)
		
		foreach(loop_var RANGE 0 ${argn_range} 3)
			get_component_hpp_function("${ARGN}" ${loop_var} ${interface} result)
			set(src ${src} ${result} "." )
		endforeach()
	endif()
	
	set(${result_var} ${src} PARENT_SCOPE)

endfunction(auto_create_component_hpp_functions)

function(auto_create_component_cpp_bind result_var component group interface)

	list(LENGTH ARGN argn_length)
	math( EXPR argn_range "(${argn_length}-3)  " ) 
	if(${argn_range} GREATER -1)
		
		foreach(loop_var RANGE 0 ${argn_range} 3)
			get_component_cpp_bind(${component} "${ARGN}"  ${loop_var} ${interface} result)
			set(src ${src} ${result} "." )
		endforeach()
	endif()
	
	set(${result_var} ${src} PARENT_SCOPE)

endfunction(auto_create_component_cpp_bind)


function(auto_create_component_cpp_functions result_var component group interface)

	list(LENGTH ARGN argn_length)
	math( EXPR argn_range "(${argn_length}-3)  " ) 
	if(${argn_range} GREATER -1)
		
		foreach(loop_var RANGE 0 ${argn_range} 3)
			get_component_cpp_function(${component} "${ARGN}" ${loop_var} ${interface} result)
			set(src ${src} ${result} "." )
		endforeach()
	endif()
	
	set(${result_var} ${src} PARENT_SCOPE)

endfunction(auto_create_component_cpp_functions)


#自动生成接口
function(auto_create_interface ground name)
	unset(functions)
	string(TOUPPER ${name} name_toupper)
	list(LENGTH ARGN argn_length)
	math( EXPR argn_range "(${argn_length}-3)  " ) 
	if(${argn_range} GREATER -1)
		set(functions "")
		foreach(loop_var RANGE 0 ${argn_range} 3)
			get_function("${ARGN}" ${loop_var} result)
			set(src ${src} ${result} "." )
		endforeach()
	endif()
	
	string(REPLACE "." ";\n" functions ${src})
	
	set( INTERFACE_DIR "${PROJECT_SOURCE_DIR}/interface/${ground}" )
	set( INTERFACE_FILE "${PROJECT_SOURCE_DIR}/interface/${ground}/${name}_interface.hpp" )
	if(NOT EXISTS ${INTERFACE_DIR}) 
		file(MAKE_DIRECTORY ${INTERFACE_DIR})
	endif(NOT EXISTS ${INTERFACE_DIR})
	
	if(NOT EXISTS ${INTERFACE_FILE}) 
	    configure_file(
	       ${GEEK_CMAKE_MODULE_PATH}/templates/component/src/interface.hpp.in
	       ${INTERFACE_FILE}
	       @ONLY
	    )
	endif(NOT EXISTS ${INTERFACE_FILE})
	 
endfunction(auto_create_interface)


function(auto_create_component name)

#-------------------------
	foreach(var_loop ${ARGN})
		list(APPEND test ${var_loop})
		if( "INTERFACE" STREQUAL ${var_loop})
			
			list(LENGTH test test_length)
			if(test_length GREATER 1)
				auto_create_component_hpp_include(result ${test})
				set(COMPONENT_HPP_INCLUDE ${COMPONENT_HPP_INCLUDE} ${result} "=" )
				
				auto_create_component_hpp_interfaces(result ${test})
				set(COMPONENT_HPP_INTERFACES ${COMPONENT_HPP_INTERFACES} ${result} "=" )
				
				
				auto_create_component_cpp_interfaces(result ${test})
				set(COMPONENT_CPP_INTERFACES ${COMPONENT_CPP_INTERFACES} ${result} "=" )
				
				auto_create_component_hpp_functions(result ${test})
				set(COMPONENT_HPP_FUNCTIONS ${COMPONENT_HPP_FUNCTIONS} ${result}  )
				
				auto_create_component_cpp_bind(result ${name} ${test})
				set(COMPONENT_CPP_BIND ${COMPONENT_CPP_BIND} ${result})
						
				auto_create_component_cpp_functions(result ${name} ${test})
				set(COMPONENT_CPP_FUNCTIONS	${COMPONENT_CPP_FUNCTIONS} ${result})
				
				auto_create_component_cpp_add(result ${test})
				set(COMPONENT_CPP_ADD ${COMPONENT_CPP_ADD} ${result} "=" )
				
			endif()
			unset(test)
		endif()
	endforeach()
	
	
	list(LENGTH test test_length)
	
	if(test_length GREATER 1)
		auto_create_component_hpp_include(result ${test})
		set(COMPONENT_HPP_INCLUDE ${COMPONENT_HPP_INCLUDE} ${result} "=" )
		
		auto_create_component_hpp_interfaces(result ${test})
		set(COMPONENT_HPP_INTERFACES ${COMPONENT_HPP_INTERFACES} ${result} "=" )
		
		auto_create_component_cpp_interfaces(result ${test})
		set(COMPONENT_CPP_INTERFACES ${COMPONENT_CPP_INTERFACES} ${result})
		
		auto_create_component_hpp_functions(result ${test})
		set(COMPONENT_HPP_FUNCTIONS ${COMPONENT_HPP_FUNCTIONS} ${result})
		
		
		auto_create_component_cpp_bind(result ${name} ${test})
		set(COMPONENT_CPP_BIND ${COMPONENT_CPP_BIND} ${result})
		
		auto_create_component_cpp_functions(result ${name} ${test})
		set(COMPONENT_CPP_FUNCTIONS	${COMPONENT_CPP_FUNCTIONS} ${result})
		
		auto_create_component_cpp_add(result ${test})
		set(COMPONENT_CPP_ADD ${COMPONENT_CPP_ADD} ${result} "=" )
		#auto_create_component_split_parameters(${test})
			
		string(REPLACE "=" "," COMPONENT_CPP_INTERFACES ${COMPONENT_CPP_INTERFACES})
		string(REPLACE "=" "\n" COMPONENT_HPP_INCLUDE ${COMPONENT_HPP_INCLUDE})
		string(REPLACE "=" ";\n" COMPONENT_HPP_INTERFACES ${COMPONENT_HPP_INTERFACES})
		string(REPLACE "." ";\n" COMPONENT_HPP_FUNCTIONS ${COMPONENT_HPP_FUNCTIONS})
		string(REPLACE "." ";\n" COMPONENT_CPP_BIND ${COMPONENT_CPP_BIND})
		string(REPLACE "." "\n" COMPONENT_CPP_FUNCTIONS ${COMPONENT_CPP_FUNCTIONS})
		string(REPLACE "=" ";\n" COMPONENT_CPP_ADD ${COMPONENT_CPP_ADD})
		
	endif()
	
#-------------------------
	
	set( component_name ${name})
	string(TOUPPER ${component_name} component_name_toupper)
	string(TOUPPER ${name} name_toupper)
	
	set( COMPONENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" )
	set( COMPONENT_HPP "${COMPONENT_DIR}/${component_name}_component.hpp" )
	set( COMPONENT_CPP "${COMPONENT_DIR}/${component_name}_component.cpp" )
	
	if(NOT EXISTS ${COMPONENT_DIR}) 
		file(MAKE_DIRECTORY ${COMPONENT_DIR})
	endif(NOT EXISTS ${COMPONENT_DIR})
	
	if(NOT EXISTS ${COMPONENT_HPP}) 
	    configure_file(
	       ${GEEK_CMAKE_MODULE_PATH}/templates/component/src/component.hpp.in
	       ${COMPONENT_HPP}
	       @ONLY
	    )
	endif(NOT EXISTS ${COMPONENT_HPP})
	
	if(NOT EXISTS ${COMPONENT_CPP}) 
	    configure_file(
	       ${GEEK_CMAKE_MODULE_PATH}/templates/component/src/component.cpp.in
	       ${COMPONENT_CPP}
	       @ONLY
	    )
	endif(NOT EXISTS ${COMPONENT_CPP})
	
endfunction(auto_create_component)

function(auto_create_component_file dir file name)
	set(NAME ${name})
	string(TOUPPER ${name} name_toupper)
	set( CREATE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${dir}" )
	set( CREATE_FILE "${CREATE_DIR}/${file}" )
	
	if(NOT EXISTS ${CREATE_DIR}) 
		file(MAKE_DIRECTORY ${CREATE_DIR})
	endif(NOT EXISTS ${CREATE_DIR})
	
	if(NOT EXISTS ${CREATE_FILE}) 
	    configure_file(
	       ${GEEK_CMAKE_MODULE_PATH}/templates/component/${dir}/${file}.in
	       ${CREATE_FILE}
	       @ONLY
	    )
	endif(NOT EXISTS ${CREATE_FILE})
	
endfunction(auto_create_component_file)


function(auto_create_component_main name)
	foreach(loop_var ${ARGN})
		set(MAIN_CPP_INCLUDE "${MAIN_CPP_INCLUDE}\n#include \"${loop_var}_component.hpp\"")
	endforeach()
	
	foreach(loop_var ${ARGN})
		set(MAIN_CPP_ADD "${MAIN_CPP_ADD}.add_factory(\"${loop_var}\", boost::factory<${loop_var}_component *>())\n")
	endforeach()	
	
	foreach(loop_var ${ARGN})
		set(MAIN_CPP_REMOVE "${MAIN_CPP_REMOVE}.remove_factory(\"${loop_var}\")\n")
	endforeach()
	auto_create_component_file("src" "main.cpp" ${name})
endfunction(auto_create_component_main)

function(auto_create_component_test_file component)

	set(name ${component})
	string(TOUPPER ${name} name_toupper)
	set(COMPONENT ${component})
	set( CREATE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test/src" )
	set( CREATE_FILE "${CREATE_DIR}/${component}_component_test.hpp" )
	
	if(NOT EXISTS ${CREATE_DIR}) 
		file(MAKE_DIRECTORY ${CREATE_DIR})
	endif(NOT EXISTS ${CREATE_DIR})
	
	if(NOT EXISTS ${CREATE_FILE}) 
	    configure_file(
	        ${GEEK_CMAKE_MODULE_PATH}/templates/component/test/src/unit_test.hpp.in
	       ${CREATE_FILE}
	       @ONLY
	    )
	endif(NOT EXISTS ${CREATE_FILE})
endfunction(auto_create_component_test_file)

function(auto_create_component_test name)
	foreach(loop_var ${ARGN})
		
		auto_create_component_test_file(${loop_var})
		
		set(TEST_CPP_INCLUDE "${TEST_CPP_INCLUDE}#include \"${loop_var}_component_test.hpp\"\n")
	endforeach()
	message(${name})
	set(TEST_HPP_LOADPLUGIN "${TEST_HPP_LOADPLUGIN}geek::plugins_manager::get_singleton().load_plugin(\"${name}_component\");\n")
	auto_create_component_file("test\\src" "test.cpp" ${name})
	auto_create_component_file("test\\src" "stable_headers.hpp" ${name})
	auto_create_component_file("test\\src" "test.hpp" ${name})
	auto_create_component_file("test\\src" "precompiled_headers.cpp" ${name})
	auto_create_component_file("test\\src" "test_running.hpp" ${name})
	auto_create_component_file("test" "CMakeLists.txt" ${name})
	
endfunction(auto_create_component_test)


function(auto_create_component_project name)
	auto_create_component_main(name ${ARGN})

	auto_create_component_file("src" "config.hpp" ${name})
	auto_create_component_file("src" "exports.def" ${name})
	auto_create_component_file("src" "precompiled_headers.cpp" ${name})
	auto_create_component_file("src" "stable_headers.hpp" ${name})
	
	
	auto_create_component_test(${name} ${ARGN})
	
	
endfunction(auto_create_component_project)
