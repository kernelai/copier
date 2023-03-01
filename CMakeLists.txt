cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)
project(copier VERSION 0.1.0)

set(EP_BASE_SUFFIX "buildtrees")
set_property(DIRECTORY PROPERTY EP_BASE ${CMAKE_CURRENT_SOURCE_DIR}/${EP_BASE_SUFFIX})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules/")
set(STAGED_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/deps)
set(INSTALL_INCLUDEDIR ${STAGED_INSTALL_PREFIX}/include)
set(INSTALL_LIBDIR ${STAGED_INSTALL_PREFIX}/lib)
set(INSTALL_LIBDIR_64 ${STAGED_INSTALL_PREFIX}/lib64)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${STAGED_INSTALL_PREFIX})
link_directories(${INSTALL_LIBDIR} ${INSTALL_LIBDIR_64})
message(STATUS "${PROJECT_NAME} staged install: ${STAGED_INSTALL_PREFIX}")
include(ExternalProject)

ExternalProject_Add(gtest
  URL
  https://github.com/google/googletest/archive/refs/tags/release-1.12.1.tar.gz
  URL_HASH
  MD5=e82199374acdfda3f425331028eb4e2a
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
  BUILD_ALWAYS
  1
)
set(GTEST_INCLUDE_DIR ${INSTALL_INCLUDEDIR})
set(GTEST_LIBRARY ${INSTALL_LIBDIR}/libgtest.a)

ExternalProject_Add(gflags
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
  BUILD_ALWAYS
  1
  CMAKE_ARGS
  -DCMAKE_INSTALL_PREFIX=${STAGED_INSTALL_PREFIX}
  -DGFLAGS_NAMESPACE=gflags
  -DBUILD_STATIC_LIBS=ON
  -DBUILD_SHARED_LIBS=OFF
)
set(GFLAGS_LIBRARIE ${INSTALL_LIBDIR}/libgflags.a)
set(GFLAGS_INCLUDE_DIR ${INSTALL_INCLUDEDIR})

ExternalProject_Add(unwind
  DEPENDS
  URL
  http://download.savannah.nongnu.org/releases/libunwind/libunwind-1.6.2.tar.gz
  URL_HASH
  MD5=f625b6a98ac1976116c71708a73dc44a 
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
  make -j8
  INSTALL_COMMAND
  make install
)
set(UNWIND_LIBRARIE ${INSTALL_LIBDIR}/libunwind.a)
set(UNWIND_INCLUDE_DIR ${INSTALL_INCLUDEDIR})

ExternalProject_Add(glog
  DEPENDS
  gflags
  gtest
  unwind
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
  BUILD_ALWAYS
  1
  CMAKE_ARGS
  -DCMAKE_INSTALL_PREFIX=${STAGED_INSTALL_PREFIX}
  -DWITH_GFLAGS=ON
  -DBUILD_TESTING=OFF
  -DBUILD_STATIC_LIBS=ON
  -DBUILD_SHARED_LIBS=OFF
  -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
)
set(GLOG_LIBRARIE ${INSTALL_LIBDIR}/libglog.a)
set(GLOG_INCLUDE_DIR ${INSTALL_INCLUDEDIR})

ExternalProject_Add(snappy
  DEPENDS
  URL
  https://github.com/google/snappy/archive/refs/tags/1.1.7.tar.gz
  URL_HASH
  MD5=ee9086291c9ae8deb4dac5e0b85bf54a
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
  -DSNAPPY_BUILD_TESTS=OFF
  -DBUILD_STATIC_LIBS=ON
  -DBUILD_SHARED_LIBS=OFF
  BUILD_ALWAYS
  1
)
set(SNAPPY_LIBRARIE ${INSTALL_LIBDIR}/libsnappy.a)
set(SNAPPY_INCLUDE_DIR ${INSTALL_INCLUDEDIR})

ExternalProject_Add(zstd
  DEPENDS
  URL
  https://github.com/facebook/zstd/releases/download/v1.5.4/zstd-1.5.4.tar.gz
  URL_HASH
  MD5=2352b1f9ccc7446641046bb3d440c3ed
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
  SOURCE_SUBDIR
  build/cmake
  CMAKE_ARGS
  -DCMAKE_INSTALL_PREFIX=${STAGED_INSTALL_PREFIX}
  -DBUILD_TESTING=OFF
  -DZSTD_BUILD_STATIC=ON
  -DZSTD_BUILD_SHARED=OFF
  BUILD_ALWAYS
  1
)
set(ZSTD_LIBRARIE ${INSTALL_LIBDIR}/libzstd.a)
set(ZSTD_INCLUDE_DIR ${INSTALL_INCLUDEDIR})

ExternalProject_Add(lz4
  DEPENDS
  URL
  https://github.com/lz4/lz4/archive/refs/tags/v1.9.4.tar.gz
  URL_HASH
  MD5=e9286adb64040071c5e23498bf753261
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
  SOURCE_SUBDIR
  build/cmake
  CMAKE_ARGS
  -DCMAKE_INSTALL_PREFIX=${STAGED_INSTALL_PREFIX}
  -DBUILD_TESTING=OFF
  -DBUILD_STATIC_LIBS=ON
  -DBUILD_SHARED_LIBS=OFF
  BUILD_ALWAYS
  1
)
set(LZ4_LIBRARIE ${INSTALL_LIBDIR}/liblz4.a)
set(LZ4_INCLUDE_DIR ${INSTALL_INCLUDEDIR})

