//
// Created by alex on 1/13/23.
//

#ifndef VKEXPERIMENT_WINDOW_H
#define VKEXPERIMENT_WINDOW_H

#include "VulkanMaster.h"

namespace VKE {

  class Window {
  public:
    Window(const char* name, uint32_t width, uint32_t height);
    Window(const Window&) = delete;
    Window(Window&) = delete;

    ~Window();

    [[nodiscard]] inline bool ShouldClose() const { return glfwWindowShouldClose(m_WindowHandle); }

  private:
    GLFWwindow* m_WindowHandle;
  };

} // VKE

#endif //VKEXPERIMENT_WINDOW_H
