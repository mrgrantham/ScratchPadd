#include <iostream> 
#include <boost/program_options.hpp>
#include <PaddConfigs.hpp>
#include <Workers.hpp>
#include <spdlog/spdlog.h>
#include <signal.h> //  our new library 
#include <ScratchPaddSystem.hpp>
#include <Timer.hpp>
#include <EventTimer.hpp>
#include <chrono>

volatile sig_atomic_t flag = 0;
void signal_handler(int sig);



int main(int argc, char **argv) {
  SCOPED_METHOD_TIMER();
  signal(SIGINT, signal_handler); 
  ScratchPadd::System<SCRATCHPADD_GL4> SPSystem;
  spdlog::info("Welcome to SCRATCHPADD!");
  SPSystem.instantiate();
  SPSystem.start();
  SPSystem.stop();

  ScratchPadd::EventTimer testRepeatTimer;
  ScratchPadd::Timer testTimer("TEST TIMER");
  testTimer.start();
  testRepeatTimer.startRepeatingEvent([&]{
    testTimer.markInterval();
  },15);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  testRepeatTimer.stop();
  testTimer.printAverageInterval();
  testTimer.markTimeAndPrint();
  std::this_thread::sleep_for(std::chrono::seconds(2));
}

void signal_handler(int sig){ // can be called asynchronously
  spdlog::info("shutting down");
  flag = 0; // set flag
  // SPSystem.stop();
}