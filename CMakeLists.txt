cmake_minimum_required(VERSION 3.19)

project(httplib)

find_package(OpenSSL ${_HTTPLIB_OPENSSL_MIN_VER} COMPONENTS Crypto SSL REQUIRED)

ADD_LIBRARY(httplib STATIC ${CMAKE_CURRENT_SOURCE_DIR}/stream/SSLSocketStream.cpp ${CMAKE_CURRENT_SOURCE_DIR}/stream/Stream.cpp)