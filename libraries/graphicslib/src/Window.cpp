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
                swapBuffers();
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

    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);  // Closer fragments replace farther ones

    return 0;
}

void Window::enableBlending(){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Window::disableBlending(){
    glDisable(GL_BLEND);
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

void Window::toOpenGLCoordinates(float& x, float& y) {
    // Convert from window coordinates to OpenGL coordinates
    x = x / width * 2.0f - 1.0f;
    y = -y / height * 2.0f + 1.0f;
}

void Window::toOpenGLSize(float& w, float& h) {
    // Convert from window size to OpenGL size
    w = w / width * 2.0f;
    h = h / height * 2.0f;
}

void Window::setBackgroundColor(float r, float g, float b){
    glClearColor(r, g, b, 1.0f);
}

void Window::clearScreen(){
    glClear(GL_COLOR_BUFFER_BIT);
}