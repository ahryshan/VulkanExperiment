//
// Created by alex on 1/13/23.
//

#include "Core/Window.h"

namespace VKE {
  Window::Window(const char* name, uint32_t width, uint32_t height) {
    glfwInit();

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    glfwCreateWindow((int)width, (int)height, name, nullptr, nullptr);
  }

  Window::~Window() {
    glfwDestroyWindow(m_WindowHandle);
    glfwTerminate();
  }
} // VKE
