# This module locates the developer's image library.
# http://openil.sourceforge.net/
#
# To help locate the library and include file, you could define an environment variable called
# DEVIL_ROOT which points to the root of the DevIL library installation. This is pretty useful
# on a Windows platform.
#
# This module sets:
# IL_LIBRARY the name of the IL library.
# ILU_LIBRARY the name of the ILU library.
# IL_INCLUDE_DIR where to find the il.h and ilu.h files.
# IL_FOUND this is set to TRUE if all the above variables were set.

# Original file by: Christopher Harvey


FIND_PATH(IL_INCLUDE_DIR il.h 
  $ENV{DEVIL_ROOT}
  PATH_SUFFIXES include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include/IL
  /usr/include/IL
  /usr/local/include
  /usr/include
  /sw/include/IL
  /sw/include
  /opt/local/include/IL
  /opt/local/include
  /opt/csw/include/IL
  /opt/csw/include 
  /opt/include
  /opt/include
  DOC "The path the the directory that contains il.h"
)

#MESSAGE("IL_INCLUDE_DIR is ${IL_INCLUDE_DIR}")

FIND_LIBRARY(IL_LIBRARY
  NAMES IL
  $ENV{DEVIL_ROOT}
  PATH_SUFFIXES lib64 lib lib32
  PATHS
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
  DOC "The file that corresponds to the base il library."
)

#MESSAGE("IL_LIBRARY is ${IL_LIBRARY}")

FIND_LIBRARY(ILU_LIBRARY
  NAMES ILU
  $ENV{DEVIL_ROOT}
  PATH_SUFFIXES lib64 lib lib32
  PATHS
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
  DOC "The file that corresponds to the il utility library."
)

#MESSAGE("ILU_LIBRARY is ${ILU_LIBRARY}")

SET(IL_FOUND FALSE)
IF(ILU_LIBRARY AND IL_LIBRARY AND IL_INCLUDE_DIR)
  SET(IL_FOUND TRUE)
ENDIF(ILU_LIBRARY AND IL_LIBRARY AND IL_INCLUDE_DIR)

IF(DevIL_FIND_REQUIRED AND NOT IL_FOUND)
  MESSAGE("Required DevIL library not found." FATAL_ERROR)
ENDIF(DevIL_FIND_REQUIRED AND NOT IL_FOUND)

IF(NOT DevIL_FIND_QUIETLY)
  IF(IL_FOUND)
    message(STATUS "Found DevIL: ${IL_LIBRARY} ${ILU_LIBRARY}")
  ELSE(IL_FOUND)
    MESSAGE("Could not find DevIL.")
  ENDIF(IL_FOUND)
ENDIF(NOT DevIL_FIND_QUIETLY)