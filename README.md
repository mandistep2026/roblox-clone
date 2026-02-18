# Roblox Clone Engine

A cross-platform game engine similar to Roblox, built with C++17.

## Features

- **3D Renderer** - OpenGL 4.5 based rendering with camera controls
- **Scene Editor** - ImGui-based scene editor with viewport, hierarchy, and properties panels
- **Scripting System** - Lua 5.4 scripting with engine API bindings
- **Multiplayer** - ENet-based client/server networking
- **Entity-Component System** - EnTT ECS architecture

## Requirements

- CMake 3.20+
- C++17 compiler (GCC, Clang, MSVC)
- vcpkg (for dependency management)

## Dependencies

| Library | Purpose |
|---------|---------|
| SDL3 | Window/input management |
| OpenGL 4.5 + GLEW | Graphics API |
| GLM | Math library |
| EnTT | Entity-Component-System |
| Lua 5.4 + sol2 | Scripting system |
| ENet | Networking |
| Dear ImGui | Editor UI |
| spdlog | Logging |
| nlohmann-json | Configuration |

## Build Instructions

### 1. Install vcpkg (if not already installed)

```bash
git clone https://github.com/Microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh
```

### 2. Install dependencies

```bash
./vcpkg/vcpkg install
```

### 3. Configure and build

```bash
# Configure
cmake --preset=debug

# Build
cmake --build --preset=debug
```

### 4. Run

```bash
./build/debug/roblox-clone
```

## Project Structure

```
roblox-clone/
├── CMakeLists.txt           # Root build configuration
├── CMakePresets.json        # Build presets
├── vcpkg.json               # Dependencies manifest
├── config.json              # Engine configuration
├── src/
│   ├── core/                # Application, logging, config
│   ├── renderer/            # OpenGL renderer, window, shaders
│   ├── scene/               # Scene graph, entities, components
│   ├── editor/              # ImGui editor UI
│   ├── scripting/           # Lua scripting engine
│   └── network/             # Client/server networking
├── assets/
│   └── shaders/             # GLSL shaders
└── tests/                   # Unit tests
```

## Usage

### Command Line Options

```bash
./roblox-clone [--no-editor] [--fullscreen]
```

- `--no-editor` - Run without the editor UI
- `--fullscreen` - Start in fullscreen mode

### Editor Controls

- **WASD** - Move camera (hold right mouse button)
- **Right Mouse Drag** - Look around
- **Q/E** - Move camera down/up

## License

MIT
