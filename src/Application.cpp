//
// Created by alex on 1/13/23.
//

#include "Core/Application.h"

namespace VKE {
  Application::Application() : m_Window("Hello Vulkan", WIDTH, HEIGHT) {
    CreatePipelineLayout();
    CreatePipeline();
    CreateCommandBuffers();
  }

  Application::~Application() {
    vkDestroyPipelineLayout(m_Device.DeviceHandle(), m_PipelineLayout, nullptr);
  }

  void Application::Run() {
    while (!m_Window.ShouldClose()) {
      glfwPollEvents();
      DrawFrame();
    }
  }

  void Application::CreatePipelineLayout() {
    VkPipelineLayoutCreateInfo layoutInfo{};

    layoutInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutInfo.setLayoutCount         = 0;
    layoutInfo.pSetLayouts            = nullptr;
    layoutInfo.pushConstantRangeCount = 0;
    layoutInfo.pPushConstantRanges    = nullptr;

    if (vkCreatePipelineLayout(m_Device.DeviceHandle(), &layoutInfo, nullptr, &m_PipelineLayout)
        != VK_SUCCESS) {
      throw std::runtime_error("failed to create a graphics pipeline layout");
    }
  }

  void Application::CreatePipeline() {
    auto pipelineConfig = Pipeline::DefaultPipelineConfigInfo(
        m_SwapChain.Width(), m_SwapChain.Height());

    pipelineConfig.RenderPass     = m_SwapChain.GetRenderPass();
    pipelineConfig.PipelineLayout = m_PipelineLayout;

    m_Pipeline = std::make_unique<Pipeline>(
        m_Device,
        "assets/shaders/simple_shader.vert.spv", "assets/shaders/simple_shader.frag.spv",
        pipelineConfig
    );
  }

  void Application::CreateCommandBuffers() {
    m_CommandBuffers.resize(m_SwapChain.ImageCount());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool        = m_Device.GetCommandPool();
    allocInfo.commandBufferCount = m_CommandBuffers.size();

    if (vkAllocateCommandBuffers(m_Device.DeviceHandle(), &allocInfo, m_CommandBuffers.data())
        != VK_SUCCESS) {
      throw std::runtime_error("failed to create command buffers");
    }

    for (int i{0}; i < m_CommandBuffers.size(); i++) {
      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      if (vkBeginCommandBuffer(m_CommandBuffers[i], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer");
      }

      VkRenderPassBeginInfo renderPassBeginInfo{};
      renderPassBeginInfo.sType       = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      renderPassBeginInfo.renderPass  = m_SwapChain.GetRenderPass();
      renderPassBeginInfo.framebuffer = m_SwapChain.GetFrameBuffer(i);

      renderPassBeginInfo.renderArea.offset = {0, 0};
      renderPassBeginInfo.renderArea.extent = m_SwapChain.GetSwapChainExtent();

      std::array<VkClearValue, 2> clearValues{};
      clearValues[0].color = {0.1f, 0.1f, 0.1f, 0.1f};
      clearValues[1].depthStencil = {0.1f, 0};

      renderPassBeginInfo.clearValueCount = clearValues.size();
      renderPassBeginInfo.pClearValues = clearValues.data();

      vkCmdBeginRenderPass(m_CommandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
      m_Pipeline->Bind(m_CommandBuffers[i]);
      vkCmdDraw(m_CommandBuffers[i], 3, 1, 0, 0);

      vkCmdEndRenderPass(m_CommandBuffers[i]);
      if(vkEndCommandBuffer(m_CommandBuffers[i]) != VK_SUCCESS) {
        throw std::runtime_error("failed to end recording command buffer");
      }
    }
  }

  void Application::DrawFrame() {
    uint32_t imageIndex;
    auto result = m_SwapChain.AcquireNextImage(&imageIndex);

    if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      throw std::runtime_error("failed to acquire swap chain image");
    }

    result = m_SwapChain.SubmitCommandBuffers(&m_CommandBuffers[imageIndex], &imageIndex);
    if(result != VK_SUCCESS) {
      throw std::runtime_error("failed to present swap chain image");
    }
  }
} // VKE
