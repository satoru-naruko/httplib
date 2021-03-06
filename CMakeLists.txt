cmake_minimum_required(VERSION 3.0)

project(httplib)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL ${_HTTPLIB_OPENSSL_MIN_VER} COMPONENTS Crypto SSL REQUIRED)

ADD_LIBRARY(httplib STATIC 
${CMAKE_CURRENT_SOURCE_DIR}/stream/SSLSocketStream.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/stream/Stream.cpp
${CMAKE_CURRENT_SOURCE_DIR}/ssl/SslContext.cpp
${CMAKE_CURRENT_SOURCE_DIR}/socket/Socket.cpp
${CMAKE_CURRENT_SOURCE_DIR}/httpclient/HttpsClient.cpp
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

# install library file
install(TARGETS httplib DESTINATION "${PROJECT_SOURCE_DIR}/lib")

# install headers
install(FILES "${PROJECT_SOURCE_DIR}/ssl/SslContext.h" DESTINATION "${PROJECT_SOURCE_DIR}/include")
install(FILES "${PROJECT_SOURCE_DIR}/httpclient/HttpsClient.h" DESTINATION "${PROJECT_SOURCE_DIR}/include")
install(FILES "${PROJECT_SOURCE_DIR}/socket/Socket.h" DESTINATION "${PROJECT_SOURCE_DIR}/include/socket")
