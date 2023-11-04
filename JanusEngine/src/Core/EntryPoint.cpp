#include "Core/Core.h"
#include "Core/EntryPoint.h"
#include "Core/Assertion.h"
#include "GLFW/glfw3.h"
namespace JanusEngine
{
    int entryPoint(int argc, const char **argv)
    {
        Log::registerLogGroup({"CORE", "%^[%T] %n: %v%$"});
        spdlog::set_level(spdlog::level::debug);
        // Engine Loop

        int success = glfwInit();
        JN_ASSERT("CORE", success, "WINDOW_ERROR: Could not initialize GLFW.");

        GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
        JN_DEBUG("CORE", "Window creation error: {}", window == NULL);
        while(true) {

        }
        Log::deregisterLogGroup("CORE");
        return 0;
    }
}
