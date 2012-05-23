option(setup_ogre_resources_cfg "setup ogre resources" OFF)
option(setup_ogre_plugins_cfg "setup ogre plugins" OFF)

set(ogre_template_path ${GEEK_CMAKE_MODULE_PATH}/templates/ogre)
set(generation_ogre_files_path ${CMAKE_BINARY_DIR}/generation/ogre)
if(NOT EXISTS ${generation_ogre_files_path})
	file(MAKE_DIRECTORY ${generation_ogre_files_path})
endif()

if(setup_ogre_plugins_cfg)
message(" ${ogre_template_path}/Plugins.cfg.in")
	configure_file(
	   ${ogre_template_path}/debug/Plugins.cfg.in
	   ${generation_ogre_files_path}/debug/Plugins.cfg
	   @ONLY
	)
	configure_file(
	   ${ogre_template_path}/release/Plugins.cfg.in
	   ${generation_ogre_files_path}/release/Plugins.cfg
	   @ONLY
	)
	install(FILES ${generation_ogre_files_path}/debug/Plugins.cfg CONFIGURATIONS Debug DESTINATION bin/debug)
	install(FILES ${generation_ogre_files_path}/release/Plugins.cfg CONFIGURATIONS Release DESTINATION bin/release)
endif(setup_ogre_plugins_cfg)


if(setup_ogre_resources_cfg)
	configure_file(
	   ${ogre_template_path}/debug/resources.cfg.in
	   ${generation_ogre_files_path}/debug/resources.cfg
	   @ONLY
	)
	configure_file(
	   ${ogre_template_path}/release/resources.cfg.in
	   ${generation_ogre_files_path}/release/resources.cfg
	   @ONLY
	)
	install(FILES ${generation_ogre_files_path}/debug/resources.cfg CONFIGURATIONS Debug DESTINATION bin/debug)
	install(FILES ${generation_ogre_files_path}/release/resources.cfg CONFIGURATIONS Release DESTINATION bin/release)
endif(setup_ogre_resources_cfg)

