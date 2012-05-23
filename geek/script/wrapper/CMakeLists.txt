include(${SWIG_USE_FILE})

unset(CMAKE_DEBUG_POSTFIX)
include_directories(
${CMAKE_CURRENT_SOURCE_DIR}/../interface
${geek_include_path}
)

set(CMAKE_SWIG_FLAGS "")
set_source_files_properties(wrapper.i PROPERTIES CPLUSPLUS ON)
message(STATUS "Swig Lua Support Enabled...")

include_directories(${LUA_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
#SET(CMAKE_SWIG_FLAGS -DTCODLIB_API)
set(CMAKE_SWIG_OUTDIR ${CMAKE_BINARY_DIR}/generation/swig/lua)
SWIG_ADD_MODULE(geek_script_wrapper lua wrapper.i runme.lua)
SWIG_LINK_LIBRARIES(geek_script_wrapper  ${LUA_LIBRARIES} ${TOOLKITS_NAME})	

install(TARGETS ${LIB_NAME} geek_script_wrapper
	RUNTIME DESTINATION bin/debug
	CONFIGURATIONS Debug
	LIBRARY DESTINATION bin/debug
	CONFIGURATIONS Debug
	ARCHIVE DESTINATION lib/debug
	CONFIGURATIONS Debug
)

install(TARGETS ${LIB_NAME} geek_script_wrapper
	RUNTIME DESTINATION bin/release
	CONFIGURATIONS Release
	LIBRARY DESTINATION bin/release 
	CONFIGURATIONS Release
	ARCHIVE DESTINATION lib/release
	CONFIGURATIONS Release
)


set(CMAKE_DEBUG_POSTFIX "_d")