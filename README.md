# ScratchPadd #

A cross platform message passing system in C++

## Introduction ##

ScratchPadd is an actor model implementation in C++ that allows threadsafe message passing between various "Padds" each with their own work queue and run loop.

Data that need to be sent to different Padds are wrapped in a lambda and stored in a heap allocated std::function<void()> that is consumed and cleaned up by the receiving Padd.

### Supported Platforms ###
While the project is written ultimately with cross platform support in mind, it has not yet been tested or run on any platform aside from macOS 10.15 and Ubuntu 21.10


## Setup ##

### Prerequisites ###

The following are currently necesary to get started with minimal problems.

macOS 10.15 or higher

[Conan C++ Package Manager](https://docs.conan.io/en/latest/installation.html)

[CMake Build Tools (3.20 or higher)](https://cmake.org)

On macOS these are best installed using [Homebrew](https://brew.sh) with the following commands.

``` bash
brew update
brew install conan
brew install cmake
```

To setup this project it is advised that you are using macOS 10.15 to build.

For Ubuntu 20.04 you can used the following

``` bash
sudo apt update
sudo apt install python3
sudo apt install python3-pip
python3 -m pip install conan
# apt only installs 3.16 on Ubuntu 20.04 LTS so snap is needed to install 3.20 or higher
sudo snap install cmake
```

Add modify your PATH variable as instructed to allow you to run conan cli



### Building ###

``` bash
# Setup
git clone https://github.com/mrgrantham/ScratchPadd.git
cd ScratchPadd
mkdir build
cd build

# --probably only needed on ubuntu--
conan profile update settings.compiler.libcxx=libstdc++11 default 
# ----------------------------------

conan install ..
cmake ..
cmake --build . --clean-first --parallel 4 -- VERBOSE=1

# Run
./bin/ScratchPadd

```

## Roadmap ##
- [X] Imgui Addition
- [ ] Raspbian Support
- [ ] Metal Support (Mac / iOS)
- [ ] iOS Support 
- [ ] AudioPadd Audio Support
- [ ] CommPadd Implementation for Networking
