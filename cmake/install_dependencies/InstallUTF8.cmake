#在以下路径中寻找 utf8
find_path(UTF8_DIR
  NAMES utf8.h utf8/unchecked.h
  PATHS 
  ${CMAKE_CURRENT_SOURCE_DIR}/utf8_v2_3_1/source
  ${CMAKE_CURRENT_SOURCE_DIR}/../utf8_v2_3_1/source
  NO_DEFAULT_PATH
)
#string(REGEX REPLACE <regular_expression>
 #        <replace_expression> <output variable>
 #        <input> [<input>...])
if(UTF8_DIR)
	install(DIRECTORY ${UTF8_DIR}/ DESTINATION include/utf8)
endif(UTF8_DIR)