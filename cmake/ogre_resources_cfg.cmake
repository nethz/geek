option(setup_ogre_resources_cfg "setup ogre resources" OFF)
if(setup_ogre_resources_cfg)
	set(ogre_template_path ${GEEK_CMAKE_MODULE_PATH}/templates/ogre)
	set(generation_ogre_files_path ${CMAKE_BINARY_DIR}/generation/ogre)
	if(NOT EXISTS ${generation_ogre_files_path})
		file(MAKE_DIRECTORY ${generation_ogre_files_path})
	endif()
	configure_file(
	   ${ogre_template_path}/Plugins.cfg.in
	   ${generation_ogre_files_path}/Plugins.cfg.in
	   @ONLY
	)
endif(setup_ogre_resources_cfg)