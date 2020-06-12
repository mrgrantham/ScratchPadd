#include <iostream> 
#include <GLFW/glfw3.h>
#include <boost/program_options.hpp>
#include <WorkerSystem.hpp>
#include <Workers.hpp>
#include <signal.h> //  our new library 
volatile sig_atomic_t flag = 0;
void signal_handler(int sig);


Dispatcher dispatcher;
void testGLFW();

int main(int argc, char **argv) {
  std::cout << "Starting WorkerSystem" <<std::endl;
  signal(SIGINT, signal_handler); 

  std::cout << "TESTTESTTEST" << std::endl;
  testGLFW();
  // dispatcher.start();
  // dispatcher.join();
}

void signal_handler(int sig){ // can be called asynchronously
  std::cout << "shutting down" << std::endl;
  flag = 0; // set flag
  dispatcher.stop();
}

void testGLFW() {
  GLFWwindow *window;
  if (!glfwInit()) {
    std::cout << "Could not init GLFW!" << std::endl;
    exit(-1);
  }


  window = glfwCreateWindow(640,480, "Hello World", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    exit(-1);
  }

  glfwMakeContextCurrent(window);
  // can only get a printable version string for OpenGL
  // after a context was created and assigned
  char * version = (char*)glGetString(GL_VERSION);
  std::cout << "GL VERSION: " << version << std::endl;

  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f,-0.5f);
    glVertex2f(0.0f,0.5f);
    glVertex2f(0.5f,-0.5f);
    glEnd();
    glfwSwapBuffers(window);
    glfwPollEvents();



  }

  glfwTerminate();
}