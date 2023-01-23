cmake_minimum_required(VERSION 3.23)
project(VkExperiment)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE _VKE_SRCS src/*.cpp include/*.h assets/shaders/*.vert assets/shaders/*.frag)

add_executable(VkExperiment ${_VKE_SRCS})

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glm)

target_link_libraries(VkExperiment PUBLIC glfw vulkan glm)

target_include_directories(VkExperiment PUBLIC include vendor/glfw/include vendor/glm)

find_package(Vulkan COMPONENTS glslc REQUIRED)
find_program(glslc_executable NAMES glslc HINTS Vulkan::glslc REQUIRED)

file(GLOB _SHADERS
        ${CMAKE_SOURCE_DIR}/assets/shaders/*.vert
        ${CMAKE_SOURCE_DIR}/assets/shaders/*.frag
        )

set(SHADER_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/assets/shaders)
add_custom_command(OUTPUT ${SHADER_OUTPUT_DIR}
        COMMAND mkdir -p ${SHADER_OUTPUT_DIR})

foreach (shader ${_SHADERS})
    get_filename_component(shader_filename ${shader} NAME)
    list(APPEND _SHADERS_OUTPUT ${shader_filename}.spv)
    add_custom_command(OUTPUT ${shader_filename}.spv
            COMMAND ${glslc_executable} ${shader} -o ${CMAKE_BINARY_DIR}/assets/shaders/${shader_filename}.spv
            COMMENT "Rebuilding ${shader_filename}.spv"
            DEPENDS ${SHADER_OUTPUT_DIR})
    message(STATUS "Generating build commands for ${shader_filename}")
endforeach ()

add_custom_target(VkExperiment_shaders ALL DEPENDS ${_SHADERS_OUTPUT})

add_dependencies(VkExperiment VkExperiment_shaders)

if(${CMAKE_BUILD_TYPE} STREQUAL "DEBUG")
    target_compile_definitions(VkExperiment PUBLIC DEBUG)
else()
    target_compile_definitions(VkExperiment PUBLIC NDEBUG)
endif()
