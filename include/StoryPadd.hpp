#pragma once
#include <ScratchPadd/Base.hpp>
#include <spdlog/spdlog.h>

class StoryPadd : public ScratchPadd::Base {
  public:
  virtual void prepare() override {
    spdlog::info("Preparing: {}",__CLASS_NAME__);
    paddName_ = __CLASS_NAME__;
    setRepeatInterval(1000);
  }
  StoryPadd(ScratchPadd::System *system)  : Base(system){
    spdlog::info("Constructing: {}", __CLASS_NAME__);
  }

  virtual void repeat() override {
    spdlog::info("Repeating: {}",__CLASS_NAME__);
  }

  virtual ~StoryPadd() {
    spdlog::info("Destroying: {}", __CLASS_NAME__ );
  }

  virtual void starting() override {
    ScratchPadd::Message_Type::Triangle triangle;
    send(ScratchPadd::Make_Msg(ScratchPadd::Message_Type::Triangle()));
  }

  virtual void receive(ScratchPadd::Message message) override {
    ScratchPadd::MessageVariant &messageVariant = *message.get();
    std::visit(overload{
        [&](ScratchPadd::Message_Type::Triangle& message)       { std::cout << paddName_ << "Triangle: " << message <<"\n"; },
        [&](ScratchPadd::Message_Type::Point& message)   { std::cout << paddName_ << "Point: " << message << "\n"; },
        [&](ScratchPadd::Message_Type::Text& message)       { std::cout << paddName_ << "Text: " << message << "\n"; }
    }, messageVariant);
  }
};