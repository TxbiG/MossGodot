# Contributing to MossGodot

Thank you for your interest in contributing to **MossGodot**.

MossGodot aims to provide a comprehensive integration between Godot and the [Moss Framework](https://github.com/TxbiG/Moss), using GDExtension and other appropriate Godot extension mechanisms.

The project is not limited to physics. Its long-term goal is to expose useful Moss functionality through Godot-friendly classes, resources, nodes, editor tooling and subsystem integrations.

Contributions are welcome across all supported areas of the framework.

## 1. Project vision

MossGodot should make the wider Moss Framework accessible to Godot developers without unnecessarily duplicating its underlying systems.

The intended integration areas include:

| Subsystem | Intended integration |
|---|---|
| Physics | 2D/3D simulation, bodies, shapes, collision queries and events |
| Audio | Audio playback, spatial sound, resources and audio controls |
| Input | Devices, events, actions, mappings and haptic feedback |
| Navigation | 2D/3D navigation, pathfinding and queries |
| Rendering | Graphics resource integration, rendering utilities and backend access |
| Networking | Multiplayer communication and synchronisation |
| XR | OpenXR and supported VR/AR/MR capabilities |
| Platform services | Cross-platform functionality and platform backends |
| Diagnostics | Logging, timing, profiling and debugging utilities |
| Other framework systems | Additional capabilities exposed through the Moss public API |

This is the intended integration scope, not a promise that every subsystem is currently implemented or fully supported.

## 2. Core principles

All contributions must follow these principles:

- **Moss owns the underlying implementation.** Avoid recreating Moss functionality unnecessarily.
- **Godot provides the engine-facing experience.** Use GDExtension, appropriate classes, resources and editor tooling.
- **Use verified APIs.** Never invent Moss functions, Godot extension methods or `godot-cpp` bindings.
- **Keep subsystems modular.** Physics, Audio, Input and other integrations should be independently usable where appropriate.
- **Respect Godot lifecycles.** Follow engine resource, RID, threading and shutdown requirements.
- **Do not force server replacement.** Use custom servers only where the relevant Godot extension API supports the required integration.
- **Prioritise performance and stability.**
- **Document actual support honestly.** Do not advertise partial features as complete replacements.

MossGodot should expose the wider Moss Framework rather than becoming exclusively a custom physics server.

## 3. Before contributing

Before starting work:

1. Read the README, architecture and roadmap.
2. Review the subsystem support matrix.
3. Check existing issues and pull requests.
4. Confirm supported Godot and `godot-cpp` versions.
5. Review the relevant Godot GDExtension API.
6. Review the actual Moss public headers.
7. Identify the correct integration mechanism for the subsystem.

Useful references:

- Moss Framework: https://github.com/TxbiG/Moss
- Godot: https://github.com/godotengine/godot
- godot-cpp: https://github.com/godotengine/godot-cpp
- GDExtension documentation: https://docs.godotengine.org/en/stable/engine_details/engine_api/gdextension/index.html
- PhysicsServer3D: https://docs.godotengine.org/en/stable/classes/class_physicsserver3d.html
- PhysicsServer3DExtension: https://docs.godotengine.org/en/stable/classes/class_physicsserver3dextension.html

Do not assume that every Godot subsystem has an equivalent custom-server extension point. Verify the appropriate public Godot API before designing an integration.

## 4. Development requirements

Depending on the subsystem, contributors may need:

- Git.
- CMake 3.20 or newer.
- A C++17-compatible compiler.
- Python 3 where required by build scripts.
- Godot Engine matching the documented version.
- The matching `godot-cpp` version.
- Moss source code and native libraries.
- Platform-specific development tools.

The README should document exact version and platform requirements.

## 5. Subsystem contribution guidelines

### 5.1 Physics

Physics integration should expose the supported Moss 2D and 3D physics functionality.

Possible integration areas include:

- Physics worlds and spaces.
- Static and dynamic bodies.
- Collision shapes.
- Collision filtering.
- Raycasts and shape queries.
- Contact reporting and callbacks.
- Areas and sensors.
- Joints and constraints.
- Fixed timestep and substeps.
- Debug visualisation.

A full custom Godot physics backend requires the appropriate server and direct-state integrations.

Do not register an incomplete physics server as a usable backend.

### 5.2 Audio

Expose available Moss audio features using appropriate Godot-facing interfaces.

Possible areas include:

- Audio system initialisation and shutdown.
- Playback and stopping.
- Audio resources.
- Volume and playback controls.
- Spatial audio and 3D positioning.
- Audio processing and effects where supported.
- Device selection and output configuration.
- Resource lifecycle and streaming, where available.

Clearly define the relationship between Moss audio resources and Godot's audio resources and buses.

Do not assume a Moss audio resource can be passed directly into Godot's audio system. Implement explicit, tested resource bridging where appropriate.

### 5.3 Input and haptics

Expose supported Moss input functionality through suitable Godot-facing interfaces.

Possible areas include:

- Keyboard and mouse input.
- Game controllers.
- Input devices and discovery.
- Input states and events.
- Actions and mappings.
- Device connection and disconnection.
- Haptic and vibration feedback.
- Event timing and processing.

Avoid duplicate or conflicting input events with Godot's existing Input system.

The integration should provide clearly documented ways to use Moss input alongside or independently from Godot input.

### 5.4 Navigation

Expose available Moss navigation features for 2D and 3D projects.

Possible areas include:

- Navigation worlds and configuration.
- Navigation data and surfaces.
- Pathfinding.
- Navigation queries.
- Agent movement and updates.
- Obstacles and avoidance where supported.
- Debugging and visualisation.

Define how Moss navigation data interacts with Godot navigation resources and nodes.

Do not assume that Godot navigation resources are compatible with Moss without explicit conversion or bridging.

### 5.5 Rendering

Rendering integration should expose appropriate Moss rendering capabilities to Godot.

Possible areas include:

- Graphics backend configuration.
- Graphics resources and resource interoperability.
- Textures and buffers.
- Shader and material workflows.
- Debug rendering.
- Rendering utilities.
- Device and resource synchronisation.

Respect Godot's RenderingServer, RenderingDevice and rendering lifecycle requirements where applicable.

Do not assume that resources from one graphics backend can be shared safely with another. Any resource interoperability must be explicit, documented and tested.

### 5.6 Networking and multiplayer

Possible areas include:

- Network initialisation and teardown.
- Connections and disconnections.
- Data transport.
- Message handling.
- Multiplayer communication.
- Synchronisation functionality.
- Diagnostics and configuration.

Document how Moss networking relates to Godot's multiplayer APIs and high-level multiplayer features.

Do not assume that a Moss connection can be used as a Godot multiplayer peer without implementing and testing the necessary adapter.

### 5.7 XR and platform services

Where supported, expose:

- OpenXR integration.
- VR/AR/MR devices.
- XR tracking and input.
- Supported platform backends.
- Desktop and mobile functionality.
- Device configuration.

Respect Godot's XR interfaces and platform requirements.

Document hardware, runtime and platform limitations.

### 5.8 Other Moss functionality

Contributions are encouraged for additional functionality exposed through Moss, including:

- Resource management.
- Timing utilities.
- Logging.
- Profiling.
- Platform abstractions.
- Additional framework systems.

Expose these through appropriate Godot interfaces without introducing unnecessary engine-specific copies of existing Moss implementations.

## 6. Architecture and subsystem boundaries

The intended architecture is:

```text
Godot Project
     |
     v
MossGodot GDExtension
     |
     +---- Physics Integration
     +---- Audio Integration
     +---- Input Integration
     +---- Navigation Integration
     +---- Rendering Integration
     +---- Networking Integration
     +---- XR Integration
     +---- Platform and Utility Integration
     |
     v
Moss Native Adapter
     |
     v
Moss Framework
```

Each subsystem should use the most appropriate Godot integration mechanism. Depending on the feature, this may be:

- GDExtension classes.
- Godot resources.
- Custom nodes.
- Editor plugins and inspectors.
- Engine server extensions where officially supported.
- Bridges to existing Godot engine systems.

Do not build a custom server simply because the feature has a server-like name in Godot. Verify that its extension API exists and is suitable for the intended use.

Each subsystem must document:

- Initialisation and shutdown.
- Resource creation and destruction.
- RID or handle ownership.
- Threading and callback requirements.
- Conversion between Godot and Moss data types.
- Error handling.
- Platform support.
- Known limitations.

## 7. Code standards

### C++

- Use C++17.
- Follow the project's existing conventions.
- Prefer RAII and clear ownership.
- Validate pointers and handles.
- Avoid unnecessary allocations.
- Keep Moss adapters separate from Godot-facing classes.
- Centralise type and coordinate conversions.
- Document threading, lifecycle and synchronisation requirements.

### Godot and GDExtension

- Follow Godot GDExtension conventions.
- Use the correct `godot-cpp` API.
- Respect reference-counted resource ownership.
- Use appropriate initialisation and termination stages.
- Keep registered classes focused.
- Avoid undocumented engine internals.
- Keep platform-specific native code isolated.
- Respect Godot's main thread and rendering/physics callback requirements.

## 8. Branches and commit messages

Suggested branches:

- `feature/audio-integration`
- `feature/input-integration`
- `feature/navigation-integration`
- `feature/rendering-integration`
- `feature/physics-server`
- `feature/networking`
- `feature/openxr`
- `fix/resource-lifecycle`
- `docs/subsystem-guide`
- `test/gdextension-loading`

Suggested commits:

- `feat: add Moss audio resource integration`
- `feat: expose Moss input events`
- `feat: add navigation query bindings`
- `fix: correct native resource destruction`
- `docs: document rendering integration`
- `test: add GDExtension lifecycle coverage`

Keep commits focused and avoid mixing unrelated subsystems.

## 9. Bug reports and feature requests

Bug reports should identify the affected subsystem, reproduction steps, expected and actual behaviour, Godot and `godot-cpp` versions, Moss version or commit, operating system, compiler and relevant logs.

Feature requests should explain:

- The problem being solved.
- The relevant Moss subsystem.
- Why the functionality belongs in MossGodot.
- The desired Godot-facing experience.
- Whether Moss needs changes.
- Any Godot extension requirements.
- Resource and threading implications.
- How the feature can be tested.

Discuss large architectural changes before implementation.

## 10. Pull requests

Every pull request should:

1. Describe the change and affected subsystem.
2. Reference relevant issues.
3. Identify the exact Moss public APIs used.
4. Identify the relevant Godot APIs.
5. Explain ownership and threading changes.
6. Include build and testing instructions.
7. Document limitations.
8. Update the subsystem support matrix.
9. Update examples and documentation.

Before submission, confirm that no unverified Moss or Godot APIs have been introduced and that native resources are released safely.

## 11. Testing

Testing should be specific to the relevant subsystem.

Examples include:

- Physics simulation and query tests.
- Audio playback and resource tests.
- Input and haptic tests.
- Navigation and pathfinding tests.
- Rendering resource and synchronisation tests.
- Networking and communication tests.
- XR initialisation and device tests.
- Resource lifetime and shutdown tests.
- GDExtension loading tests.
- Cross-platform build tests.
- Performance benchmarks.

A successful GDExtension build does not prove that the integration is functionally compatible with Godot.

## 12. Subsystem support matrix

Maintain a support matrix with these statuses:

- **Not started:** no integration available.
- **In development:** work is underway.
- **Experimental:** usable in limited scenarios.
- **Partial:** selected functionality is implemented and tested.
- **Supported:** documented capabilities pass the required tests.
- **Unsupported:** intentionally unavailable in this integration.

Update the matrix as each subsystem progresses.

Do not claim full replacement or parity with Godot systems without evidence.

## 13. Platform compatibility

Document supported Godot versions, `godot-cpp` versions, operating systems, CPU architectures, compiler toolchains and native library configurations.

Only claim a platform is supported after building and testing on that target.

## 14. Security and licensing

Never commit credentials, private keys, access tokens or confidential project files.

Report security vulnerabilities privately to the repository maintainer.

By contributing, you agree that your contribution may be distributed under the licence used by this repository. Only submit work that you have permission to redistribute.

## 15. Final note

MossGodot aims to expose the wider Moss Framework to Godot developers.

Physics is one part of this goal. Audio, input, navigation, rendering, networking, XR and other supported framework systems are equally important areas of development.

The priority is a modular, reliable and maintainable integration that preserves Moss as the underlying implementation.

Thank you for contributing to MossGodot.
