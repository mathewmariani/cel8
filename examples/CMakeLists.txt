macro(emscripten target)
  if (CMAKE_SYSTEM_NAME STREQUAL Emscripten)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    target_link_options(${target} PRIVATE
      -sINITIAL_MEMORY=50MB
      -sMAXIMUM_MEMORY=200MB
      -sALLOW_MEMORY_GROWTH=1
      -sUSE_WEBGL2=1
      $<$<CONFIG:Debug>:-g>
      $<$<CONFIG:Debug>:--shell-file ../extra/shell.html>)
  endif()
endmacro()

macro(c8_project target file)
  add_executable(${target} ${file})
  emscripten(${target})
  target_link_libraries(${target} PRIVATE sokol)
  target_include_directories(${target} PUBLIC ${THIRDPARTY_DIR})
endmacro()

# programs
c8_project(clock clock-sokol.c)
c8_project(crosshair crosshair-sokol.c)
c8_project(font font-sokol.c)
c8_project(glitch glitch-sokol.c)
c8_project(hello hello-sokol.c)
c8_project(palette palette-sokol.c)
c8_project(snake snake-sokol.c)
c8_project(sprite sprite-sokol.c)
c8_project(tetris tetris-sokol.c)