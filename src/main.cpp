#include <iostream> 
#include <GL/glew.h> // include before GLFW
#include <GLFW/glfw3.h>
#include <boost/program_options.hpp>
// #include <WorkerSystem.hpp>
// #include <Workers.hpp>
#include "spdlog/spdlog.h"
#include <signal.h> //  our new library 
volatile sig_atomic_t flag = 0;
void signal_handler(int sig);


// Dispatcher dispatcher;
void testGLFW();

int main(int argc, char **argv) {
  std::cout << "Starting WorkerSystem" <<std::endl;
  signal(SIGINT, signal_handler); 
  spdlog::info("Welcome to SCRATCHPAAD!");
  testGLFW();
  // dispatcher.start();
  // dispatcher.join();
}

void signal_handler(int sig){ // can be called asynchronously
  std::cout << "shutting down" << std::endl;
  flag = 0; // set flag
  // dispatcher.stop();
}

static unsigned int compileShader(unsigned int type, std::string &source) {
  unsigned int id = glCreateShader(type);
  const char* source_c_str = source.c_str();
  glShaderSource(id, 1, &source_c_str, nullptr);
  glCompileShader(id);

  int status;
  glGetShaderiv(id, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    int status_length = 0;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &status_length);
    auto message = std::make_unique<char[]>(status_length);
    glGetShaderInfoLog(id,status_length, &status_length, message.get());
    spdlog::error("FAILED TO COMPILE {} SHADER WITH ERROR:\n{}",(type == GL_VERTEX_SHADER) ? "VERTEX":"FRAGMENT",message.get());
    glDeleteShader(id);
    return 0;
  }
  return id;
}

static int generateShaders(std::string &vertexShader, std::string &fragmentShader) {
  unsigned int program = glCreateProgram();
  unsigned int v_shader = compileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int f_shader = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
  glAttachShader(program,v_shader);
  glAttachShader(program,f_shader);
  glLinkProgram(program);
  GLint link_ok;
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if (!link_ok) {
    int status_length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &status_length);
    auto message = std::make_unique<char[]>(status_length);
    glGetProgramInfoLog(program,status_length, &status_length, message.get());
		spdlog::error("Error in glLinkProgram: {}",message.get());
		return 0;
	}
  const char* position_attribute_name = "position";
  // glBindAttribLocation(program, 0, "position");
  GLint attribute_position;
	attribute_position = glGetAttribLocation(program, position_attribute_name);
  if (attribute_position == -1) {
		spdlog::error("Could not bind attribute '{}'",position_attribute_name);
	}
  glValidateProgram(program);
	glEnableVertexAttribArray(attribute_position);

  // now that the shaders are linked into a program
  // we can delete them

  glDeleteShader(v_shader);
  glDeleteShader(f_shader);
  return program;
}

void testGLFW() {
  GLFWwindow *window;
  if (!glfwInit()) {
    spdlog::error("Could not init GLFW!");
    exit(-1);
  }


  window = glfwCreateWindow(1366, 768, "Hello World", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    exit(-1);
  }

  glfwMakeContextCurrent(window);

  GLenum status = glewInit();
  if (status != GLEW_OK) {
      spdlog::error("Error: {}\n", glewGetErrorString(status));
  }
  // can only get a printable version string for OpenGL
  // after a context was created and assigned
  char * version = (char*)glGetString(GL_VERSION);
  const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
  spdlog::info("GL RENDERER: {}", renderer);
  spdlog::info("GL VERSION: {}", version);
  spdlog::info("Status: Using GLEW {}\n", glewGetString(GLEW_VERSION));

  float positions[] = {
      -0.5f,-0.5f,
       0.0f, 0.5f,
       0.5f,-0.5f,
  };

  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER,buffer);
  glBufferData(GL_ARRAY_BUFFER,6 * sizeof(float),positions,GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(float) * 2,0);
  std::string vertexShader =
    "#version 120\n"
    "\n"
    "attribute vec4 position;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   gl_Position = position;\n"
    "}\n";

  std::string fragmentShader =
    "#version 120\n"
    "\n"
    "\n"
    "void main()\n"
    "{\n"
    "   gl_FragColor =  vec4(1.0,0.5,0.5,0.5);\n"
    "}\n";

  unsigned int shaders = generateShaders(vertexShader, fragmentShader);
  glUseProgram(shaders);
  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES,0,3);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
}