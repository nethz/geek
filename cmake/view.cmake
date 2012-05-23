set(geek_view_name "ogre" "ois" "cegui" "fmod")
set(view_template_path ${GEEK_CMAKE_MODULE_PATH}/templates/view)
macro(create_view view)
	set(toolkits_path ${PROJECT_SOURCE_DIR}/toolkits)
	
	set(view_${view}_path ${PROJECT_SOURCE_DIR}/view_${view})
	set(view ${view})
	string(TOUPPER ${view} view_toupper)
	configure_file(
	   ${view_template_path}/CMakeLists.txt.in
	   ${view_${view}_path}/CMakeLists.txt
	   @ONLY
	)
	
	
	configure_file(
	   ${view_template_path}/libs/src/precompiled_headers.cpp.in
	   ${view_${view}_path}/libs/src/precompiled_headers.cpp
	   @ONLY
	)
	configure_file(
	   ${view_template_path}/libs/src/stable_headers.hpp.in
	   ${view_${view}_path}/libs/src/stable_headers.hpp
	   @ONLY
	)
	
	
	
	configure_file(
	   ${view_template_path}/geek/view/config.hpp.in
	   ${view_${view}_path}/geek/view_${view}/config.hpp
	   @ONLY
	)
endmacro(create_view)
macro(geek_setup_view)
	
	
	foreach(view ${geek_view_name})
		string(TOUPPER ${view} view_toupper)
		if(build_static)
			set(VIEW_${view_toupper}_NAME "libgeek_view_${view}")
		else(build_static)
			set(VIEW_${view_toupper}_NAME "geek_view_${view}")
		endif(build_static)
		
		list(APPEND geek_librarys ${VIEW_${view_toupper}_NAME})
		set(view_${view}_path ${PROJECT_SOURCE_DIR}/view_${view})
		list(APPEND geek_include_path ${view_${view}_path})
		if(NOT EXISTS ${view_${view}_path})
			create_view(${view})
		endif()
		add_subdirectory(${view_${view}_path})
	endforeach(view)
endmacro(geek_setup_view)