include(ExternalProject)

set(SNDFILE_TARBALL "libsndfile-1.0.27")

ExternalProject_Add(libsndfile
  URL http://www.mega-nerd.com/libsndfile/files/${SNDFILE_TARBALL}.tar.gz
  BUILD_IN_SOURCE 1
  INSTALL_DIR vendor/libsndfile
  CONFIGURE_COMMAND ./configure --prefix=${CMAKE_BINARY_DIR}/vendor/libsndfile --disable-external-libs --disable-shared
  BUILD_COMMAND $(MAKE) V=1
  INSTALL_COMMAND $(MAKE) install
)
