[![Build](https://github.com/TxbiG/MossGodot/actions/workflows/build.yml/badge.svg)](https://github.com/TxbiG/MossGodot/actions/workflows/build.yml)
# MossGodot

**Native integration between the [Moss Framework](https://github.com/TxbiG/Moss) and [Godot Engine](https://godotengine.org/), built with C++ and GDExtension.**

MossGodot is an open-source project designed to bring the capabilities of the Moss Framework into Godot Engine.
Using native C++ and GDExtension, the project aims to expose Moss functionality through Godot-compatible interfaces, providing developers with access to native framework systems while retaining Godot's scene, resource and editor workflows.
The integration is designed to be modular, extensible and performance-oriented, with support planned across multiple engine subsystems.

> The initial GDExtension structure and subsystem scaffolding are established. Native Moss backend integration and functional subsystem implementations remain under development.

### Features
- **Physics** – 2D and 3D physics, collision detection, rigid bodies and queries.
- **Audio** – Playback, spatial audio and audio processing.
- **Input** – Keyboard, mouse, controllers and input management.
- **Navigation** – Pathfinding, navigation meshes and agents.
- **Rendering** – Graphics resources, shaders and rendering utilities.
- **Networking** – Connections, data transport and multiplayer functionality.
- **XR** – Virtual and augmented reality integrations.

## Documentation
Comprehensive documentation is available in the [`docs/`](docs/README.md) directory:

* Architecture overview
* Rendering system design
* Physics system design
* Audio pipeline
* Input system
* Networking model
* Platform backend architecture
* Performance guidelines

- [API cheatsheet](docs/Cheatsheet.md)
- [API Reference](docs/API_Reference.md)
- [Roadmap](docs/README.md#roadmap)


## Required features
- x86/x64
  - Minimum: SSE2.
  - Optional compile targets: SSE4.1, SSE4.2, AVX, AVX2, or AVX512.
- ARM32/ARM64 
  - NEON and FP16. 
  - ARM32 can be built without any special CPU extensions.
- Other Architectures
  - RISC-V64/LoongArch64/PowerPC64LE support
 
## Compiling
- Moss Framework
- C++17
- CMake 3.20 or newer
- Godot Engine 4.7
- GDExtension
- godot-cpp

## Getting Started

```bash
git clone https://github.com/TxbiG/MossGodot.git
cd MossGodot

cmake -S . -B build
cmake --build build --config Release
```

## License
Distributed under the [MIT License](LICENSE).
