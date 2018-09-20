# CMakeList.txt : 顶级 CMake 项目文件，执行全局配置
# 和此处的包含子项目。
#
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
cmake_minimum_required (VERSION 3.8)

project ("project")

function(group_by_dir src_dir)
    if(MSVC)
        foreach(FILE ${ARGN})
            # 获取文件绝对路径
            get_filename_component(FULL_NAME "${FILE}" ABSOLUTE)

            # 获取文件父路径
            get_filename_component(PARENT_DIR "${FULL_NAME}" PATH)

            # 移除父路径中的源码根路径
            string(REPLACE "${ARGV0}" "" GROUP "${PARENT_DIR}")

            # 确保路径使用windows路径符号
            string(REPLACE "/" "\\" GROUP "${GROUP}")

            # 将文件归组到 "Source Files" 和 "Header Files"

            if("${FILE}" MATCHES ".*\\.h")
              set(GROUP "${GROUP}")
            else()
              set(GROUP "${GROUP}")
            endif()

            source_group("${GROUP}" FILES "${FILE}")
        endforeach()
    endif(MSVC)
endfunction(group_by_dir)

# 包含子项目。
add_subdirectory ("mongopp")
add_subdirectory ("mongopp-test")