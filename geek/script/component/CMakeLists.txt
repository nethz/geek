sub_project_begin(geek_script_component)

	include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/../interface
	${geek_include_path}
	)
	
	
	set_project_files(interface ${CMAKE_CURRENT_SOURCE_DIR}/../interface .hpp)
	set_project_files(include ${CMAKE_CURRENT_SOURCE_DIR} .hpp)
	set_project_files(src ${CMAKE_CURRENT_SOURCE_DIR} .cpp)
	set_project_files(def ${CMAKE_CURRENT_SOURCE_DIR} .def)
	add_library(${LIB_NAME} SHARED ${${LIB_NAME}_FILES})

	 if(precompiled_header)
	    use_precompiled_header(${LIB_NAME}
	      "${CMAKE_CURRENT_SOURCE_DIR}/stable_headers.hpp"
	      "${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers.cpp"
	    )
    endif(precompiled_header)
    
    target_link_libraries(
		${LIB_NAME} 
		${geek_librarys}
		debug OgreMain_d 
		optimized OgreMain 
	)
sub_project_end()
