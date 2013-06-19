ExternalProject_Add(
  boost
  URL http://sourceforge.net/projects/boost/files/boost/1.53.0/boost_1_53_0.tar.gz/download
  UPDATE_COMMAND ""
  # BUILD_COMMAND bash -c "cd <SOURCE_DIR> && ./b2"
  # INSTALL_COMMAND bash -c "cd <SOURCE_DIR> && ./b2 install"
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  CONFIGURE_COMMAND bash -c "cd <SOURCE_DIR> && ./bootstrap.sh --prefix=<INSTALL_DIR>"
  LOG_DOWNLOAD ON
  LOG_CONFIGURE ON
  LOG_BUILD ON
  LOG_INSTALL ON
  )

ExternalProject_Get_Property(boost source_dir)
set(BOOST_INCLUDE_DIR ${source_dir})

ExternalProject_Get_Property(boost install_dir)
set(BOOST_LIB_DIR ${install_dir}/lib)
