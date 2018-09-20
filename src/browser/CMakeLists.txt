#-----------------------------------------------------------------
# Project converted from qmake file using q2c
# https://github.com/benapetr/q2c at пн февр. 12 13:32:04 2018
#-----------------------------------------------------------------
cmake_minimum_required (VERSION 2.6)
project(browser)

set ( PLATFORM_INCLUDE "/usr/lib/x86_64-linux-gnu/glib-2.0/include/")

if(EXISTS "/etc/redhat-release")
     set ( PLATFORM_INCLUDE "/usr/lib64/glib-2.0/include/")
endif(EXISTS "/etc/redhat-release")

set(HEADERS_DIRS 
  "/usr/include/gtk-3.0/"
  "/usr/include/webkitgtk-4.0/"
  "/usr/include/glib-2.0/"
  "/usr/include/pango-1.0/"
  "/usr/include/gdk-pixbuf-2.0/"
  "/usr/include/cairo/"
  "/usr/include/atk-1.0/"
  "/usr/include/libsoup-2.4/"
  ${PLATFORM_INCLUDE})

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -g")

set(LIBS 
  gtk-3 
  webkit2gtk-4.0 
  glib-2.0 
  gobject-2.0
  pthread
  X11)

include_directories("./" ${HEADERS_DIRS})
link_libraries(${LIBS})

set(SOURCES "browser.cpp")
# add_executable(webkit_test ${SOURCES})
add_library ( ${PROJECT_NAME} SHARED
        ${SOURCES} )

target_link_libraries(${PROJECT_NAME} ${LIBS})
