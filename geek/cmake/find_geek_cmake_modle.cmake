#在以下路径中寻找 dependencies（其中需要包含cmake文件夹）
find_path(GEEK_CMAKE_MODULE_PATH
  dependencies.cmake
  PATHS 
  ${PROJECT_SOURCE_DIR}/cmake
  NO_DEFAULT_PATH
)

message(${PROJECT_SOURCE_DIR}/dependencies)
#如果找不到，提醒用户去下载解压
if(NOT GEEK_CMAKE_MODULE_PATH)
	MESSAGE(SEND_ERROR "[need to do]")
endif(NOT GEEK_CMAKE_MODULE_PATH)
