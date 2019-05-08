
#include <iostream>
#include "glad/glad.h"
#include "game_utils.h"
#include "GLFW/glfw3.h"
#include "../input/key_input.h"
#include "../input/mouse_input.h"

namespace gu
{

Config config;
GLFWwindow *window = nullptr;

// size of the window:
// width & height are in screen coordinates, widthPixels & heightPixels are in pixels.
int width = 0, height = 0, widthPixels = 0, heightPixels = 0;

namespace
{
Screen *screen;

void APIENTRY glMessageCallback(GLenum source, GLenum type, GLuint id,
                            GLenum severity, GLsizei length,
                            const GLchar *msg, const void *data)
{
    std::cerr << "\n====== OpenGL Message. ID: " << id << " ======" << std::endl << msg << std::endl << "======================================\n\n";
}

bool resized = true;
int nextWidth, nextHeight, nextWidthPixels, nextHeightPixels;

void window_size_callback(GLFWwindow* window, int width, int height)
{
    nextWidth = width;
    nextHeight = height;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    nextWidthPixels = width;
    nextHeightPixels = height;
    resized = true;
}

void render(double deltaTime)
{
    if (screen)
        screen->render(deltaTime);
}

void onResize()
{
    if (screen)
        screen->onResize();
}


} // namespace

bool init(Config config_)
{
    config = config_;
    // Initialise GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

    window = glfwCreateWindow(config.width, config.height, config.title.c_str(), NULL, NULL);

    if (!window)
    {
        std::cerr << "Failed to open GLFW window.\n";
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize OpenGL context\n";
        return false;
    }

    if (config.printOpenGLMessages)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(glMessageCallback, NULL);
    }

    if (!config.vsync)
        glfwSwapInterval(0); // a way to disable vsync.

    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwGetWindowSize(window, &nextWidth, &nextHeight);
    glfwGetFramebufferSize(window, &nextWidthPixels, &nextHeightPixels);

    KeyInput::setInputWindow(window);
    MouseInput::setInputWindow(window);

    return true;
}

void run()
{
    double prevTime = glfwGetTime();
    int framesInSecond = 0;
    double remainingSecond = 1;

    do
    {
        double currTime = glfwGetTime();
        double deltaTime = currTime - prevTime;

        framesInSecond++;

        if (config.showFPSInTitleBar && (remainingSecond -= deltaTime) <= 0)
        {
            std::string fps = std::to_string(framesInSecond) + "fps";
            glfwSetWindowTitle(window, fps.c_str());
            framesInSecond = 0;
            remainingSecond = 1;
        }

        if (resized)
        {
            width = nextWidth;
            height = nextHeight;
            widthPixels = nextWidthPixels;
            heightPixels = nextHeightPixels;
            resized = false;
            glViewport(0, 0, nextWidthPixels, nextHeightPixels);
            onResize();
        }

        render(std::min(deltaTime, .1));
        KeyInput::update();
        MouseInput::update();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        prevTime = currTime;

    } // Check if the window was closed
    while (!glfwWindowShouldClose(window));

    // glfwTerminate();
}

void setScreen(Screen *newScreen)
{
    screen = newScreen;
}

} // namespace gu