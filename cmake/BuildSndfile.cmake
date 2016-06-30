include(ExternalProject)

ExternalProject_Add(libsndfile
  GIT_REPOSITORY      https://github.com/erikd/libsndfile.git
  BUILD_IN_SOURCE 1
  INSTALL_DIR vendor/libsndfile
  CONFIGURE_COMMAND ./configure --prefix=${CMAKE_BINARY_DIR}/vendor/libsndfile --disable-external-libs --disable-shared
  BUILD_COMMAND $(MAKE) V=1
  INSTALL_COMMAND $(MAKE) install
  )

ExternalProject_Add_Step(libsndfile autogen
  COMMAND              <SOURCE_DIR>/autogen.sh
  DEPENDEES            update
  DEPENDERS            configure
)
