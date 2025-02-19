#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>

class Window {
public:
    bool isVsyncEnabled = false;
    float timeScale = 1;

    Window(void);
    ~Window();

    void run();
    void stop();
    void close();
    void open(int width, int height, const char* title);
    int initializeOpenGL();

    bool shouldClose() const;
    void swapBuffers() const;
    bool isKeyPressed(int key) const;

    void setFramerate(int newFramerate);

    float* toOpenGLCoordinates(float x, float y);
    float* toOpenGLSize(float x, float y);

    std::function<void(void)> updateFunction;

private:
    int width, height;

    GLFWwindow* window;
    int framerate = 0;
    float timeInterval = 1.0f / framerate;
    float t1;
    float t2;
    float deltaTime;

    bool isActive = false;
    bool isRunning = false;
};

#endif
