cmake_minimum_required(VERSION 3.10)

project(ubuntu_fetcher)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(CURL REQUIRED)
#find_package(PkgConfig REQUIRED)
#pkg_check_modules(JSON_C REQUIRED json-c)

add_executable(ubuntu_fetcher
    ../main.cpp
    ../distro_fetcher.cpp
)

target_include_directories(ubuntu_fetcher
    PRIVATE
    ../
    #${CURL_INCLUDE_DIRS}
    #${JSON_C_INCLUDE_DIRS}
)

target_link_libraries(ubuntu_fetcher
    PRIVATE
    #${CURL_LIBRARIES}
    #${JSON_C_LIBRARIES}
)
