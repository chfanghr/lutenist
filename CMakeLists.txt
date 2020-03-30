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

ADD_LIBRARY(demo
    examples/demo/app.cc
    examples/demo/app.h
    examples/demo/bmp.h
    examples/demo/texture_shader.h)

ADD_EXECUTABLE(demo_fake_platform examples/demo/platforms/fake/main.cc)

TARGET_LINK_LIBRARIES(demo_fake_platform demo lutenist)