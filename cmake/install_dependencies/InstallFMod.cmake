#在以下路径中寻找 ogre
find_path(FMOD_DIR
  NAMES api/inc/fmod.h
  PATHS 
  ${CMAKE_CURRENT_SOURCE_DIR}
  NO_DEFAULT_PATH
)



if(FMOD_DIR)
	install(DIRECTORY ${FMOD_DIR}/api/inc/ DESTINATION include/FMod)
	
		
	install(FILES 
	${FMOD_DIR}/api/lib/fmodex_vc.lib
	DESTINATION vc80/lib)
	
	
	install(FILES 
	${FMOD_DIR}/api/lib/fmodex_vc.lib
	DESTINATION vc90/lib)
	
	
	install(FILES 
	${FMOD_DIR}/api/lib/fmodex_vc.lib
	DESTINATION vc100/lib)
	
	
	
	
	install(FILES 
	${FMOD_DIR}/api/fmodex.dll
	DESTINATION vc80/bin/debug)
	
	install(FILES 
	${FMOD_DIR}/api/fmodex.dll
	DESTINATION vc80/bin/release)
	
	
	
	install(FILES 
	${FMOD_DIR}/api/fmodex.dll
	DESTINATION vc90/bin/debug)
	
	install(FILES 
	${FMOD_DIR}/api/fmodex.dll
	DESTINATION vc90/bin/release)
	
	
	
	
	install(FILES 
	${FMOD_DIR}/api/fmodex.dll
	DESTINATION vc100/bin/debug)
	
	install(FILES 
	${FMOD_DIR}/api/fmodex.dll
	DESTINATION vc100/bin/release)
	
	
	
	
	
	#install(DIRECTORY ${OGRE_VC8_DIR}/include/OIS DESTINATION include)
	#install(DIRECTORY ${OGRE_VC8_DIR}/bin ${OGRE_VC8_DIR}/lib DESTINATION vc80 PATTERN "opt" EXCLUDE)
	
endif(FMOD_DIR)
