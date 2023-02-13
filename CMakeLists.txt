cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/third/vcpkg/scripts/buildsystems/vcpkg.cmake"
#   CACHE STRING "Vcpkg toolchain file")
set_property(DIRECTORY PROPERTY EP_BASE ${CMAKE_CURRENT_SOURCE_DIR}/buildtrees/subprojects)
set(STAGED_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/installed)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules/")

include(ExternalProject)
ExternalProject_Add(Gflags
  URL
    https://github.com/gflags/gflags/archive/refs/tags/v2.2.2.tar.gz
  URL_HASH
    MD5=1a865b93bacfa963201af3f75b7bd64c
  DOWNLOAD_NO_PROGRESS
    1
  UPDATE_COMMAND
    ""
  LOG_CONFIGURE
    1
  LOG_BUILD
    1
  LOG_INSTALL
    1
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${STAGED_INSTALL_PREFIX}
    -DGFLAGS_NAMESPACE=gflags

  )
  set(GFLAGS_LIBRARIES
    ${STAGED_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/libgflags.a
    CACHE PATH "Path to internally built library directories"
    FORCE
    )
    
 set(GFLAGS_INCLUDE_DIRS
    ${STAGED_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}
    CACHE PATH "Path to internally built include directories"
    FORCE
    )

ExternalProject_Add(Glog
   DEPENDS
     Gflags
   URL
     https://github.com/google/glog/archive/refs/tags/v0.6.0.tar.gz
   URL_HASH
     MD5=c98a6068bc9b8ad9cebaca625ca73aa2
   DOWNLOAD_NO_PROGRESS
     1
   UPDATE_COMMAND
     ""
   LOG_CONFIGURE
     1
   LOG_BUILD
     1
   LOG_INSTALL
     1
   CMAKE_ARGS
     -DCMAKE_INSTALL_PREFIX=${STAGED_INSTALL_PREFIX}
     -DWITH_GTEST=OFF
     -DBUILD_SHARED_LIBS=OFF
     -DCMAKE_INCLUDE_PATH:PATH=${GFLAGS_INCLUDE_DIRS}
     -DCMAKE_LIBRARY_PATH:PATH=${GFLAGS_LIBRARIES}
   )
   set(
       GLOG_LIBRARIES
       ${STAGED_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/libglog.a
       CACHE PATH "Path to internally built library directories"
       FORCE
   )
   set(
       GLOG_INCLUDE_DIRS
       ${STAGED_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}
       CACHE PATH "Path to internally built include directories"
       FORCE
   )

ExternalProject_Add(Liburing
   DEPENDS
    #  Gflags
   URL
     https://github.com/axboe/liburing/archive/refs/tags/liburing-2.3.tar.gz
   URL_HASH
    #  MD5=c98a6068bc9b8ad9cebaca625ca73aa2
   DOWNLOAD_NO_PROGRESS
     1
   UPDATE_COMMAND
     ""
   LOG_CONFIGURE
     1
   LOG_BUILD
     1
   LOG_INSTALL
     1
   CONFIGURE_COMMAND
    <SOURCE_DIR>/configure --prefix=${STAGED_INSTALL_PREFIX} 
   BUILD_IN_SOURCE
     1
   BUILD_COMMAND
    make
   INSTALL_COMMAND
    make install
)

   set(
       LIBURING_LIBRARIES
       ${STAGED_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/liburing.a
       CACHE PATH "Path to internally built library directories"
       FORCE
   )
   set(
       LIBURING_INCLUDE_DIRS
       ${STAGED_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}
       CACHE PATH "Path to internally built include directories"
       FORCE
   )
set(Boost_NO_WARN_NEW_VERSIONS 1)
project(copier VERSION 0.1.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules/")
# find_package(GTest CONFIG REQUIRED)
# find_package(glog CONFIG REQUIRED)
# find_package(Gflags CONFIG REQUIRED)
# find_package(folly CONFIG REQUIRED)

include(CTest)
enable_testing()

add_executable(copier main.cpp)

# add_executable(copier main.cpp fibers.cpp)
# target_link_libraries(copier Folly::folly)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
