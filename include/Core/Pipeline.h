//
// Created by alex on 1/14/23.
//

#ifndef VKEXPERIMENT_PIPELINE_H
#define VKEXPERIMENT_PIPELINE_H

#include <string>
#include <vector>

#include "Device.h"

namespace VKE {

  struct PipelineConfigInfo {
    VkViewport                             Viewport;
    VkRect2D                               Scissor;
    VkPipelineInputAssemblyStateCreateInfo InputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo RasterizationInfo;
    VkPipelineMultisampleStateCreateInfo   MultisampleInfo;
    VkPipelineColorBlendAttachmentState    ColorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo    ColorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo  DepthStencilInfo;
    VkPipelineLayout                       PipelineLayout{nullptr};
    VkRenderPass                           RenderPass{nullptr};
    uint32_t                               Subpass{0};
  };

  class Pipeline {
  public:
    Pipeline(
        Device& device, const std::string& vertexShaderFilepath,
        const std::string& fragmentShaderFilepath, const PipelineConfigInfo& configInfo
    );
    ~Pipeline();

    Pipeline(const Pipeline&) = delete;
    void operator=(const Pipeline&) = delete;

    void Bind(VkCommandBuffer commandBuffer);

    static PipelineConfigInfo DefaultPipelineConfigInfo(uint32_t width, uint32_t height);

  private:
    static std::vector<char> ReadFile(const std::string& filepath);
    void CreateGraphicsPipeline(
        const std::vector<char>& vertShaderCode, const std::vector<char>& fragShaderCode,
        const PipelineConfigInfo& configInfo
    );

    void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

  private:
    Device& m_Device;

    VkPipeline     m_PipelineHandle;
    VkShaderModule m_VertShaderModule;
    VkShaderModule m_FragShaderModule;
  };

} // VKE

#endif //VKEXPERIMENT_PIPELINE_H
