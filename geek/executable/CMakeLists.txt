sub_project_begin(geek_executable)

	
	include_directories(${toolkits_path} ${framework_path} ${view_ogre_path} ${view_ois_path} ${view_cegui_path})
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
    target_link_libraries(
		${LIB_NAME}  
		${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
		${TOOLKITS_NAME}
		${FRAMEWORK_NAME}
		${VIEW_OGRE_NAME}
		${VIEW_OIS_NAME}
		${VIEW_CEGUI_NAME}
		
		debug OgreMain_d 
		optimized OgreMain
	)
    
sub_project_end()
