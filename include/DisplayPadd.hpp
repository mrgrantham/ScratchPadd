#pragma once
#include <BaseWorker.hpp>
#include <spdlog/spdlog.h>

template <typename T>
class DisplayPadd : public BaseWorker {
public:
  virtual void prepare() override {
    spdlog::info("Preparing: {}", __CLASS_NAME__ );
    workerName_ = __CLASS_NAME__;
  }
  DisplayPadd() : BaseWorker() {
    spdlog::info("Constructing: {}",__CLASS_NAME__ );
  }
  virtual ~DisplayPadd() {
    spdlog::info("Destroying: {}", __CLASS_NAME__);
  }

};