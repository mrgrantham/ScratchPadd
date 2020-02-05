#ifndef BASEWORKER_HPP
#define BASEWORKER_HPP

class BaseWorker {
  private:
  bool on_{true};

  public:
  virtual void prepare(){}
  virtual void cleanup(){}
  virtual void starting(){}
  virtual void finishing(){}
  void run() {
    starting();
    while (on_) {
      std::cout << "working: " << std::endl;
    }
    finishing();
  }
};

#endif