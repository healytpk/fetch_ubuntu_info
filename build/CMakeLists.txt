cmake_minimum_required(VERSION 3.22.0)

cmake_policy(SET CMP0026 OLD)

set(CMAKE_CXX_STANDARD 23)

add_compile_definitions(DYNAMO_LOAD_PCAP_AT_RUNTIME=1)

# For Linux we will need to execute CMake twice in order to generate
# Debug and Release versions of Makefiles
if(UNIX AND NOT APPLE)
	set(LINUX ON)
	set(LIB_SUFFIX /${CMAKE_BUILD_TYPE})
endif(UNIX AND NOT APPLE)

if(WIN32)
    add_compile_definitions(DYNAMO_OS=1)
    add_compile_definitions(DYNAMO_NET=0)
elseif(APPLE)
    add_compile_definitions(DYNAMO_OS=2)
    add_compile_definitions(DYNAMO_NET=0)
elseif(LINUX)
    add_compile_definitions(DYNAMO_OS=3)
    add_compile_definitions(DYNAMO_NET=2)
endif()

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

set(PROJECT_NAME main_program)
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

# Here we specify the list of wxWidgets libs which we will use in our project
set(wxWidgets_USE_LIBS base core adv aui net gl xml propgrid html)

# Here we specify that we need DLL version of wxWidgets libs and dynamic CRT
# This is a MUST for applications with plugins. Both app and DLL plugin MUST
# use the same instance of wxWidgets and the same event loop.
set(BUILD_SHARED_LIBS ON)

# Find wxWidgets library on current PC
# You should have %WXWIN%  environment variable which should point to the
# directory where wxWidgets source code is placed.
# wxWidgets libs MUST be compiled for both Debug and Release versions
find_package(wxWidgets REQUIRED)


# For some reason CMake generates wrong list of definitions.
# Each item should start with /D but it does not.
# We need to fix that manually
set(wxWidgets_DEFINITIONS_TEMP)
foreach(DEFINITION ${wxWidgets_DEFINITIONS})
	
	if(NOT ${DEFINITION} MATCHES "/D.*")
		set(DEFINITION "/D${DEFINITION}")
	endif()
	set(wxWidgets_DEFINITIONS_TEMP ${wxWidgets_DEFINITIONS_TEMP} 
		${DEFINITION})
endforeach(${DEFINITION})
set(wxWidgets_DEFINITIONS ${wxWidgets_DEFINITIONS_TEMP})

# Here we add some definitions which prevent Visual Studio from 
# generating tons of warnings about unsecure function calls.
# See http://msdn.microsoft.com/en-us/library/ttcz0bys.aspx
if(WIN32)
	set(wxWidgets_DEFINITIONS ${wxWidgets_DEFINITIONS};
		/D_CRT_SECURE_NO_DEPRECATE;
		/D_CRT_NONSTDC_NO_DEPRECATE;
		/D_UNICODE;
		/D_WINSOCKAPI_)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /wd4996")
endif(WIN32)

# Since we are going to use wxWidgets in all subrojects,
# it's OK to create the variable which will contain 
# common preprocessor definitions. This variable will be 
# used in subprojects.
set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};
		${wxWidgets_DEFINITIONS})

# Variable which points to root folder of our source code
set(PROJECT_ROOT_DIR ${PROJECT_SOURCE_DIR}/..)

# If any ThirdParty libraries are going to be 
# used in our project then it would be better to put 
# them into separate subfolder. We will create 
# the variable which points to this subfolder.
set(THIRD_PARTY_DIR ${PROJECT_ROOT_DIR}/ThirdParty)

# Add wxWidgets include paths to the list of 
# include directories for all projects.
include_directories(${PROJECT_ROOT_DIR}/include ${PROJECT_ROOT_DIR} ${wxWidgets_INCLUDE_DIRS})

if(WIN32)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D__WXDEBUG__=1" )
endif(WIN32)
if(APPLE OR LINUX)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__WXDEBUG__=1" )
endif(APPLE OR LINUX)

# Now we can include all our subprojects. 
# CMake will generate project files for them
add_subdirectory (../binary_hexer ../../binary_hexer/${OS_BASE_NAME}${LIB_SUFFIX})
add_subdirectory (../wxModularCore ../../wxModularCore/${OS_BASE_NAME}${LIB_SUFFIX})
add_subdirectory (../plugin_arp ../../plugin_arp/${OS_BASE_NAME}${LIB_SUFFIX})
add_subdirectory (../main_program ../../main_program/${OS_BASE_NAME}${LIB_SUFFIX})

#add_dependencies(plugin_arp wxModularCore)  --  not needed
add_dependencies(main_program wxModularCore binary_hexer)
