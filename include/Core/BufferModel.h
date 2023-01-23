//
// Created by alex on 1/23/23.
//

#ifndef VKEXPERIMENT_BUFFERMODEL_H
#define VKEXPERIMENT_BUFFERMODEL_H

#include <vector>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "VulkanMaster.h"

#include "Device.h"

namespace VKE {
  class BufferModel {
  public:
    struct Vertex {
      glm::vec2 Position;
      glm::vec3 Color;

      static std::vector<VkVertexInputBindingDescription> BindingDescription();
      static std::vector<VkVertexInputAttributeDescription> AttributeDescription();
    };

  public:
    BufferModel(Device& device, const std::vector<Vertex>& vertices);

    BufferModel(const BufferModel&) = delete;
    BufferModel& operator=(const BufferModel&) = delete;

    ~BufferModel();

    void Bind(VkCommandBuffer commandBuffer);
    void Draw(VkCommandBuffer commandBuffer);

  private:
    void CreateVertexBuffers(const std::vector<Vertex>& vertices);

  private:
    Device& m_Device;
    VkBuffer       m_VertexBuffer;
    VkDeviceMemory m_VertexBufferMemory;
    uint32_t       m_VertexCount;
  };
}

#endif //VKEXPERIMENT_BUFFERMODEL_H
