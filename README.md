# 🎮 Plota Mini Game

![C++](https://img.shields.io/badge/C++-17-blue.svg?style=flat&logo=c%2B%2B)
![Qt](https://img.shields.io/badge/Qt-5.15%2B-41CD52.svg?style=flat&logo=qt)
![CMake](https://img.shields.io/badge/CMake-Build-064F8C.svg?style=flat&logo=cmake)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows-lightgrey.svg)

> A fun and interactive 2D mini-game developed using **C++** and the **Qt Framework**. 
> This repository contains the source code for a university project, focusing on Object-Oriented Programming (OOP) principles, event handling, and memory management.

## 📑 Table of Contents
- [About the Project](#-about-the-project)
- [Features](#-features)
- [Prerequisites](#-prerequisites)
- [Installation & Build](#-installation--build)
  - [Standard CMake Build](#standard-cmake-build)
  - [Arch Linux (PKGBUILD)](#arch-linux-pkgbuild)
- [Project Structure](#-project-structure)
- [How to Play](#-how-to-play)
- [Troubleshooting](#-troubleshooting)
- [Acknowledgments](#-acknowledgments)

## 🚀 About the Project
**Plota Mini Game** is a lightweight desktop application designed to demonstrate the power of Qt's graphics view framework. The game features smooth character movements, collision detection, and a scoring system. It serves as a practical implementation of academic concepts in a real-world application.

## ✨ Features
* **Custom UI/UX:** Clean and intuitive user interface built with Qt Widgets.
* **Responsive Controls:** Low-latency keyboard/mouse event handling.
* **Robust Architecture:** Clean separation of game logic and UI (Model-View architecture).
* **Cross-Platform:** Can be compiled and run on Linux, Windows, and macOS.
* **[Add Feature]:** [e.g., Dynamic difficulty scaling, local high-score saving, etc.]

## 🛠 Prerequisites
To build and run this game from the source, you will need the following development tools installed on your system:
* **C++ Compiler:** GCC, Clang, or MSVC (supporting C++17 or later).
* **Build System:** CMake (version 3.10 or higher) and Make/Ninja.
* **Qt Framework:** Qt 5.x or Qt 6.x (Core, Gui, Widgets modules).

**For Arch Linux users:**
```bash
sudo pacman -S base-devel cmake qt5-base
