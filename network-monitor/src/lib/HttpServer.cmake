
# The two following lines are needed to find the microhttpd library
# on macOS. On Linux, the library is found automatically.
list(APPEND CMAKE_PREFIX_PATH /usr/local /opt/brew)
find_library(MICROHTTPD_LIB microhttpd)
find_path(MICROHTTPD_INCLUDE_DIR microhttpd)

target_link_libraries(${LF_MAIN_TARGET} PRIVATE microhttpd)
target_include_directories(${LF_MAIN_TARGET} PRIVATE {MICROHTTPD_INCLUDE_DIR})
