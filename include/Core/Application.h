//
// Created by alex on 1/13/23.
//

#ifndef VKEXPERIMENT_APPLICATION_H
#define VKEXPERIMENT_APPLICATION_H

#include <memory>
#include <vector>

#include "VulkanMaster.h"

#include "Window.h"
#include "Pipeline.h"
#include "Device.h"
#include "SwapChain.hpp"
#include "BufferModel.h"

namespace VKE {

  class Application {
  public:
    Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    ~Application();

    void Run();

  private:
    void LoadModels();
    void CreatePipelineLayout();
    void CreatePipeline();
    void CreateCommandBuffers();
    void DrawFrame();

  private:

    static constexpr uint32_t WIDTH{800};
    static constexpr uint32_t HEIGHT{600};

    Window                       m_Window;
    Device                       m_Device{m_Window};
    SwapChain                    m_SwapChain{m_Device, m_Window.WindowExtent()};
    std::unique_ptr<Pipeline>    m_Pipeline;
    VkPipelineLayout             m_PipelineLayout;
    std::vector<VkCommandBuffer> m_CommandBuffers;
    std::unique_ptr<BufferModel> m_VertexBufferModel;
  };

} // VKE

#endif //VKEXPERIMENT_APPLICATION_H
