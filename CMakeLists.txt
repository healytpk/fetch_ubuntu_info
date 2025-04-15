cmake_minimum_required(VERSION 3.10)

project(ubuntu_fetcher)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find CURL
find_package(CURL REQUIRED)
# Find json-c manually if it's not available through find_package
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSON_C REQUIRED json-c)

# Define the executable and source files
add_executable(ubuntu_fetcher
    main.cpp
    distro_fetcher.cpp
)

# Include directories
target_include_directories(ubuntu_fetcher
    PRIVATE
    ${CURL_INCLUDE_DIRS}
    ${JSON_C_INCLUDE_DIRS}
    include
)

# Link libraries manually for CURL and JSON-C
target_link_libraries(ubuntu_fetcher
    PRIVATE
    ${CURL_LIBRARIES}
    ${JSON_C_LIBRARIES}
)
