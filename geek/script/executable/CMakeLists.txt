sub_project_begin(geek_script_executable)

	message(${geek_include_path})
	include_directories(${geek_include_path})
	set_project_files(include ${CMAKE_CURRENT_SOURCE_DIR} .hpp)
	set_project_files(src ${CMAKE_CURRENT_SOURCE_DIR} .cpp)
	set_project_files(lua ${CMAKE_CURRENT_SOURCE_DIR}/lua .lua)
	
	
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
  
    target_link_libraries(
		${LIB_NAME}  
		${geek_librarys}
	)
    
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/lua/runme.lua DESTINATION bin/release CONFIGURATIONS Release PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE)
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/lua/runme.lua DESTINATION bin/debug CONFIGURATIONS Debug PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE)
		
	
sub_project_end()
