//
// Created by alex on 1/14/23.
//

#include "Core/Pipeline.h"

#include <fstream>
#include <iostream>

#include "Core/BufferModel.h"

namespace VKE {
  Pipeline::Pipeline(
      Device& device, const std::string& vertexShaderFilepath,
      const std::string& fragmentShaderFilepath, const PipelineConfigInfo& configInfo
  ) : m_Device{device} {
    auto vertCode = ReadFile(vertexShaderFilepath);
    auto fragCode = ReadFile(fragmentShaderFilepath);

    CreateGraphicsPipeline(vertCode, fragCode, configInfo);
  }


  std::vector<char> Pipeline::ReadFile(const std::string& filepath) {
    std::ifstream file{filepath, std::ios::ate | std::ios::binary};
    if (!file.is_open()) {
      throw std::runtime_error{"failed to open file: " + filepath};
    }

    size_t            fileSize = file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();
    return buffer;
  }

  void Pipeline::CreateGraphicsPipeline(
      const std::vector<char>& vertShaderCode, const std::vector<char>& fragShaderCode,
      const PipelineConfigInfo& configInfo
  ) {
    assert(configInfo.PipelineLayout != VK_NULL_HANDLE
           && "Cannot create graphics pipeline without correct layout");
    assert(configInfo.RenderPass != VK_NULL_HANDLE
           && "Cannot create graphics pipeline without correct render pass");

    CreateShaderModule(vertShaderCode, &m_VertShaderModule);
    CreateShaderModule(fragShaderCode, &m_FragShaderModule);

    VkPipelineShaderStageCreateInfo shaderStages[2];

    shaderStages[0].sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage               = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module              = m_VertShaderModule;
    shaderStages[0].pName               = "main";
    shaderStages[0].flags               = 0;
    shaderStages[0].pNext               = nullptr;
    shaderStages[0].pSpecializationInfo = nullptr;

    shaderStages[1].sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage               = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].module              = m_FragShaderModule;
    shaderStages[1].pName               = "main";
    shaderStages[1].flags               = 0;
    shaderStages[1].pNext               = nullptr;
    shaderStages[1].pSpecializationInfo = nullptr;

    auto bindingDescriptions   = BufferModel::Vertex::BindingDescription();
    auto attributeDescriptions = BufferModel::Vertex::AttributeDescription();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
    vertexInputInfo.vertexBindingDescriptionCount   = bindingDescriptions.size();
    vertexInputInfo.pVertexAttributeDescriptions    = attributeDescriptions.data();
    vertexInputInfo.pVertexBindingDescriptions      = bindingDescriptions.data();

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType             = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount        = 2;
    pipelineInfo.pStages           = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;

    VkPipelineViewportStateCreateInfo viewportStateInfo{};
    viewportStateInfo.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateInfo.viewportCount = 1;
    viewportStateInfo.pViewports    = &configInfo.Viewport;
    viewportStateInfo.scissorCount  = 1;
    viewportStateInfo.pScissors     = &configInfo.Scissor;

    pipelineInfo.pViewportState = &viewportStateInfo;

    pipelineInfo.pRasterizationState = &configInfo.RasterizationInfo;
    pipelineInfo.pColorBlendState    = &configInfo.ColorBlendInfo;
    pipelineInfo.pDepthStencilState  = &configInfo.DepthStencilInfo;
    pipelineInfo.pMultisampleState   = &configInfo.MultisampleInfo;
    pipelineInfo.pInputAssemblyState = &configInfo.InputAssemblyInfo;

    pipelineInfo.pDynamicState = nullptr;

    pipelineInfo.layout     = configInfo.PipelineLayout;
    pipelineInfo.renderPass = configInfo.RenderPass;
    pipelineInfo.subpass    = configInfo.Subpass;

