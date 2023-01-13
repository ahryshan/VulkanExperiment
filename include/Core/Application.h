//
// Created by alex on 1/13/23.
//

#ifndef VKEXPERIMENT_APPLICATION_H
#define VKEXPERIMENT_APPLICATION_H

#include "Window.h"

namespace VKE {

  class Application {
  public:
    Application() : m_Window("Hello Vulkan", WIDTH, HEIGHT) {}

    void Run();

  private:
    static constexpr uint32_t WIDTH{800};
    static constexpr uint32_t HEIGHT{600};

    Window m_Window;
  };

} // VKE

#endif //VKEXPERIMENT_APPLICATION_H
