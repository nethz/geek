#在以下路径中寻找 ogre
find_path(OGRE_VC8_DIR
  NAMES include/OGRE/OgreAnimation.h
  PATHS 
  ${CMAKE_CURRENT_SOURCE_DIR}
  NO_DEFAULT_PATH
)


find_path(OGRE_VC9_DIR
  NAMES include/OGRE/OgreAnimation.h
  PATHS 
  ${CMAKE_CURRENT_SOURCE_DIR}
  NO_DEFAULT_PATH
)


find_path(OGRE_VC10_DIR
  NAMES include/OGRE/OgreAnimation.h
  PATHS 
  ${CMAKE_CURRENT_SOURCE_DIR}
  NO_DEFAULT_PATH
)

find_path(OGRE_SRC_DIR
  NAMES include/OGRE/OgreAnimation.h
  PATHS 
  ${CMAKE_CURRENT_SOURCE_DIR}
  NO_DEFAULT_PATH
)
if(OGRE_SRC_DIR)
	install(DIRECTORY ${OGRE_SRC_DIR}/Samples/Media DESTINATION .)
endif(OGRE_SRC_DIR)

if(OGRE_VC8_DIR)
	install(DIRECTORY ${OGRE_VC8_DIR}/include/OGRE DESTINATION include)
	install(DIRECTORY ${OGRE_VC8_DIR}/include/OIS DESTINATION include)
	install(DIRECTORY ${OGRE_VC8_DIR}/bin ${OGRE_VC8_DIR}/lib DESTINATION vc80 PATTERN "opt" EXCLUDE)
	
endif(OGRE_VC8_DIR)

if(OGRE_VC9_DIR)
	message(${OGRE_VC9_DIR})
	install(DIRECTORY ${OGRE_VC9_DIR}/bin DESTINATION vc90)
	install(DIRECTORY ${OGRE_VC9_DIR}/lib DESTINATION vc90 PATTERN "opt" EXCLUDE)
endif(OGRE_VC9_DIR)

if(OGRE_VC10_DIR)
	message(${OGRE_VC10_DIR})
	install(DIRECTORY ${OGRE_VC10_DIR}/bin DESTINATION vc100)
	install(DIRECTORY ${OGRE_VC10_DIR}/lib DESTINATION vc100 PATTERN "opt" EXCLUDE )
endif(OGRE_VC10_DIR)