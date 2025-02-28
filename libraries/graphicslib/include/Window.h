#ifndef WINDOW_H
#define WINDOW_H

#include "../../glad/include/glad/glad.h"
#include "../../glfw/include/GLFW/glfw3.h"
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

    void enableBlending();
    void disableBlending();

    bool shouldClose() const;
    void swapBuffers() const;
    bool isKeyPressed(int key) const;

    void setFramerate(int newFramerate);

    void toOpenGLCoordinates(float& x, float& y);
    void toOpenGLSize(float& w, float& h);

    void setBackgroundColor(float r, float g, float b);
    void clearScreen();

    GLFWwindow* getGLFWwindow() {
        return window;
    }

    std::function<void(void)> updateFunction;
    

protected:
    GLFWwindow* window;
    int width, height;

    int framerate = 0;
    float timeInterval = 1.0f / framerate;
    float t1;
    float t2;
    float deltaTime;

    bool isActive = false;
    bool isRunning = false;
};

#endif
