cmake_minimum_required(VERSION 3.9)
cmake_policy(SET CMP0042 NEW)
set (CMAKE_CXX_STANDARD 11)

project (hello-world-native)
set(VERSION 1.0.0.0)
SET(CMAKE_CXX_FLAGS "-D_GLIBCXX_USE_CXX11_ABI=0")

if (MSVC)
    set(CMAKE_DEBUG_POSTFIX _d)
endif ()

# find_package(JThread REQUIRED)
# find_package(JRTPLIB REQUIRED)

set(NODE_LINK_LIBS "")
set(NODE_EXTERNAL_INCLUDES "")


FIND_PACKAGE(Boost REQUIRED)
IF (DEFINED BOOST_INCLUDEDIR)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
else()
    IF(MSVC)
        set(Boost_INCLUDE_DIR "C:/Users/wt32/boost")
    ENDIF()
ENDIF()


INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/iotdb/include)
message(${Boost_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

IF(MSVC)
    LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/iotdb/lib/Release)
ENDIF()
LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/iotdb/lib)

include_directories(${CMAKE_JS_INC})
# include_directories(${JTHREAD_INCLUDE_DIRS} ${JRTPLIB_INCLUDE_DIRS})

# file(GLOB HEADER_FILES 
# "src/index.h")

file(GLOB SOURCE_FILES 
"src/hello_world.cc")

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES} ${CMAKE_JS_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")


IF(MSVC)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${CMAKE_JS_LIB} iotdb_session thriftmd)
ELSE()
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${CMAKE_JS_LIB} iotdb_session pthread)
ENDIF()
# ${JTHREAD_LIBRARIES} ${JRTPLIB_LIBRARIES}

# Include Node-API wrappers
execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )
string(REGEX REPLACE "[\r\n\"]" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})

# define NPI_VERSION
add_definitions(-DNAPI_VERSION=8)
add_definitions(-DNAPI_DISABLE_CPP_EXCEPTIONS)