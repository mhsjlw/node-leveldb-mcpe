cmake_minimum_required(VERSION 3.0)
project(node_leveldb_mcpe_native)

include(ExternalProject)

if(UNIX)
  ExternalProject_Add(
      leveldb
      GIT_REPOSITORY    "https://github.com/Mojang/leveldb-mcpe.git"
      GIT_TAG           "8fb685effc48819cb54b2f353871fbc80b8c67ed"
      CONFIGURE_COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/ext/leveldb/include
                        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/ext/leveldb/lib
      PATCH_COMMAND     ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/patches/force_fpic_linux.patch ${CMAKE_BINARY_DIR}/leveldb-prefix/src/leveldb/
                        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/patches/snappy_path_linux.patch ${CMAKE_BINARY_DIR}/leveldb-prefix/src/leveldb/
                        COMMAND git apply force_fpic_linux.patch
                        COMMAND git apply snappy_path_linux.patch
      BUILD_COMMAND     $(MAKE) out-static/libleveldb.a
      INSTALL_COMMAND   ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/leveldb-prefix/src/leveldb/out-static/libleveldb.a ${CMAKE_BINARY_DIR}/ext/leveldb/lib
                        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/leveldb-prefix/src/leveldb/include/ ${CMAKE_BINARY_DIR}/ext/leveldb/include/
      BUILD_IN_SOURCE   1
  )
else(NOT UNIX)
  ExternalProject_Add(
      leveldb
      GIT_REPOSITORY    "https://github.com/Mojang/leveldb-mcpe.git"
      GIT_TAG           "8fb685effc48819cb54b2f353871fbc80b8c67ed"
      CONFIGURE_COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/ext/leveldb/include
                        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/ext/leveldb/lib
      BUILD_COMMAND     $(MAKE) out-static/libleveldb.a
      INSTALL_COMMAND   ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/leveldb-prefix/src/leveldb/out-static/libleveldb.a ${CMAKE_BINARY_DIR}/ext/leveldb/lib
                        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/leveldb-prefix/src/leveldb/include/ ${CMAKE_BINARY_DIR}/ext/leveldb/include/
      BUILD_IN_SOURCE   1
  )
endif(UNIX)

file(GLOB SOURCE_FILES src/*.cc)

include_directories(${CMAKE_BINARY_DIR}/ext/leveldb/include)
set(LIBRARIES ${CMAKE_BINARY_DIR}/ext/leveldb/lib/libleveldb.a)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})
add_dependencies(${PROJECT_NAME} leveldb)
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} ${LIBRARIES})
