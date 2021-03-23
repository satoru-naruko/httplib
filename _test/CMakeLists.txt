cmake_minimum_required(VERSION 3.0)

project(httplibtest)

link_directories(../lib)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL ${_HTTPLIB_OPENSSL_MIN_VER} COMPONENTS Crypto SSL REQUIRED)

ADD_EXECUTABLE(httplibtest test.cpp)

target_include_directories(httplibtest
PUBLIC
    ${OPENSSL_INCLUDE_DIR}
    "../include"
)

target_link_libraries(httplibtest
PUBLIC
    httplib
    OpenSSL::SSL
    OpenSSL::Crypto
)
