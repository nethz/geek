#在以下路径中寻找 ogre
find_path(CEGUI_VC8_DIR
  NAMES lib/CEGUIBase.lib
  PATHS 
  ${CMAKE_CURRENT_SOURCE_DIR}
  NO_DEFAULT_PATH
)


find_path(CEGUI_VC9_DIR
  NAMES lib/CEGUIBase.lib
  PATHS 
  ${CMAKE_CURRENT_SOURCE_DIR}
  NO_DEFAULT_PATH
)


find_path(CEGUI_VC10_DIR
  NAMES lib/CEGUIBase.lib
  PATHS 
  ${CMAKE_CURRENT_SOURCE_DIR}
  NO_DEFAULT_PATH
)

if(CEGUI_VC8_DIR)
	install(DIRECTORY ${CEGUI_VC8_DIR}/cegui/include/  DESTINATION include/cegui PATTERN "opt" EXCLUDE)
	install(FILES 
	${CEGUI_VC8_DIR}/lib/CEGUIBase.lib
	${CEGUI_VC8_DIR}/lib/CEGUILuaScriptModule.lib
	${CEGUI_VC8_DIR}/lib/CEGUIOgreRenderer.lib
	${CEGUI_VC8_DIR}/lib/tolua++.lib
	${CEGUI_VC8_DIR}/dependencies/lib/dynamic/lua.lib
	DESTINATION vc80/lib/release)
	
	install(FILES 
	${CEGUI_VC8_DIR}/lib/CEGUIBase_d.lib
	${CEGUI_VC8_DIR}/lib/CEGUILuaScriptModule_d.lib
	${CEGUI_VC8_DIR}/lib/CEGUIOgreRenderer_d.lib
	${CEGUI_VC8_DIR}/lib/tolua++_d.lib
	${CEGUI_VC8_DIR}/dependencies/lib/dynamic/lua_d.lib
	DESTINATION vc80/lib/debug)
	##########################################
	
	install(FILES 
	${CEGUI_VC8_DIR}/bin/CEGUIBase.dll
	${CEGUI_VC8_DIR}/bin/CEGUILuaScriptModule.dll
	${CEGUI_VC8_DIR}/bin/CEGUIOgreRenderer.dll
	${CEGUI_VC8_DIR}/bin/tolua++.dll
	${CEGUI_VC8_DIR}/dependencies/bin/lua.dll
	${CEGUI_VC8_DIR}/bin/CEGUIExpatParser.dll
	${CEGUI_VC8_DIR}/bin/CEGUIFalagardWRBase.dll
	DESTINATION vc80/bin/release)
	
	
	install(FILES 
	${CEGUI_VC8_DIR}/bin/CEGUIBase_d.dll
	${CEGUI_VC8_DIR}/bin/CEGUILuaScriptModule_d.dll
	${CEGUI_VC8_DIR}/bin/CEGUIOgreRenderer_d.dll
	${CEGUI_VC8_DIR}/bin/tolua++_d.dll
	${CEGUI_VC8_DIR}/dependencies/bin/lua_d.dll
	${CEGUI_VC8_DIR}/bin/CEGUIExpatParser_d.dll
	${CEGUI_VC8_DIR}/bin/CEGUIFalagardWRBase_d.dll
	DESTINATION vc80/bin/debug)
	
	
	#install(DIRECTORY ${OGRE_VC8_DIR}/bin ${OGRE_VC8_DIR}/lib DESTINATION vc80 PATTERN "opt" EXCLUDE)
	
endif(CEGUI_VC8_DIR)

