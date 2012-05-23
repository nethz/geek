

sub_project_begin(${FRAMEWORK_NAME})

	
	
	include_directories(${toolkits_path} ${framework_path})
	if(build_static)
	else()
		add_definitions(-D_USRDLL -DGEEK_FRAMEWORK_SOURCE)
	endif(build_static)
	
	
	set_project_files(geek ${PROJECT_SOURCE_DIR}/geek .hpp)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR})
	
	set_project_files(libs ${CMAKE_CURRENT_SOURCE_DIR}/libs/src .hpp)
	set_project_files(libs ${CMAKE_CURRENT_SOURCE_DIR}/libs/src .cpp)
	set_project_files(include ${CMAKE_CURRENT_SOURCE_DIR}/geek/framework .hpp)
	
	if(precompiled_header)
		use_precompiled_header(${LIB_NAME}
		  "${CMAKE_CURRENT_SOURCE_DIR}/libs/src/stable_headers.hpp"
		  "${CMAKE_CURRENT_SOURCE_DIR}/libs/src/precompiled_headers.cpp"
		)
	endif(precompiled_header)

	if(build_static)
		add_library(${LIB_NAME} STATIC ${${LIB_NAME}_FILES})
	else(build_static)
		add_library(${LIB_NAME} SHARED ${${LIB_NAME}_FILES})
	endif(build_static)
	    
    target_link_libraries(
		${LIB_NAME} 
		${TOOLKITS_NAME}
	)

sub_project_end()