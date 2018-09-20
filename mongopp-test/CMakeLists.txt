# CMakeList.txt: engine 的 CMake 项目，包括源和定义
# 此处特定于项目的逻辑。
#
cmake_minimum_required (VERSION 3.8)

SET(CMAKE_C_COMPILER g++)
add_compile_options(-std=c++11)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)

if(MSVC)
    ADD_DEFINITIONS(-D_WIN32_WINNT=0x0A00 -DMONGO_CXX_DRIVER_COMPILING -DBSONCXX_EXPORT -DMONGOCXX_EXPORTS -DWIN32)
endif(MSVC)

#ADD_DEFINITIONS(-D_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)


if(MSVC)
    include_directories(
        ${PROJECT_SOURCE_DIR}/3rd/include
        ${PROJECT_SOURCE_DIR}/util
        ${PROJECT_SOURCE_DIR}/mongopp
    )
else()
    include_directories(
        /usr/local/include
        ${PROJECT_SOURCE_DIR}/util
    )
endif(MSVC)

aux_source_directory(../util UTIL)

file(GLOB_RECURSE INC_UTIL "../util/*.h")
file(GLOB_RECURSE CPP_UTIL "../util/*.cpp")
file(GLOB_RECURSE INC_LIST "*.h")
file(GLOB_RECURSE CPP_LIST "*.cpp")

group_by_dir(${PROJECT_SOURCE_DIR} ${INC_UTIL})
group_by_dir(${PROJECT_SOURCE_DIR} ${CPP_UTIL})
group_by_dir(${CMAKE_CURRENT_SOURCE_DIR} ${INC_LIST})
group_by_dir(${CMAKE_CURRENT_SOURCE_DIR} ${CPP_LIST})


# 将源添加到此项目的可执行文件。
add_executable (
mongopp-test
${INC_LIST}
${CPP_LIST}
${INC_UTIL}
${CPP_UTIL}
)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

if(WIN32)
    target_link_libraries(mongopp-test
        debug ${PROJECT_SOURCE_DIR}/3rd/lib/debug/mongocxx.lib 
        debug ${PROJECT_SOURCE_DIR}/3rd/lib/debug/bsoncxx.lib 
        debug ${PROJECT_SOURCE_DIR}/3rd/lib/debug/mongoc-1.0.lib 
        debug ${PROJECT_SOURCE_DIR}/3rd/lib/debug/bson-1.0.lib 
        debug ${PROJECT_SOURCE_DIR}/bin/debug/mongopp.lib 
    )
    
    target_link_libraries(mongopp-test 
        optimized ${PROJECT_SOURCE_DIR}/3rd/lib/release/mongocxx.lib 
        optimized ${PROJECT_SOURCE_DIR}/3rd/lib/release/bsoncxx.lib 
        optimized ${PROJECT_SOURCE_DIR}/3rd/lib/release/mongoc-1.0.lib 
        optimized ${PROJECT_SOURCE_DIR}/3rd/lib/release/bson-1.0.lib 
        optimized ${PROJECT_SOURCE_DIR}/bin/release/mongopp.lib 
    )
else()
    target_link_libraries(mongopp-test 
        optimized ${PROJECT_SOURCE_DIR}/bin/release/libmongopp.a 
        optimized /usr/local/lib/libmongocxx.so 
        optimized /usr/local/lib/libbsoncxx.so 
        optimized /usr/local/lib/libmongoc-1.0.so 
        optimized /usr/local/lib/libbson-1.0.so 
        optimized /usr/local/lib/libcrypto.so 
        optimized /usr/local/lib/libssl.so 
        optimized libpthread.so
        optimized libdl.so
    )
endif()
