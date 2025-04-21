# Universal C/C++ development container

***All the basics from the get-go***

## Table of contents

- [Universal C/C++ development container](#universal-cc-development-container)
  - [Table of contents](#table-of-contents)
  - [About](#about)
  - [Features and Development tools](#features-and-development-tools)
  - [Setup](#setup)
    - [VSCode](#vscode)
    - [Alternative (manual building and running)](#alternative-manual-building-and-running)
  - [Configuration](#configuration)
  - [TODO](#todo)

## About

This repository holds everything you may need to start developing a new C/C++
project in a `Docker container`:

- A self-sufficient `Dockerfile`, if you need not the `devcontainer.json`
  automations
- Some useful `VSCode` quality of life features:
  - The most useful and common extensions are marked for autoinstall
  - Compiler and Debugger configurations
- An example code to test

## Features and Development tools

***WIP***

## Setup

### VSCode

1. Execute `/.devcontainer/parse-non-root.sh`. This will parse `uname`, `uid`,
   `group` and `gid` to `.devcontainer/Dockerfile`.
2. Open this project in devcontainer
3. Wait a bit for extension installation (a VSCode might show a warning about
   incorrect `tasks.json`. This is due to extensions being not yet installed -
   ignore)
4. Check if `/basic_example/src/main.cpp` builds and launches, and IntelliSense works

### Alternative (manual building and running)

If you'd like not to use `VSCode`, you can manually build and run the container,
since all development tools are hard-coded into it.

1. Execute `/.devcontainer/parse-non-root.sh`. This will parse `uname`, `uid`,
   `group` and `gid` to `.devcontainer/Dockerfile`.
2. Build image
3. Run container

## Configuration

***WIP***

## TODO

- [x] Fix cpptools integration
- [x] Add more tasks and launch configs
- [x] Use `CMake`
- [x] Add an intermediate example, that could work only with `CMake` additional utility
- [ ] Add `vcpkg` support
- [ ] Add an advanced example, that uses `vcpkg` and `GTest`
- [ ] Check, whether mounting `ssh` etc. could be a `feature`
- [ ] Add `LICENSE.md`
- [ ] Finalize `README.md`
