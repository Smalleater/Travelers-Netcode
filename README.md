# Travelers Netcode (TRA)

---

## Overview

**TRA (Travelers Netcode)** is a cross-platform C++ netcode designed to simplify multiplayer game development.
It supports both Windows (via Winsock) and POSIX-compliant systems (via BSD sockets).

The current goal is to provide an optimized, lightweight, and easy-to-use netcode capable of handling all basic networking operations such as creating servers, connecting clients, sending packets, and more.
The ultimate goal is to develop TRA into a fully featured netcode specifically tailored for multiplayer needs.

---

## 🚧 Current Development Status

I am currently working on a **custom ECS (Entity Component System) library** that will serve as a core foundation for the netcode.
This ECS library is designed specifically to fit the needs of multiplayer and networking (synchronization, entity ownership, replication, etc.).

➡️ **Active netcode development is temporarily paused** while this ECS library is under development.
Once the ECS library reaches a stable and usable state, I will **resume and accelerate work on the netcode**, integrating it tightly with the ECS for cleaner architecture and better scalability.

This approach allows the netcode to be built on a solid and future-proof foundation rather than retrofitting ECS concepts later.

---

## 📄 Documentation

➡️ [View the documentation](https://smalleater.github.io/Travelers-Multiplayer-Engine/)

---

## ⚙️ Technologies

* C++17
* Windows (via Winsock)
* POSIX (Linux, macOS, etc. via BSD sockets)

---

## 📘 Learning Purpose

This project is part of a personal journey to learn and improve my skills in **C++**, **network programming**, and **engine architecture**.
Both the netcode and the ECS library are developed with learning, experimentation, and long-term improvement in mind.

New features and improvements will continue to come as I explore and grow.

---

## 📫 Contact

For questions, suggestions, or feedback:

📬 [My GitHub profile](https://github.com/Smalleater)

💬 [Ask a question or start a discussion](https://github.com/Smalleater/Travelers-Multiplayer-Engine/discussions)
