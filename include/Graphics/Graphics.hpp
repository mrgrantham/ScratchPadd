class Graphics {
  public:
  virtual void setupWindow()=0;
  virtual bool draw()=0;
  virtual void tearDown()=0;
  virtual ~Graphics(){};
};

#if defined(SCRATCHPADD_GL)
#include <Graphics/GL.hpp>
#elif defined(SCRATCHPADD_METAL)
#include <Graphics/Metal.hpp>
#elif defined(SCRATCHPADD_VULKAN)
#include <GraphicsVulkan.hpp>
#else
#error "No Graphics Backend Selected"
#endif
