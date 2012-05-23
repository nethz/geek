#在以下路径中寻找 utf8
find_path(URAPIDXML_DIR
  NAMES rapidxml.hpp
  PATHS 
  ${CMAKE_CURRENT_SOURCE_DIR}/rapidxml-1.13
  ${CMAKE_CURRENT_SOURCE_DIR}/../rapidxml-1.13
  NO_DEFAULT_PATH
)

if(URAPIDXML_DIR)
	install(DIRECTORY ${URAPIDXML_DIR}/ DESTINATION include/rapidxml)
endif(URAPIDXML_DIR)