# Copyright (C) 2022-2023 Jacob R. Green
# All Rights Reserved.


add_library(rndr
        src/rndr.cpp

        src/graphics_context.cpp
        src/render_surface.cpp

        # src/DeviceMemory.cpp
        src/buffer.cpp
        src/texture.cpp

        src/command_pool.cpp

        src/pipeline.cpp
        src/shader.cpp
        src/descriptor_set.cpp
        src/renderable.cpp
        src/render_pass.cpp
        src/renderer.cpp
        src/mesh.cpp
)

target_include_directories(rndr
    PUBLIC
        inc
)

target_link_libraries(rndr
    PUBLIC
        core

        vulkan
        Vulkan::Headers
        VulkanMemoryAllocator
        glfw
        glm
)

target_compile_definitions(rndr
    PUBLIC
        GLFW_EXPOSE_NATIVE_WIN32
        VK_USE_PLATFORM_WIN32_KHR
)

target_precompile_headers(rndr
    PUBLIC
        <muchcool/core.hpp>
        <vulkan/vulkan.hpp>
        <vk_mem_alloc.h>
        <glfw/glfw3.h>
        <glfw/glfw3native.h>
        <glm/glm.hpp>
        <glm/gtc/matrix_transform.hpp>
        <glm/gtx/matrix_transform_2d.hpp>
        <glm/gtx/transform.hpp>
)

install(TARGETS rndr)
