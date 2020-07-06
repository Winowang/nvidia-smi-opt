cmake_minimum_required(VERSION 3.0)

project(nvidia_opti)

option(CUDA_DYNAMIC "dynmaic load cuda lib" ON)
set(CMAKE_CXX_STANDARD 11)

find_package(CUDA REQUIRED)

include_directories(${CUDA_INCLUDE_DIRS})

aux_source_directory(src SRC_LIST)
aux_source_directory(src/system SRC_SYSTEM_LIST)

add_library(nvidia_opti SHARED ${SRC_LIST} ${SRC_SYSTEM_LIST})
set_target_properties(nvidia_opti PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(nvidia_opti PROPERTIES C_VISIBILITY_PRESET hidden)
set_target_properties(nvidia_opti PROPERTIES VISIBILITY_INLINES_HIDDEN ON)

if(CUDA_DYNAMIC)
    target_link_libraries(nvidia_opti nvidia-ml)
else()
    target_link_libraries(nvidia_opti ${CUDA_LIBRARIES} nvidia-ml)
endif()

