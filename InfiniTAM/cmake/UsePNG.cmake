################
# UsePNG.cmake #
################
OPTION(WITH_PNG "Build with libpng support?" OFF)
#message("defined address!!!!!!!!!!")
#message("option is ${WITH_PNG}")

IF(WITH_PNG)
  FIND_PACKAGE(PNG REQUIRED)
  message("defined address!!!!!!!!!!")
  INCLUDE_DIRECTORIES(${PNG_INCLUDE_DIRS})
  ADD_DEFINITIONS(${PNG_DEFINITIONS})
  ADD_DEFINITIONS(-DUSE_LIBPNG)
ENDIF()
