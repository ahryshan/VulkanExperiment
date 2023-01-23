//
// Created by alex on 1/13/23.
//

#include "Core/Window.h"
#include <stdexcept>

namespace VKE {
  Window::Window(const char* name, uint32_t width, uint32_t height) : m_Width{width},
                                                                      m_Height{height},
                                                                      m_WindowName(name) {
    InitWindow();
  }

  Window::~Window() {
    glfwDestroyWindow(m_WindowHandle);
    glfwTerminate();
  }

  void Window::InitWindow() {
    glfwInit();

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_WindowHandle = glfwCreateWindow((int)m_Width, (int)m_Height, m_WindowName.c_str(), nullptr, nullptr);
  }

  void Window::CreateSurface(VkInstance instance, VkSurfaceKHR* surface) {
    if (glfwCreateWindowSurface(instance, m_WindowHandle, nullptr, surface) != VK_SUCCESS)
      throw std::runtime_error("failed to create window Surface");
  }
} // VKE
