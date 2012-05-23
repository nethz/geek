sub_project_begin(game_made_in_geeks)

	
	include_directories(${geek_include_path})
	set_project_files(include ${CMAKE_CURRENT_SOURCE_DIR} .hpp)
	set_project_files(src ${CMAKE_CURRENT_SOURCE_DIR} .cpp)
	if(enable_win32)
	
		add_definitions(-D_ENABLE_WIN32)
		add_executable(${EXE_NAME} WIN32 ${${EXE_NAME}_FILES})
	else()
		add_executable(${EXE_NAME} ${${EXE_NAME}_FILES})
	endif(enable_win32)
	if(precompiled_header)
	    use_precompiled_header(${LIB_NAME}
	      "${CMAKE_CURRENT_SOURCE_DIR}/stable_headers.hpp"
	      "${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers.cpp"
	    )
    endif(precompiled_header)
    TARGET_LINK_LIBRARIES(
		${LIB_NAME}  
		${geek_librarys}
		
	)
    
sub_project_end()
