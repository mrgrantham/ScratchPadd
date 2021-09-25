#pragma once

class GL_ControlView : public Graphics::ControlView {
  
  public:
  void draw() override {
    ImGui::Begin(controls_.sourceName.c_str());
    buildControls(controls_);
    ImGui::End();
  }

  void destroy() override {

  }

  void reset() override {

  }

  void setup(const std::string &&name) override {

  }

};

static std::unique_ptr<Graphics::ControlView> ControlViewBuilder() {
  return std::make_unique<GL_ControlView>();
}