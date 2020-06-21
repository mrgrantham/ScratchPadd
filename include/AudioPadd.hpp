#pragma once
#include <Base.hpp>
#include <spdlog/spdlog.h>

class AudioPadd : public ScratchPadd::Base {
  public:
  virtual void prepare()  {
    spdlog::info("Preparing: {}",__CLASS_NAME__);
    paddName_ = __CLASS_NAME__;
  }

  AudioPadd() : Base() {
    spdlog::info("Constructing: {}",__CLASS_NAME__);
  }
  virtual ~AudioPadd() {
    spdlog::info("Destroying: {}", __CLASS_NAME__);
  }
};