ExternalProject_Add(zlib
  DEPENDS
  URL
  https://github.com/madler/zlib/releases/download/v1.2.13/zlib-1.2.13.tar.gz
  URL_HASH
  MD5=9b8aa094c4e5765dabf4da391f00d15c
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
  BUILD_ALWAYS
  1
)
set(ZLIB_LIBRARIE ${INSTALL_LIBDIR}/libz.a)
set(ZLIB_INCLUDE_DIR ${INSTALL_INCLUDEDIR})

ExternalProject_Add(gperftools
  DEPENDS
  URL
  https://github.com/gperftools/gperftools/releases/download/gperftools-2.10/gperftools-2.10.tar.gz
  URL_HASH
  MD5=62bf6c76ba855ed580de5e139bd2a483
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
  -DGPERFTOOLS_BUILD_STATIC=ON
  -DEFAULT_BUILD_MINIMAL=ON
)
set(TCMALLOC_LIBRARIE ${INSTALL_LIBDIR}/libtcmalloc.a)
set(TCMALLOC_INCLUDE_DIR ${INSTALL_INCLUDEDIR})

ExternalProject_Add(jemalloc
  DEPENDS
  URL
  https://github.com/jemalloc/jemalloc/archive/refs/tags/5.3.0.tar.gz
  URL_HASH
  MD5=594dd8e0a1e8c1ef8a1b210a1a5aff5b
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
  <SOURCE_DIR>/autogen.sh --prefix=${STAGED_INSTALL_PREFIX}
  BUILD_IN_SOURCE
  1
  BUILD_COMMAND
  make -j8
  BUILD_ALWAYS
  1
  INSTALL_COMMAND
  make install
)
set(JEMALLOC_LIBRARIE ${INSTALL_LIBDIR}/libjemalloc.a)
set(JEMALLOC_INCLUDE_DIR ${INSTALL_INCLUDEDIR})

ExternalProject_Add(boost
  DEPENDS
  URL
  https://boostorg.jfrog.io/artifactory/main/release/1.78.0/source/boost_1_78_0.tar.gz
  URL_HASH
  MD5=c2f6428ac52b0e5a3c9b2e1d8cc832b5
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
  <SOURCE_DIR>/bootstrap.sh --prefix=${STAGED_INSTALL_PREFIX}
  BUILD_IN_SOURCE
  1
  BUILD_COMMAND
  <SOURCE_DIR>/b2
  --with-atomic
  --with-chrono
  --with-container
  --with-context
  --with-contract
  --with-coroutine
  --with-date_time
  --with-exception
  --with-fiber
  --with-filesystem
  --with-graph
  --with-graph_parallel
  --with-iostreams
  --with-locale
  --with-log
  --with-math
  --with-mpi
  --with-program_options
  --with-python
  --with-random
  --with-regex
  --with-serialization
  --with-stacktrace
  --with-system
  --with-test
  --with-thread
  --with-timer
  --with-type_erasure
  --with-wave
  BUILD_ALWAYS
  1
  INSTALL_COMMAND
  <SOURCE_DIR>/b2 install
)

ExternalProject_Add(libevent
  DEPENDS
  URL
  https://github.com/libevent/libevent/releases/download/release-2.1.12-stable/libevent-2.1.12-stable.tar.gz
  URL_HASH
  MD5=b5333f021f880fe76490d8a799cd79f4
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
  -DEVENT__DISABLE_TESTS=ON
  -DEVENT__DISABLE_BENCHMARK=ON
  -DEVENT__DISABLE_SAMPLES=ON
  -DEVENT__DISABLE_REGRESS=ON
  -DEVENT__BUILD_SHARED_LIBRARIES=ON
  BUILD_ALWAYS
  1
)

ExternalProject_Add(libsodium
  DEPENDS
  URL
  https://github.com/jedisct1/libsodium/releases/download/1.0.17/libsodium-1.0.17.tar.gz
  URL_HASH
  MD5=0f71e2680187a1558b5461e6879342c5
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
  make -j8
  INSTALL_COMMAND
  make install
)

ExternalProject_Add(double-conversion
  DEPENDS
  URL
  https://github.com/google/double-conversion/archive/refs/tags/v3.1.4.tar.gz
  URL_HASH
  MD5=9851753f352c7079c8c8fd07c63dd1e7
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
)

ExternalProject_Add(fmt
  DEPENDS
  URL
  https://github.com/fmtlib/fmt/archive/refs/tags/8.0.1.tar.gz
  URL_HASH
  MD5=7d5af964c6633ef90cd6a47be3afe6a0
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
  -DFMT_TEST=OFF
  -DFMT_DOC=OFF
)

ExternalProject_Add(liburing
  DEPENDS
  URL
  https://github.com/axboe/liburing/archive/refs/tags/liburing-2.3.tar.gz
  URL_HASH
  # MD5=7d5af964c6633ef90cd6a47be3afe6a0
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
  make -j8
  INSTALL_COMMAND
  make install
)

ExternalProject_Add(folly
  DEPENDS
  gflags
  glog
  gtest
  boost
  libevent
  libsodium
  double-conversion
  fmt
  lz4
  snappy
  zstd
  liburing
  URL
  https://github.com/facebook/folly/releases/download/v2023.02.27.00/folly-v2023.02.27.00.tar.gz
  URL_HASH
  # MD5=7d5af964c6633ef90cd6a47be3afe6a0
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
)

include(CTest)
enable_testing()

add_executable(copier main.cpp)

add_dependencies(copier glog gflags gtest boost libevent libsodium double-conversion fmt lz4 snappy zstd liburing folly)
target_include_directories(copier PRIVATE ${INSTALL_INCLUDEDIR})
target_link_libraries(copier libglog.a libgflags.a libunwind.a)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
