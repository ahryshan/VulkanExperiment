//
// Created by alex on 1/13/23.
//

#include "Core/Application.h"

namespace VKE {
  void Application::Run() {
    while(!m_Window.ShouldClose()) {
      glfwPollEvents();
    }
  }
} // VKE
