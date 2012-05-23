sub_project_begin(test_pool)
	include_directories(${toolkits_path})
	set_project_files(include ${CMAKE_CURRENT_SOURCE_DIR} .hpp)
	set_project_files(src ${CMAKE_CURRENT_SOURCE_DIR} .cpp)
	
	add_executable(${EXE_NAME} ${${EXE_NAME}_FILES})
	
	 if(precompiled_header)
	    use_precompiled_header(${LIB_NAME}
	      "${CMAKE_CURRENT_SOURCE_DIR}/stable_headers.hpp"
	      "${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers.cpp"
	    )
    endif(precompiled_header)
    
    TARGET_LINK_LIBRARIES(
		${LIB_NAME} 
		${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
		${TOOLKITS_NAME}
	)
    
sub_project_end()