if(CEGUI_VC9_DIR)
	message(${CEGUI_VC9_DIR})
	install(FILES 
	${CEGUI_VC9_DIR}/lib/CEGUIBase.lib
	${CEGUI_VC9_DIR}/lib/CEGUILuaScriptModule.lib
	${CEGUI_VC9_DIR}/lib/CEGUIOgreRenderer.lib
	${CEGUI_VC9_DIR}/lib/tolua++.lib
	${CEGUI_VC9_DIR}/dependencies/lib/dynamic/lua.lib
	DESTINATION vc90/lib/release)
	
	install(FILES 
	${CEGUI_VC9_DIR}/lib/CEGUIBase_d.lib
	${CEGUI_VC9_DIR}/lib/CEGUILuaScriptModule_d.lib
	${CEGUI_VC9_DIR}/lib/CEGUIOgreRenderer_d.lib
	${CEGUI_VC9_DIR}/lib/tolua++_d.lib
	${CEGUI_VC9_DIR}/dependencies/lib/dynamic/lua_d.lib
	DESTINATION vc90/lib/debug)
	
	install(FILES 
	${CEGUI_VC9_DIR}/bin/CEGUIBase.dll
	${CEGUI_VC9_DIR}/bin/CEGUILuaScriptModule.dll
	${CEGUI_VC9_DIR}/bin/CEGUIOgreRenderer.dll
	${CEGUI_VC9_DIR}/bin/tolua++.dll
	${CEGUI_VC9_DIR}/dependencies/bin/lua.dll
	${CEGUI_VC9_DIR}/bin/CEGUIExpatParser.dll
	${CEGUI_VC9_DIR}/bin/CEGUIFalagardWRBase.dll
	DESTINATION vc90/bin/release)
	
	
	install(FILES 
	${CEGUI_VC9_DIR}/bin/CEGUIBase_d.dll
	${CEGUI_VC9_DIR}/bin/CEGUILuaScriptModule_d.dll
	${CEGUI_VC9_DIR}/bin/CEGUIOgreRenderer_d.dll
	${CEGUI_VC9_DIR}/bin/tolua++_d.dll
	${CEGUI_VC9_DIR}/dependencies/bin/lua_d.dll
	${CEGUI_VC9_DIR}/bin/CEGUIExpatParser_d.dll
	${CEGUI_VC9_DIR}/bin/CEGUIFalagardWRBase_d.dll
	DESTINATION vc90/bin/debug)
	
endif(CEGUI_VC9_DIR)

if(CEGUI_VC10_DIR)
	message(${CEGUI_VC10_DIR})	
	
	install(DIRECTORY ${CEGUI_VC10_DIR}/datafiles DESTINATION Media)
	install(FILES 
	${CEGUI_VC10_DIR}/lib/CEGUIBase.lib
	${CEGUI_VC10_DIR}/lib/CEGUILuaScriptModule.lib
	${CEGUI_VC10_DIR}/lib/CEGUIOgreRenderer.lib
	${CEGUI_VC10_DIR}/lib/tolua++.lib
	${CEGUI_VC10_DIR}/dependencies/lib/dynamic/lua.lib
	DESTINATION vc100/lib/release)
	
	install(FILES 
	${CEGUI_VC10_DIR}/lib/CEGUIBase_d.lib
	${CEGUI_VC10_DIR}/lib/CEGUILuaScriptModule_d.lib
	${CEGUI_VC10_DIR}/lib/CEGUIOgreRenderer_d.lib
	${CEGUI_VC10_DIR}/lib/tolua++_d.lib
	${CEGUI_VC10_DIR}/dependencies/lib/dynamic/lua_d.lib
	DESTINATION vc100/lib/debug)
	
	install(FILES 
	${CEGUI_VC10_DIR}/bin/CEGUIBase.dll
	${CEGUI_VC10_DIR}/bin/CEGUILuaScriptModule.dll
	${CEGUI_VC10_DIR}/bin/CEGUIOgreRenderer.dll
	${CEGUI_VC10_DIR}/bin/tolua++.dll
	${CEGUI_VC10_DIR}/dependencies/bin/lua.dll
	${CEGUI_VC10_DIR}/bin/CEGUIExpatParser.dll
	${CEGUI_VC10_DIR}/bin/CEGUIFalagardWRBase.dll
	DESTINATION vc100/bin/release)
	
	
	install(FILES 
	${CEGUI_VC10_DIR}/bin/CEGUIBase_d.dll
	${CEGUI_VC10_DIR}/bin/CEGUILuaScriptModule_d.dll
	${CEGUI_VC10_DIR}/bin/CEGUIOgreRenderer_d.dll
	${CEGUI_VC10_DIR}/bin/tolua++_d.dll
	${CEGUI_VC10_DIR}/dependencies/bin/lua_d.dll
	${CEGUI_VC10_DIR}/bin/CEGUIExpatParser_d.dll
	${CEGUI_VC10_DIR}/bin/CEGUIFalagardWRBase_d.dll
	DESTINATION vc100/bin/debug)
	#install(DIRECTORY ${OGRE_VC10_DIR}/bin DESTINATION vc100)
	#install(DIRECTORY ${OGRE_VC10_DIR}/lib DESTINATION vc100 PATTERN "opt" EXCLUDE )
endif(CEGUI_VC10_DIR)