#include <GL/glew.h>  // include before GLFW
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

float positions[] = {
     -0.5f, -0.5f, 
      0.0f, 0.5f, 
      0.5f, -0.5f
};

static void clearOpenGLErrors();
static void checkOpenGLErrors();
static unsigned int compileShader(unsigned int type, std::string &source);
static int generateShaders(std::string &vertexShader, std::string &fragmentShader);


class Graphics {
  public:
  virtual void setupWindow()=0;
  virtual bool draw()=0;
  virtual void tearDown()=0;
  virtual ~Graphics(){};
};

class GL2 : public Graphics {
 public:
  void setupWindow() override {}
  bool draw() override {return true;}
  void tearDown() override {}
};

static void glfw_error_callback(int error, const char* description)
{
    spdlog::error("Glfw Error {}: {}\n", error, description);
}

class GL4 : public Graphics {
  unsigned int shaders = 0;
  std::string vertexShader =
      "#version 330 core\n"
      "\n"
      "layout(location = 0) in vec4 position;\n"
      "\n"
      "void main()\n"
      "{\n"
      "   gl_Position = position;\n"
      "}\n";

  std::string fragmentShader =
      "#version 330 core\n"
      "\n"
      "layout(location = 0) out vec4 color;"
      "\n"
      "void main()\n"
      "{\n"
      "   color =  vec4(1.0,0.5,0.5,0.5);\n"
      "}\n";

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    // const char* glsl_version = "#version 330 core";
    const char* glsl_version = "#version 330 core";

 public:
  GLFWwindow *window;
  void setupWindow() override {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
      spdlog::error("Could not init GLFW!");
      exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(1366, 768, "ScratchPaad", nullptr, nullptr);
    if (!window) {
      spdlog::error("No window created!");
      glfwTerminate();
      exit(-1);
    }
    spdlog::info("Window Created!");
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    glewExperimental = GL_TRUE;
    GLenum status = glewInit();
    if (status != GLEW_OK) {
      spdlog::error("Error: {}\n", glewGetErrorString(status));
    }
    // can only get a printable version string for OpenGL
    // after a context was created and assigned
    char *version = (char *)glGetString(GL_VERSION);
    const GLubyte *renderer = glGetString(GL_RENDERER);  // get renderer string
    spdlog::info("GL RENDERER: {}", renderer);
    spdlog::info("GL VERSION: {}", version);
    spdlog::info("Status: Using GLEW {}\n", glewGetString(GLEW_VERSION));

    // glEnable(GL_DEPTH_TEST);
    GLuint buffer = 0;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    shaders = generateShaders(vertexShader, fragmentShader);
    glUseProgram(shaders);
  }

  bool draw() override {
    if (!glfwWindowShouldClose(window)) {
      spdlog::info("window on display");
      checkOpenGLErrors();
      glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

      // Rendering
      ImGui::Render();
      int display_w, display_h;
      glfwGetFramebufferSize(window, &display_w, &display_h);
      glViewport(0, 0, display_w, display_h);
      glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
      glClear(GL_COLOR_BUFFER_BIT);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      glfwSwapBuffers(window);
      return true;
    }
    return false;
  }

  void tearDown() override {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteProgram(shaders);
    glfwDestroyWindow(window);
    glfwTerminate();
  }
};

static void clearOpenGLErrors() {
  while (glGetError() != GL_NO_ERROR) {
  }
}

static void checkOpenGLErrors() {
  while (GLenum error = glGetError()) {
    spdlog::error("OPENGL ERROR: (0x{0:x})", error);
  }
}

unsigned int compileShader(unsigned int type, std::string &source) {
  unsigned int id = glCreateShader(type);
  const char *source_c_str = source.c_str();
  glShaderSource(id, 1, &source_c_str, nullptr);
  glCompileShader(id);

  int status;
  glGetShaderiv(id, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    int status_length = 0;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &status_length);
    auto message = std::make_unique<char[]>(status_length);
    glGetShaderInfoLog(id, status_length, &status_length, message.get());
    spdlog::error("FAILED TO COMPILE {} SHADER WITH ERROR:\n{}",
                  (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT",
                  message.get());
    glDeleteShader(id);
    return 0;
  }
  return id;
}

static int generateShaders(std::string &vertexShader,
                           std::string &fragmentShader) {
  unsigned int program = glCreateProgram();
  unsigned int v_shader = compileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int f_shader = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
  glAttachShader(program, v_shader);
  glAttachShader(program, f_shader);
  glLinkProgram(program);
  GLint link_ok;
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if (!link_ok) {
    int status_length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &status_length);
    auto message = std::make_unique<char[]>(status_length);
    glGetProgramInfoLog(program, status_length, &status_length, message.get());
    spdlog::error("Error in glLinkProgram: {}", message.get());
    return 0;
  }
  glValidateProgram(program);

  // now that the shaders are linked into a program
  // we can delete them
  glDeleteShader(v_shader);
  glDeleteShader(f_shader);
  return program;
}
