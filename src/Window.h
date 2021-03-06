#include <GLFW/glfw3.h>

#include "gl/glew.h"

class Window
{
  public:
    Window(int width, int height, char* name);

    ~Window();

    // Window Functions
    int Window_intit(int width, int height, char* name);

    void Update_Window();

    int Window_shouldClose();

    void Window_destroy();

    float GetDeltaTime();

  protected:
    // Main Window
    GLFWwindow* window;

    // DeltaTime
    double last = 0.0;
    double now = 0.0;
    float delta = 1.0f;
};
