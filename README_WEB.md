# Cub3D WebAssembly Build

## Prerequisites

- Emscripten SDK (emsdk)
- CMake
- Make

## Setup Emscripten

```bash
# Clone emsdk (if not already installed)
git clone https://github.com/emscripten-core/emsdk.git ~/emsdk
cd ~/emsdk

# Install and activate latest version
./emsdk install latest
./emsdk activate latest

# Activate environment (required for each shell session)
source ~/emsdk/emsdk_env.sh
```

## Build for Web

```bash
# 1. Build MLX42 for WebAssembly
cd MLX42
emcmake cmake -B build_web -DCMAKE_BUILD_TYPE=Release
cmake --build build_web
cd ..

# 2. Build the game
make web
```

This will generate:
- `cub3d.html` - HTML wrapper
- `cub3d.js` - JavaScript glue code
- `cub3d.wasm` - WebAssembly binary (160KB)
- `cub3d.data` - Preloaded assets (24MB - maps and textures)

## Testing Locally

```bash
# Start a local web server
python3 -m http.server 8000

# Open in browser
# http://localhost:8000/cub3d.html
```

## Controls

- **WASD** - Move
- **Mouse** - Look around
- **E** - Open/close doors
- **ESC** - Close

## Deployment

Upload all 4 files (`cub3d.html`, `cub3d.js`, `cub3d.wasm`, `cub3d.data`) to any static hosting service:
- GitHub Pages
- Netlify
- Vercel
- Any web server with static file serving

No server-side code needed - runs 100% in the browser!
