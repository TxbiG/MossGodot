[![Build](https://github.com/TxbiG/MossGodot/actions/workflows/build.yml/badge.svg)](https://github.com/TxbiG/MossGodot/actions/workflows/build.yml)
# MossGodot

**Native integration between the [Moss Framework](https://github.com/TxbiG/Moss) and [Godot Engine](https://godotengine.org/), built with C++ and GDExtension.**

## Overview

MossGodot is an open-source project designed to bring the capabilities of the Moss Framework into Godot Engine.

Using native C++ and GDExtension, the project aims to expose Moss functionality through Godot-compatible interfaces, providing developers with access to native framework systems while retaining Godot's scene, resource and editor workflows.

The integration is designed to be modular, extensible and performance-oriented, with support planned across multiple engine subsystems.

### Planned Features

- **Physics** – 2D and 3D physics, collision detection, rigid bodies and queries.
- **Audio** – Playback, spatial audio and audio processing.
- **Input** – Keyboard, mouse, controllers and input management.
- **Navigation** – Pathfinding, navigation meshes and agents.
- **Rendering** – Graphics resources, shaders and rendering utilities.
- **Networking** – Connections, data transport and multiplayer functionality.
- **XR** – Virtual and augmented reality integrations.

## Documentation

Documentation and development guides will be maintained separately.

| Documentation | Description |
|---|---|
| [Architecture](docs/ARCHITECTURE.md) | Project architecture and integration design |
| [Getting Started](docs/GETTING_STARTED.md) | Setup and installation instructions |
| [Building](docs/BUILDING.md) | Build configuration and compilation |
| [Subsystems](docs/SUBSYSTEMS.md) | Subsystem implementations and support status |
| [Contributing](CONTRIBUTING.md) | Contribution guidelines |

*Documentation files will be added as the project develops.*

## Compiling

- C++17
- CMake
- Godot Engine 4.7
- GDExtension
- godot-cpp
- Moss Framework

## Project Status

The initial GDExtension structure and subsystem scaffolding are established. Native Moss backend integration and functional subsystem implementations remain under development.

## Getting Started

```bash
git clone https://github.com/TxbiG/MossGodot.git
cd MossGodot

cmake -S . -B build
cmake --build build --config Release
```

## License

Distributed under the [MIT License](LICENSE).
