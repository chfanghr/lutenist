CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(lutenist)

SET(CMAKE_CXX_STANDARD 17)

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