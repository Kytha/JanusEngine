<h1 align="center">
    <a href="https://github.com/Kytha/JanusEngine">
        <img src="var/docs/images/janusEngine.png" valign="middle" width="58" height="58" alt="Janus Engine Logo" />
    </a>
    <span valign="middle">
        Janus Engine
    </span>
</h1>

<h3 align="center">A cross-platform game engine built in C++ for creating 2D & 3D games</h3>

<br>

<p align="center">
    <a href="https://github.com/Kytha/JanusEngine/commits/master">
    <img src="https://img.shields.io/github/last-commit/Kytha/JanusEngine.svg?style=for-the-badge&logo=github&logoColor=white"
         alt="GitHub last commit">
    <a href="https://github.com/Kytha/JanusEngine/blob/master/LICENSE.md">
    <img src="https://img.shields.io/github/license/Kytha/JanusEngine.svg?style=for-the-badge&logo=github&logoColor=white"
         alt="License">
    <a href="https://github.com/Kytha/JanusEngine/issues">
    <img src="https://img.shields.io/github/issues/Kytha/JanusEngine.svg?style=for-the-badge&logo=github&logoColor=white"
         alt="GitHub issues">
    <a href="https://github.com/Kytha/JanusEngine/pulls">
    <img src="https://img.shields.io/github/issues-pr-raw/Kytha/JanusEngine.svg?style=for-the-badge&logo=github&logoColor=white"
         alt="GitHub pull requests">
</p>

---

<h4 align="center">
  <a href="#About">About</a> •
  <a href="#Getting-Started">Getting Started</a> •
  <a href="#Platform-Support">Platform Support</a> •
  <a href="#License">License</a> •
  <a href="https://janusengine.com">Documentation</a>
</h4>

---

# About

A cross-platform game engine built in C++ for creating 2D & 3D games. It is as much an educational project as a passion project.

# Getting-Started

If you want to start building games with Janus Engine, follow the steps below to build the engine and tools from source.

## Installing

Janus Engine uses CMake for it's build system, so you have to make sure that you have a working cmake-installation on your system. You can download it at https://cmake.org/ or for linux install it via: `sudo apt-get install cmake`.

Make sure you have a working git-installation. Open a command prompt and recursively clone the repo via:

```bash
git clone --recursive https://github.com/Kytha/Janus.git
```

Alternatively, if the repository was not cloned recursively use the following command.

```bash
git submodule update --init
```

## Building

Once your local repo is setup, you'll want to execute the following commands in the root directory to configure CMake

```bash
mkdir build
cd build
cmake ..
make
```

# Platform Support

The Janus engine is intended to be a cross-platform engine to design cross-platform games. Applications built with the Janus Engine have only been officially tested on Windows, but in its current state should support the following platforms;

- Windows ✔️
- Mac ❌
- Linux ❌
- Android ❌
- IOS ❌

# License

Janus Engine is licensed under the MIT license.
