
option(enable_script "enable script" OFF)
if(enable_script)
	if(build_static)
		set(VIEW_LUA_NAME "libgeek_view_lua")
	else(build_static)
		set(VIEW_LUA_NAME "geek_view_lua")
	endif(build_static)
	list(APPEND geek_librarys ${VIEW_LUA_NAME})
	add_definitions(-D_ENABLE_SCRIPT)
endif(enable_script)


macro(geek_setup_script)
	if(enable_script)
		find_package(Lua51 REQUIRED)
		find_package(SWIG REQUIRED)
		
		set(view_lua_path ${PROJECT_SOURCE_DIR}/view_lua)
		list(APPEND geek_include_path ${view_lua_path})
		add_subdirectory(${view_lua_path})
		
		add_subdirectory(${PROJECT_SOURCE_DIR}/script/component)
		add_subdirectory(${PROJECT_SOURCE_DIR}/script/wrapper)
		add_subdirectory(${PROJECT_SOURCE_DIR}/script/executable)
	endif(enable_script)
endmacro(geek_setup_script)

