target_link_libraries(${LF_MAIN_TARGET} PRIVATE microhttpd)

# The two following lines are needed to find the microhttpd library
# on macOS. On Linux, the library is found automatically.
target_include_directories(${LF_MAIN_TARGET} PRIVATE "/usr/local/include")
target_link_directories(${LF_MAIN_TARGET} PRIVATE "/usr/local/lib")