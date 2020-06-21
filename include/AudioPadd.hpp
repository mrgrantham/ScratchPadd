#pragma once
#include <BaseWorker.hpp>
#include <spdlog/spdlog.h>

class AudioPadd : public ScratchPadd::BaseWorker {
  public:
  virtual void prepare()  {
    spdlog::info("Preparing: {}",__CLASS_NAME__);
    workerName_ = __CLASS_NAME__;
  }

  AudioPadd() : BaseWorker() {
    spdlog::info("Constructing: {}",__CLASS_NAME__);
  }
  virtual ~AudioPadd() {
    spdlog::info("Destroying: {}", __CLASS_NAME__);
  }
};