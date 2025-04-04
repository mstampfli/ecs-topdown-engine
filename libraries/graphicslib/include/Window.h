#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
public:
    Window();
    ~Window();

    // Create a window with the specified dimensions and title.
    void open(int width, int height, const char* title);
    // Initialize OpenGL via GLAD.
    int initializeOpenGL();
    // Run a basic loop (if needed; you may handle the loop externally).
    void run();
    // Close and clean up the window.
    void close();

    // Swap buffers and clear the screen.
    void swapBuffers() const;
    void clearScreen();
    // Enable alpha blending.
    void enableBlending();

    // Retrieve the underlying GLFWwindow.
    GLFWwindow* getGLFWwindow();

    // Get window dimensions.
    int getWidth() const;
    int getHeight() const;

private:
    GLFWwindow* window;
    int width, height;
};

#endif
