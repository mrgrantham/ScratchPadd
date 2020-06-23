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


typedef ScratchPadd::System<SCRATCHPADD_GL4> SPSystem;

int main(int argc, char **argv) {
  SCOPED_METHOD_TIMER();
  signal(SIGINT, signal_handler); 
  spdlog::info("Welcome to SCRATCHPADD!");
  SPSystem::instantiate();
  SPSystem::start();
  std::this_thread::sleep_for(std::chrono::seconds(5));
  SPSystem::stop();
}

void signal_handler(int sig){ // can be called asynchronously
  spdlog::info("shutting down");
  flag = 0; // set flag
  SPSystem::stop();
}