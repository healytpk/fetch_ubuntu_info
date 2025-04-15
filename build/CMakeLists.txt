cmake_minimum_required(VERSION 3.22.0)

cmake_policy(SET CMP0026 OLD)

set(CMAKE_CXX_STANDARD 17)

# For Linux we will need to execute CMake twice in order to generate
# Debug and Release versions of Makefiles
if(UNIX AND NOT APPLE)
	set(LINUX ON)
	set(LIB_SUFFIX /${CMAKE_BUILD_TYPE})
endif(UNIX AND NOT APPLE)

if(LINUX)
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
endif(LINUX)

if(APPLE)
    # for std::stop_source
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-fexperimental-library>)
endif(APPLE)

# We will generate both Debug and Release project files at the same time
# for Windows and OS X
if(WIN32 OR APPLE)
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
	set(LIB_SUFFIX "") 
endif(WIN32 OR APPLE)

set(PROJECT_NAME ubuntu_fetcher)
project(${PROJECT_NAME})

# If there are any additional CMake modules (e.g. module which searches 
# for OpenCV or for DirectShow libs), then CMake should start searching 
# for them in current folder
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

if(APPLE)
	set(OS_BASE_NAME Mac)
endif(APPLE)
if(LINUX)
	set(OS_BASE_NAME Linux)
endif(LINUX)
if(WIN32)
	set(OS_BASE_NAME Win)
endif(WIN32)

# Variable which points to root folder of our source code
set(PROJECT_ROOT_DIR ${PROJECT_SOURCE_DIR}/..)

include_directories(${PROJECT_ROOT_DIR}/include ${PROJECT_ROOT_DIR})

add_subdirectory (../ ../../${OS_BASE_NAME}${LIB_SUFFIX})

