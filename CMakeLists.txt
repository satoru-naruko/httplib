cmake_minimum_required(VERSION 3.0)

project(httplib)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL ${_HTTPLIB_OPENSSL_MIN_VER} COMPONENTS Crypto SSL REQUIRED)

ADD_LIBRARY(httplib STATIC 
${CMAKE_CURRENT_SOURCE_DIR}/stream/SSLSocketStream.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/stream/Stream.cpp
${CMAKE_CURRENT_SOURCE_DIR}/ssl/SslContext.cpp
)

target_include_directories(httplib
PUBLIC
    ${OPENSSL_INCLUDE_DIR}
)

target_link_libraries(httplib
PUBLIC
    OpenSSL::SSL
    OpenSSL::Crypto
)