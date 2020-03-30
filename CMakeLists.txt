CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(lutenist)

SET(CMAKE_CXX_STANDARD 17)

IF (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
  INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
  CONAN_BASIC_SETUP()
ELSE ()
  MESSAGE(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
ENDIF ()

IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  SET(SDLLib "-framework AudioToolbox -framework CoreAudio -framework Carbon")
  SET(SDLLib ${SDLLib} "-framework ForceFeedback -framework IOKit -framework Cocoa -framework CoreVideo")
  SET(SDLLib ${SDLLib} iconv SDL2)
ELSE ()
  SET(SDLLib SDL2)
ENDIF ()

INCLUDE_DIRECTORIES(include)

ADD_LIBRARY(lutenist
    include/lutenist/vector.h
    include/lutenist/matrix.h
    include/lutenist/quaternion.h
    include/lutenist/plane.h
    include/lutenist/convex_volume.h
    include/lutenist/box.h
    include/lutenist/size.h
    include/lutenist/rect.h
    include/lutenist/constants.h
    include/lutenist/color.h
    include/lutenist/utils.h
    include/lutenist/render_target.h
    include/lutenist/texture.h
    include/lutenist/sampler.h
    src/texture.cc
    include/lutenist/vertex.h
    include/lutenist/shader.h
    include/lutenist/depth_state.h
    include/lutenist/renderer.h
    include/lutenist/render_error.h
    include/lutenist/blend_state.h
    src/renderer.cc
    include/lutenist.h)

IF (BUILD_EXAMPLES)
  ADD_SUBDIRECTORY(examples)
ENDIF ()