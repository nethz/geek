#������·����Ѱ�� dependencies��������Ҫ����cmake�ļ��У�
find_path(GEEK_CMAKE_MODULE_PATH
  dependencies.cmake
  PATHS 
  ${PROJECT_SOURCE_DIR}/cmake
  NO_DEFAULT_PATH
)

message(${PROJECT_SOURCE_DIR}/dependencies)
#����Ҳ����������û�ȥ���ؽ�ѹ
if(NOT GEEK_CMAKE_MODULE_PATH)
	MESSAGE(SEND_ERROR "[need to do]")
endif(NOT GEEK_CMAKE_MODULE_PATH)
