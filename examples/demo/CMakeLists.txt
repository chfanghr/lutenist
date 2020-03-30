INCLUDE_DIRECTORIES(include)

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

ADD_EXECUTABLE(demo_sdl platforms/sdl/main.cc)

TARGET_LINK_LIBRARIES(demo_fake_platform demo lutenist)

TARGET_LINK_LIBRARIES(demo_sdl demo lutenist ${SDLLib})