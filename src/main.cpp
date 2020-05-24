#include <iostream> 
#include <boost/program_options.hpp>
#include <WorkerSystem.hpp>
#include <Workers.hpp>
#include <signal.h> //  our new library 
volatile sig_atomic_t flag = 0;
void signal_handler(int sig);


Dispatcher dispatcher;

int main(int argc, char **argv) {
  std::cout << "Starting WorkerSystem" <<std::endl;
  signal(SIGINT, signal_handler); 

  dispatcher.start();
  dispatcher.join();
}

void signal_handler(int sig){ // can be called asynchronously
  std::cout << "shutting down" << std::endl;
  flag = 0; // set flag
  dispatcher.stop();
}