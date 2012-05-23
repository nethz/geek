

sub_project_begin(${TOOLKITS_NAME})
	if(build_static)
	else(build_static)
		add_definitions(-D_USRDLL -DGEEK_TOOLKITS_SOURCE)
	endif(build_static)
	set_project_files(geek ${PROJECT_SOURCE_DIR}/geek .hpp)
	include_directories(${generation_toolkits_files_path})
	include_directories(${CMAKE_CURRENT_SOURCE_DIR})
	set_project_files(generation ${generation_toolkits_files_path} .hpp)
	set_project_files(generation ${generation_toolkits_files_path} .cpp)
	geek_setup_toolkits_sub()
	
	if(toolkit_platform)
		set_project_files(include\\\\\\\\platform\\\\\\\\win32 ${PROJECT_SOURCE_DIR}/toolkits/geek/toolkits/platform/win32 .hpp)
		set_project_files(include\\\\\\\\platform\\\\\\\\glx ${PROJECT_SOURCE_DIR}/toolkits/geek/toolkits/platform/glx .hpp)
		set_project_files(include\\\\\\\\platform\\\\\\\\gtk ${PROJECT_SOURCE_DIR}/toolkits/geek/toolkits/platform/gtk .hpp)
		set_project_files(include\\\\\\\\platform\\\\\\\\iphone ${PROJECT_SOURCE_DIR}/toolkits/geek/toolkits/platform/iphone .hpp)
		set_project_files(include\\\\\\\\platform\\\\\\\\osx ${PROJECT_SOURCE_DIR}/toolkits/geek/toolkits/platform/osx .hpp)
		set_project_files(include\\\\\\\\platform\\\\\\\\symbian ${PROJECT_SOURCE_DIR}/toolkits/geek/toolkits/platform/symbian .hpp)

		set_project_files(libs\\\\\\\\platform\\\\\\\\win32 ${PROJECT_SOURCE_DIR}/toolkits/libs/platform/src/win32 .cpp)
		set_project_files(libs\\\\\\\\platform\\\\\\\\glx ${PROJECT_SOURCE_DIR}/toolkits/libs/platform/src/glx .cpp)
		set_project_files(libs\\\\\\\\platform\\\\\\\\gtk ${PROJECT_SOURCE_DIR}/toolkits/libs/platform/src/gtk .cpp)
		set_project_files(libs\\\\\\\\platform\\\\\\\\iphone ${PROJECT_SOURCE_DIR}/toolkits/libs/platform/src/iphone .cpp)
		set_project_files(libs\\\\\\\\platform\\\\\\\\osx ${PROJECT_SOURCE_DIR}/toolkits/libs/platform/src/osx .cpp)
		set_project_files(libs\\\\\\\\platform\\\\\\\\symbian ${PROJECT_SOURCE_DIR}/toolkits/libs/platform/src/symbian .cpp)
	endif(toolkit_platform)
	if(build_static)
		add_library(${LIB_NAME} STATIC ${${LIB_NAME}_FILES})
	else(build_static)
	message(900)
		add_library(${LIB_NAME} SHARED ${${LIB_NAME}_FILES})
	endif(build_static)
	    
    target_link_libraries(
		${LIB_NAME} 
		${Boost_SYSTEM_LIBRARY}
		${Boost_THREAD_LIBRARY}
		${Boost_LOG_LIBRARY}
		${Boost_LOG_SETUP_LIBRARY}
		${Boost_CONTEXT_LIBRARY}
		${Boost_CHRONO_LIBRARY}
	)
	if(APPLE)
		 target_link_libraries(
			${LIB_NAME} 
			${Cocoa_LIBRARIES} 
			${Carbon_LIBRARIES}
		)
	endif(APPLE)
sub_project_end()