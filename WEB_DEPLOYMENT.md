# Web Deployment Guide - Cub3D to WebAssembly

## Overview

This guide explains how to port Cub3D from MiniLibX to MLX42 and compile it to WebAssembly for web deployment on your portfolio.

## Option 1: MLX42 Migration (Recommended)

### What is MLX42?

MLX42 is a modern graphics library that replaces MiniLibX with:

- Native WebAssembly/Emscripten support
- OpenGL-based rendering (WebGL in browsers)
- Nearly identical API to MiniLibX
- Repository: https://github.com/codam-coding-college/MLX42

### Migration Steps

#### 1. Install MLX42

```bash
git clone https://github.com/codam-coding-college/MLX42.git
cd MLX42
cmake -B build
cmake --build build -j4
```

#### 2. Update Header Include

Change in `cub3d.h`:

```c
// OLD:
#include <mlx.h>

// NEW:
#include "MLX42/MLX42.h"
```

#### 3. Key API Changes

**Window & Image Creation:**

```c
// OLD MiniLibX:
data->mlx = mlx_init();
data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "cub3d");
data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);

// NEW MLX42:
data->mlx = mlx_init(WIDTH, HEIGHT, "cub3d", true);
data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
mlx_image_to_window(data->mlx, data->img, 0, 0);
```

**Pixel Manipulation:**

```c
// OLD: Direct address manipulation
char *dst = data->addr + (y * data->line_len + x * (data->bpp / 8));
*(unsigned int *)dst = color;

// NEW: Direct pixel array access
uint32_t pixel_index = y * WIDTH + x;
data->img->pixels[pixel_index] = color;
```

**Event Hooks:**

```c
// OLD:
mlx_hook(data->win, 2, 1L << 0, key_hook, &data);
mlx_loop_hook(data->mlx, track_mouse, &data);
mlx_loop(data->mlx);

// NEW:
mlx_key_hook(data->mlx, key_hook_mlx42, &data);
mlx_loop_hook(data->mlx, render_loop, &data);
mlx_loop(data->mlx);
```

**Texture Loading:**

```c
// OLD:
texture->img = mlx_xpm_file_to_image(data->mlx, file, &width, &height);
texture->data = (int *)mlx_get_data_addr(texture->img, &bpp, &line_len, &endian);

// NEW:
mlx_texture_t *tex = mlx_load_xpm42(file);
mlx_image_t *img = mlx_texture_to_image(data->mlx, tex);
// Access pixels directly: img->pixels[y * img->width + x]
mlx_delete_texture(tex);
```

#### 4. Update Makefile

Add MLX42-specific compilation:

```makefile
# Add MLX42 paths
MLX42_DIR = MLX42
MLX42_LIB = $(MLX42_DIR)/build/libmlx42.a

# Update flags for native build
NATIVE_FLAGS = -lglfw -ldl -pthread -lm

# WebAssembly build target
WASM_CC = emcc
WASM_FLAGS = -s USE_WEBGL2=1 -s FULL_ES3=1 -s WASM=1 \
             -s ALLOW_MEMORY_GROWTH=1 \
             --preload-file maps --preload-file textures

# Targets
web: ${COMMON_SRCS} main_bonus.c
	$(WASM_CC) $(WASM_FLAGS) $(COMMON_SRCS) main_bonus.c $(MLX42_LIB) \
	-o web/cub3d.html
```

#### 5. Build for Web

```bash
# Install Emscripten if not installed
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh

# Build MLX42 with Emscripten
cd MLX42
emcmake cmake -B build
cmake --build build

# Build your project
cd ../
make web
```

#### 6. Deploy

The `make web` command generates:

- `cub3d.html` - Main HTML file
- `cub3d.js` - JavaScript runtime
- `cub3d.wasm` - WebAssembly binary
- `cub3d.data` - Preloaded assets (maps/textures)

Host these files on any static web server (GitHub Pages, Netlify, Vercel, etc.)

### Estimated Migration Effort

- **Small project like this**: 2-4 hours
- **Main changes**: Event handling, pixel manipulation, texture loading
- **Core raycasting logic**: Zero changes needed!

## Option 2: Keep MiniLibX + VNC Server (Not Recommended)

If you want to avoid migration, you could:

1. Run the native binary on a Linux server
2. Stream X11 display via VNC/noVNC
3. Embed in iframe with websocket proxy

**Downsides:**

- Requires running server (cost)
- High latency
- Poor mobile experience
- Security concerns

## Option 3: Record Demo Video + Interactive Elements

Quick portfolio solution while working on WASM:

1. Record high-quality gameplay video
2. Create interactive overlay showing:
   - Map editor that previews rendering
   - Code snippets with syntax highlighting
   - Performance metrics visualization
3. Link to GitHub repo with build instructions

## Recommended Timeline

**Week 1**: MLX42 migration

- Day 1-2: Setup and basic rendering
- Day 3-4: Input handling and textures
- Day 5: Testing and bug fixes

**Week 2**: Web deployment

- Day 1-2: Emscripten compilation
- Day 3: Optimize WASM bundle size
- Day 4-5: Deploy and create portfolio page

## Resources

- MLX42 Documentation: https://github.com/codam-coding-college/MLX42/wiki
- Emscripten Guide: https://emscripten.org/docs/getting_started/
- WebAssembly.org: https://webassembly.org/

## Testing the Web Build

```bash
# Serve locally (Emscripten includes server)
emrun web/cub3d.html

# Or use Python
cd web
python3 -m http.server 8000
# Visit http://localhost:8000/cub3d.html
```

## Common Issues & Solutions

### XPM Textures

MLX42 supports XPM42 format (similar to XPM). You may need to convert:

```bash
# Your current XPM files should work, but if issues:
convert texture.xpm texture.png
# Use PNG loader: mlx_load_png() instead
```

### Mouse Lock

For browser FPS controls:

```javascript
// Add to generated HTML
canvas.addEventListener("click", () => {
  canvas.requestPointerLock();
});
```

### File System

Emscripten preloads files into virtual FS. Your file reading code works unchanged!

## Next Steps

1. Create a branch: `git checkout -b web-port`
2. Follow migration steps above
3. Test locally with Emscripten
4. Deploy to your portfolio
5. (Optional) Keep both versions - native and web

Good luck! The WebAssembly version will be an impressive portfolio piece. 🚀
