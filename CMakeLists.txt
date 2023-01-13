cmake_minimum_required(VERSION 3.23)
project(VkExperiment)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE _VKE_SRCS src/*.cpp include/*.h)

add_executable(VkExperiment ${_VKE_SRCS})

add_subdirectory(vendor/glfw)

target_link_libraries(VkExperiment PUBLIC glfw vulkan)

target_include_directories(VkExperiment PUBLIC include vendor/glfw/include)
