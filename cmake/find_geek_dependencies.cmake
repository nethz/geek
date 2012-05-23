find_path(geek_dependencies_path
  include/boost/config.hpp
  PATHS 
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/..
  ${PROJECT_SOURCE_DIR}/../..
  ${PROJECT_SOURCE_DIR}/dependencies
  ${PROJECT_SOURCE_DIR}/all_dependencies
  NO_DEFAULT_PATH
)

