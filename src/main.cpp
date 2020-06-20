#include <iostream> 
#include <boost/program_options.hpp>
#include <PaddConfigs.hpp>
#include <Workers.hpp>
#include <spdlog/spdlog.h>
#include <signal.h> //  our new library 
#include <ScratchPaddSystem.hpp>
volatile sig_atomic_t flag = 0;
void signal_handler(int sig);



int main(int argc, char **argv) {
  signal(SIGINT, signal_handler); 
  ScratchPadd::System<SCRATCHPADD_GL4> SPSystem;
  spdlog::info("Welcome to SCRATCHPADD!");
  {
  SPSystem.instantiate();
  SPSystem.start();
  SPSystem.stop();
  }

}

void signal_handler(int sig){ // can be called asynchronously
  spdlog::info("shutting down");
  flag = 0; // set flag
  // SPSystem.stop();
}