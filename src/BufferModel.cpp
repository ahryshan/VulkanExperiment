//
// Created by alex on 1/23/23.
//

#include "Core/BufferModel.h"

namespace VKE {
  BufferModel::BufferModel(Device& device, const std::vector<Vertex>& vertices) : m_Device{device} {
    CreateVertexBuffers(vertices);
  }

  BufferModel::~BufferModel() {
    vkDestroyBuffer(m_Device.DeviceHandle(), m_VertexBuffer, nullptr);
    vkFreeMemory(m_Device.DeviceHandle(), m_VertexBufferMemory, nullptr);
  }

  void BufferModel::Bind(VkCommandBuffer commandBuffer) {
    VkBuffer buffers[]{m_VertexBuffer};
    VkDeviceSize offsets[]{0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
  }

  void BufferModel::Draw(VkCommandBuffer commandBuffer) {
    vkCmdDraw(commandBuffer, m_VertexCount, 1, 0, 0);
  }

  void BufferModel::CreateVertexBuffers(const std::vector<Vertex>& vertices) {
    m_VertexCount = vertices.size();
    assert(m_VertexCount >= 3 && "Vertex count must be at least 3");

    VkDeviceSize bufferSize = sizeof(vertices[0]) * m_VertexCount;
    m_Device.CreateBuffer(
        bufferSize,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        m_VertexBuffer,
        m_VertexBufferMemory
    );

    void* data;
    vkMapMemory(m_Device.DeviceHandle(), m_VertexBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), bufferSize);
    vkUnmapMemory(m_Device.DeviceHandle(), m_VertexBufferMemory);
  }

  std::vector<VkVertexInputBindingDescription> BufferModel::Vertex::BindingDescription() {
    std::vector<VkVertexInputBindingDescription> descriptions(1);
    descriptions[0].binding = 0;
    descriptions[0].stride = sizeof(Vertex);
    descriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return descriptions;
  }

  std::vector<VkVertexInputAttributeDescription> BufferModel::Vertex::AttributeDescription() {
    std::vector<VkVertexInputAttributeDescription> descriptions(2);

    descriptions[0].binding = 0;
    descriptions[0].location = 0;
    descriptions[0].offset = 0;
    descriptions[0].format = VK_FORMAT_R32G32_SFLOAT;

    descriptions[1].binding = 0;
    descriptions[1].location = 1;
    descriptions[1].offset = sizeof(glm::vec2);
    descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;

    return descriptions;
  }
}
