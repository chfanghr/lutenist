INCLUDE_DIRECTORIES(include)

ADD_LIBRARY(demo
    include/lutenist/demo/app.h
    include/lutenist/demo/bmp.h
    include/lutenist/demo/texture_shader.h
    include/lutenist/demo/utils.h
    src/app.cc
    src/texture_shader.cc
    src/utils.cc
    src/bmp.cc)

ADD_EXECUTABLE(demo_fake_platform platforms/fake/main.cc)

TARGET_LINK_LIBRARIES(demo_fake_platform demo lutenist)