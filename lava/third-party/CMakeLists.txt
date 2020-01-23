cmake_minimum_required(VERSION 3.14)

project(third_party LANGUAGES C CXX)

# glm
add_library(glm INTERFACE)
set(GLM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glm)
target_sources(glm INTERFACE ${GLM_DIR}/glm/glm.hpp)
target_include_directories(glm INTERFACE ${GLM_DIR})

target_compile_definitions(glm INTERFACE
        GLM_FORCE_SWIZZLE
        GLM_FORCE_RADIANS
        GLM_FORCE_DEPTH_ZERO_TO_ONE
        GLM_ENABLE_EXPERIMENTAL
        GLM_GTX_random
        )
if(NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_definitions(glm INTERFACE GLM_FORCE_CXX14)
endif()

# vulkan
find_package(Vulkan REQUIRED)

# vma
add_library(vma INTERFACE)
set(VMA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/VulkanMemoryAllocator-Hpp)
target_sources(vma INTERFACE ${VMA_DIR}/vk_mem_alloc.h ${VMA_DIR}/vk_mem_alloc.hpp)
target_include_directories(vma INTERFACE ${VMA_DIR})
target_link_libraries(vma INTERFACE Vulkan::Vulkan)

# stb
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/stb")

# tinyobj
add_library(tinyobj INTERFACE)
set(TINYOBJ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tinyobjloader)
target_sources(tinyobj INTERFACE ${TINYOBJ_DIR}/tiny_obj_loader.h)
target_include_directories(tinyobj INTERFACE ${TINYOBJ_DIR})

## gli
add_library(gli INTERFACE)
set(GLI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/gli)
target_sources(gli INTERFACE ${GLI_DIR}/gli/gli.hpp)
target_include_directories(gli INTERFACE ${GLI_DIR})

# GLFW
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_INSTALL OFF)
add_subdirectory(glfw)
set_property(TARGET glfw PROPERTY FOLDER "ThirdParty")