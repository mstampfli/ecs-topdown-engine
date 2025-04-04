#include "Window.h"
#include <iostream>

Window::Window() : window(nullptr), width(0), height(0) { }

Window::~Window() {
    close();
}

void Window::open(int w, int h, const char* title) {
    if (!glfwInit()){
        std::cerr << "GLFW initialization failed!" << std::endl;
        return;
    }
    window = glfwCreateWindow(w, h, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW window creation failed!" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    width = w;
    height = h;
}

int Window::initializeOpenGL() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return -1;
    }
    return 0;
}

void Window::run() {
    // Basic loop without calling any update function.
    while (!glfwWindowShouldClose(window)) {
        swapBuffers();
        glfwPollEvents();
    }
}

void Window::close() {
    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
        window = nullptr;
    }
}

void Window::swapBuffers() const {
    glfwSwapBuffers(window);
}

void Window::clearScreen() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::enableBlending() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

GLFWwindow* Window::getGLFWwindow() {
    return window;
}

int Window::getWidth() const {
    return width;
}

int Window::getHeight() const {
    return height;
}
