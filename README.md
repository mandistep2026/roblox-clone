# Roblox Clone Engine

A cross-platform game engine similar to Roblox, built with C++17.

## Features

- **3D Renderer** - OpenGL 4.5 based rendering with camera controls
- **Scene Editor** - ImGui-based scene editor with viewport, hierarchy, and properties panels
- **Scripting System** - Lua 5.4 scripting with engine API bindings
- **Multiplayer** - ENet-based client/server networking
- **Entity-Component System** - EnTT ECS architecture
- **Materials & Textures** - Material system with texture support

## Requirements

- CMake 3.20+
- C++17 compiler (GCC, Clang, MSVC)
- vcpkg (auto-installed by setup script)

## Dependencies

| Library | Purpose |
|---------|---------|
| SDL2 | Window/input management |
| OpenGL 4.5 + GLEW | Graphics API |
| GLM | Math library |
| EnTT | Entity-Component-System |
| Lua 5.4 + sol2 | Scripting system |
| ENet | Networking |
| Dear ImGui | Editor UI |
| spdlog | Logging |
| nlohmann-json | Configuration |
| stb | Image loading |

## Quick Start

### 1. Run the setup script

```bash
./setup.sh
```

This will:
- Clone vcpkg
- Bootstrap vcpkg
- Install all dependencies

### 2. Configure and build

```bash
cmake --preset=debug
cmake --build --preset=debug
```

### 3. Run

```bash
./build/debug/roblox-clone
```

## Manual Setup

If you prefer manual setup:

```bash
# Clone vcpkg
git clone https://github.com/Microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh

# Install dependencies
./vcpkg/vcpkg install

# Build
cmake --preset=debug
cmake --build --preset=debug
```

## Project Structure

```
roblox-clone/
├── CMakeLists.txt           # Root build configuration
├── CMakePresets.json        # Build presets
├── vcpkg.json               # Dependencies manifest
├── setup.sh                 # Setup script
├── config.json              # Engine configuration
├── src/
│   ├── core/                # Application, logging, config
│   ├── renderer/            # OpenGL renderer, window, shaders, textures
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
- **Mouse Scroll** - Zoom in/out

### Creating Objects

1. Click "Create Entity" in the Scene Hierarchy panel
2. Select the entity to view its properties
3. Use the Properties panel to set position, rotation, and scale

## Development Roadmap

### Phase 1: Core Engine (Current)
- [x] Basic renderer with OpenGL
- [x] Scene editor with ImGui
- [x] Entity-Component System
- [x] Lua scripting foundation
- [x] Networking foundation

### Phase 2: Rendering
- [ ] Lighting system (point, directional, spot lights)
- [ ] Shadow mapping
- [ ] Model loading (OBJ, GLTF)
- [ ] Advanced materials

### Phase 3: Gameplay
- [ ] Physics engine (Box2D/Bullet)
- [ ] Player character controller
- [ ] In-game object interaction
- [ ] Parent/child hierarchy

### Phase 4: Roblox-like Features
- [ ] Terrain editing
- [ ] Part creation tools
- [ ] Character customization
- [ ] Multiplayer sync

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

## License

MIT
