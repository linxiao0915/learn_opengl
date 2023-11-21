
#set(PCL_3RD_LIB_DIR "C:/Program Files/CppLibrary/pcl/3rdParty")
#
##添加第三方库
#include_directories(${PCL_3RD_LIB_DIR}/FLANN/include)
#include_directories(${PCL_3RD_LIB_DIR}/Qhull/include)
#link_directories(${PCL_3RD_LIB_DIR}/FLANN/lib)
#link_directories(${PCL_3RD_LIB_DIR}/Qhull/lib)
#
## 添加FLANN
#set(FLANN_INCLUDE_DIR ${PCL_3RD_LIB_DIR}/FLANN/include)
#set(FLANN_LIBRARY_DEBUG_SHARED ${PCL_3RD_LIB_DIR}/FLANN/lib/flann_cpp-gd.lib)
#set(FLANN_LIBRARY_DEBUG_STATIC ${PCL_3RD_LIB_DIR}/FLANN/lib/flann_cpp_s-gd.lib)
#set(FLANN_LIBRARY_SHARED ${PCL_3RD_LIB_DIR}/FLANN/lib/flann_cpp.lib)
#set(FLANN_LIBRARY_STATIC ${PCL_3RD_LIB_DIR}/FLANN/lib/flann_cpp_s.lib)
#

find_package(PCL REQUIRED)
if (PCL_FOUND)
    include_directories(${PCL_INCLUDE_DIRS})
    link_directories(${PCL_LIBRARY_DIRS})
    add_definitions(${PCL_DEFINITIONS})

    message(STATUS PCL_ROOT: ${PCL_ROOT})
    message(STATUS PCL_INCLUDE_DIRS: ${PCL_INCLUDE_DIRS})
    message(STATUS PCL_LIBRARY_DIRS: ${PCL_LIBRARY_DIRS})
    message(STATUS PCL_DEFINITIONS: ${PCL_DEFINITIONS})
    message(STATUS PCL_VERSION: ${PCL_VERSION})

    message(STATUS "The PCL lib is found! ")
else ()
    message(STATUS "The PCL lib is no found! ")
endif ()