    pipelineInfo.basePipelineIndex  = -1;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(
        m_Device.DeviceHandle(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_PipelineHandle
    ) != VK_SUCCESS) {
      throw std::runtime_error("failed to create graphics pipeline");
    }
  }

  Pipeline::~Pipeline() {
    vkDestroyShaderModule(m_Device.DeviceHandle(), m_VertShaderModule, nullptr);
    vkDestroyShaderModule(m_Device.DeviceHandle(), m_FragShaderModule, nullptr);
    vkDestroyPipeline(m_Device.DeviceHandle(), m_PipelineHandle, nullptr);
  }

  PipelineConfigInfo Pipeline::DefaultPipelineConfigInfo(uint32_t width, uint32_t height) {
    PipelineConfigInfo configInfo{};

    configInfo.InputAssemblyInfo
              .sType                                    = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    configInfo.InputAssemblyInfo.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    configInfo.InputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    configInfo.Viewport.x        = 0.0f;
    configInfo.Viewport.y        = 0.0f;
    configInfo.Viewport.width    = static_cast<float>(width);
    configInfo.Viewport.height   = static_cast<float>(height);
    configInfo.Viewport.minDepth = 0.0f;
    configInfo.Viewport.maxDepth = 1.0f;

    configInfo.Scissor.offset = {0, 0};
    configInfo.Scissor.extent = {width, height};

    configInfo.RasterizationInfo.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    configInfo.RasterizationInfo.depthClampEnable        = VK_FALSE;
    configInfo.RasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    configInfo.RasterizationInfo.polygonMode             = VK_POLYGON_MODE_FILL;
    configInfo.RasterizationInfo.lineWidth               = 1.0f;
    configInfo.RasterizationInfo.cullMode                = VK_CULL_MODE_NONE;
    configInfo.RasterizationInfo.frontFace               = VK_FRONT_FACE_CLOCKWISE;
    configInfo.RasterizationInfo.depthBiasEnable         = VK_FALSE;
    configInfo.RasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
    configInfo.RasterizationInfo.depthBiasClamp          = 0.0f;           // Optional
    configInfo.RasterizationInfo.depthBiasSlopeFactor    = 0.0f;     // Optional

    configInfo.MultisampleInfo.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    configInfo.MultisampleInfo.sampleShadingEnable   = VK_FALSE;
    configInfo.MultisampleInfo.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
    configInfo.MultisampleInfo.minSampleShading      = 1.0f;           // Optional
    configInfo.MultisampleInfo.pSampleMask           = nullptr;             // Optional
    configInfo.MultisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
    configInfo.MultisampleInfo.alphaToOneEnable      = VK_FALSE;       // Optional

    configInfo.ColorBlendAttachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT;
    configInfo.ColorBlendAttachment.blendEnable         = VK_FALSE;
    configInfo.ColorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
    configInfo.ColorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
    configInfo.ColorBlendAttachment.colorBlendOp        = VK_BLEND_OP_ADD;              // Optional
    configInfo.ColorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
    configInfo.ColorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
    configInfo.ColorBlendAttachment.alphaBlendOp        = VK_BLEND_OP_ADD;              // Optional

    configInfo.ColorBlendInfo.sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    configInfo.ColorBlendInfo.logicOpEnable   = VK_FALSE;
    configInfo.ColorBlendInfo.logicOp         = VK_LOGIC_OP_COPY;  // Optional
    configInfo.ColorBlendInfo.attachmentCount = 1;
    configInfo.ColorBlendInfo.pAttachments    = &configInfo.ColorBlendAttachment;
    configInfo.ColorBlendInfo.blendConstants[0] = 0.0f;  // Optional
    configInfo.ColorBlendInfo.blendConstants[1] = 0.0f;  // Optional
    configInfo.ColorBlendInfo.blendConstants[2] = 0.0f;  // Optional
    configInfo.ColorBlendInfo.blendConstants[3] = 0.0f;  // Optional

    configInfo.DepthStencilInfo.sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    configInfo.DepthStencilInfo.depthTestEnable       = VK_TRUE;
    configInfo.DepthStencilInfo.depthWriteEnable      = VK_TRUE;
    configInfo.DepthStencilInfo.depthCompareOp        = VK_COMPARE_OP_LESS;
    configInfo.DepthStencilInfo.depthBoundsTestEnable = VK_FALSE;
    configInfo.DepthStencilInfo.minDepthBounds        = 0.0f;  // Optional
    configInfo.DepthStencilInfo.maxDepthBounds        = 1.0f;  // Optional
    configInfo.DepthStencilInfo.stencilTestEnable     = VK_FALSE;
    configInfo.DepthStencilInfo.front                 = {};  // Optional
    configInfo.DepthStencilInfo.back                  = {};   // Optional

    return configInfo;
  }

  void Pipeline::CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) {
    VkShaderModuleCreateInfo createInfo{};

    createInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode    = reinterpret_cast<const uint32_t*>(code.data());

    if (vkCreateShaderModule(m_Device.DeviceHandle(), &createInfo, nullptr, shaderModule)
        != VK_SUCCESS)
      throw std::runtime_error("failed to create shader module");
  }

  void Pipeline::Bind(VkCommandBuffer commandBuffer) {
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineHandle);
  }
} // VKE
