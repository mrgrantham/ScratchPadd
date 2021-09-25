#pragma once

#include <string>
#include <ScratchPadd/Message.hpp>

class Graphics {
  public:
  class View {
    public:
    virtual void draw()=0;
    virtual void destroy()=0;
    virtual void reset()=0;
    virtual void setup(const std::string &&name)=0;
    virtual ~View() {}
  };

  class ControlView : public View {
    protected:
    ScratchPadd::MessageType::Control controls_;
    std::vector<std::string> orderedControlVector_;
    void buildControls(ScratchPadd::MessageType::Control &controls);
    public:
    void setControls(ScratchPadd::MessageType::Control &controls);
  };

  class ViewCollection : View {
    public:
    virtual void draw(){}
    virtual void destroy(){}
    virtual void reset(){}
    virtual void setup(const std::string &&name){}
  };

  class FrameBuffer {
    public:
    virtual void create(int32_t width, int32_t height)=0;
    virtual void destroy()=0;
    virtual uint32_t get()=0;
    virtual void bind()=0;
    virtual void unbind()=0;
    virtual ~FrameBuffer(){}
  };
  struct VertexPair {
    float position[3];
    float normal[3];
  };
  class VertexIndexBuffer {
    public:
    virtual void create(const std::vector<VertexPair>& vertices, const std::vector<uint32_t>& indices)=0;
    virtual void destroy()=0;
    virtual void draw()=0;
    virtual void bind()=0;
    virtual void unbind()=0;
    virtual ~VertexIndexBuffer() {}
  };

  virtual void setupWindow()=0;
  virtual void addView(std::unique_ptr<Graphics::View> view)=0;
  virtual bool draw()=0;
  virtual void tearDown()=0;
  virtual ~Graphics(){};
};

#if defined(SCRATCHPADD_GL)
#include <ScratchPadd/Graphics/GL/GL.hpp>
#elif defined(SCRATCHPADD_METAL)
#include <ScratchPadd/Graphics/Metal.hpp>
#elif defined(SCRATCHPADD_VULKAN)
#include <ScratchPadd/Graphics/Vulkan.hpp>
#else
#error "No Graphics Backend Selected"
#endif
