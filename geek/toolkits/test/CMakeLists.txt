sub_project_begin(toolkits_test)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR})
	geek_setup_toolkits_test_sub()
	
	add_library(${LIB_NAME} SHARED ${${LIB_NAME}_FILES})
	
sub_project_end()