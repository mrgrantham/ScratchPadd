#pragma once

class GL_View : public Graphics::View {
  private:
  std::unique_ptr<Graphics::FrameBuffer> frameBuffer_;
  std::unique_ptr<Graphics::VertexIndexBuffer> vertexIndexBuffer_;
  GL_Shader shader_;
  ImVec2 size_;
  std::string name_{"Unnamed"};
  public:
  GL_View() : frameBuffer_(std::make_unique<GL_FrameBuffer>()),    vertexIndexBuffer_(std::make_unique<GL_VertexIndexBuffer>()){}
  void setup(const std::string&& name) override {
    name_ = name;
    frameBuffer_->create(800, 600);
    vertexIndexBuffer_->create(vertices, indices);
    // shader_.generate(vertexShader, fragmentShader);
    // shader_.generate("../include/ScratchPadd/Graphics/GL/Shaders/gl_view_vertex.shader", "../include/ScratchPadd/Graphics/GL/Shaders/gl_view_fragment.shader");
    shader_.generate("../include/ScratchPadd/Graphics/GL/Shaders/vertex.shader", "../include/ScratchPadd/Graphics/GL/Shaders/fragment.shader");

  }

  void draw() override {
    shader_.use();
    // light_->update(shader_.get());

    frameBuffer_->bind();

    // if (mMesh)
    // {
    //   mMesh->update(mShader.get());
    //   mMesh->render();
    // }
    shader_.update();
    vertexIndexBuffer_->draw();

    frameBuffer_->unbind();

    ImGui::Begin(name_.c_str());

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    size_ = { viewportPanelSize.x, viewportPanelSize.y };

    // mCamera->set_aspect(mSize.x / mSize.y);
    // mCamera->update(mShader.get());

    // add rendered texture to ImGUI scene window
    uint64_t textureID = frameBuffer_->get();
    ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ size_.x, size_.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

    ImGui::End();
  }

  void destroy() override {
    vertexIndexBuffer_->destroy();
  }

  virtual void reset() override {

  }
};




static std::unique_ptr<Graphics::View> ViewBuilder() {
  return std::make_unique<GL_View>();
}