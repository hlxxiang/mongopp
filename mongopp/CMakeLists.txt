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

#ADD_DEFINITIONS(-D_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)  使用C++17编译需要添加


if(MSVC)
    include_directories(
        ${PROJECT_SOURCE_DIR}/3rd/include
        ${PROJECT_SOURCE_DIR}/util
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
ADD_LIBRARY (
mongopp
${INC_LIST}
${CPP_LIST}
${INC_UTIL}
${CPP_UTIL}
)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
