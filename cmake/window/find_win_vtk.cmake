
find_package(VTK REQUIRED)
if (VTK_FOUND)
    include_directories(${CUSTOM_LIB_DIR}/vtk/include/vtk-9.2)
    include_directories(${CUSTOM_LIB_DIR}/vtk/include)
    message(STATUS "The vtk lib is found!")
else ()
    message(STATUS "The vtk lib is no found!")
endif ()
