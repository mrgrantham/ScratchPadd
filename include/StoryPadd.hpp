#pragma once
#include <BaseWorker.hpp>
#include <spdlog/spdlog.h>

class StoryPadd : public BaseWorker {
  public:
  virtual void prepare() {
    spdlog::info("Preparing: {}",__CLASS_NAME__);
    workerName_ = __CLASS_NAME__;
  }
  StoryPadd()  : BaseWorker(){
    spdlog::info("Constructing: {}", __CLASS_NAME__);
  }
  virtual ~StoryPadd() {
    spdlog::info("Destroying: {}", __CLASS_NAME__ );
  }
};