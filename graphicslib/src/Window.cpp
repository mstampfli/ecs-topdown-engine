#include "Window.h"

Window::Window() {
    //Handled by manual open
}

Window::~Window() {
    //Handled by manual close
}

void Window::run() {
    if(isActive && !glfwWindowShouldClose(window)){
        isRunning = true;
    }
    if(!isVsyncEnabled && framerate > 0){
        while(isActive && isRunning && !glfwWindowShouldClose(window)){
            t2 = glfwGetTime();
            deltaTime = (t2 - t1) * timeScale;
            if(deltaTime >= timeInterval){
                t1 = t2;
                
                updateFunction();
                glfwSwapBuffers(window);
            }
            //Waiting to reduce load
            //glfwWaitEvents();
            glfwWaitEventsTimeout(std::max(timeInterval - deltaTime, 0.0f)); //Avoid negative Waiting times if lagging behind

            glfwPollEvents();
        }
    } else {
         while(isActive && isRunning && !glfwWindowShouldClose(window)){
            updateFunction();
            glfwPollEvents();
        }
    }
    isRunning = false;
}

void Window::stop() {
    isRunning = false;
}

void Window::close() {
    glfwDestroyWindow(window);
    glfwTerminate();
    isActive = false;
}

void Window::open(int w, int h, const char* title) {
    if (!glfwInit()) {
        std::cerr << "GLFW Initialization failed!" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_RESIZABLE, false); //When resized needs to adjust width and height accordingly
    //glfwGetWindowSize(&width, &height);
    //glfwSetWindowPos(width / 2, height / 2);

    window = glfwCreateWindow(w, h, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW Window creation failed!" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    if(isVsyncEnabled){
        glfwSwapInterval(1); // Enable VSync
    } else {
        glfwSwapInterval(0); // Disable VSync
    }
    isActive = true;

    width = w; //Captures current Screen Size in window class
    height = h;

    t1 = glfwGetTime();
    t2 = t1;
}

int Window::initializeOpenGL(){
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return -1;//exit(EXIT_FAILURE);
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    return 0;
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void Window::swapBuffers() const {
    glfwSwapBuffers(window);
}

bool Window::isKeyPressed(int key) const {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

void Window::setFramerate(int newFramerate) {
    framerate = newFramerate;
    timeInterval = 1.0f / framerate;
}

float* Window::toOpenGLCoordinates(float x, float y) {
    // Convert from window coordinates to OpenGL coordinates
    float* pos = new float[2];
    pos[0] = x / width * 2 - 1.0f;
    pos[1] = -y / height * 2 + 1.0f;

    std::cout << pos[0] << ", " << pos[1] << std::endl;
    return pos;
}

float* Window::toOpenGLSize(float x, float y) {
    // Convert from window size to OpenGL size
    float* pos = new float[2];
    pos[0] = x / width * 2;
    pos[1] = y / height * 2;

    std::cout << pos[0] << ", " << pos[1] << std::endl;
    return pos;
}