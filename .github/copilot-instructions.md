# Cub3D - AI Agent Instructions

## Project Overview
This is a raycasting-based 3D game engine (like Wolfenstein 3D) built with MiniLibX. It renders a first-person view from `.cub` map files using DDA-style raycasting algorithms.

## Architecture

### Dual Binary System
- **Mandatory**: `cub3D` binary from `main.c` (static textures only)
- **Bonus**: `cub3D_bonus` binary from `main_bonus.c` (animated textures + interactive doors)
- The only difference: `data.map.is_bonus` flag controls feature availability
- Shared codebase in `COMMON_SRCS` - same files serve both binaries

### Core Components
1. **Parsing** (`parsing*.c`): Validates `.cub` files - textures (NO/SO/EA/WE/DO), RGB colors (F/C), and map grid
2. **Raycasting** (`cast_rays*.c`): DDA algorithm split into horizontal/vertical intersection checks, stores results in `data->ray_distance[WIDTH]`
3. **Rendering** (`mlx.c`, `draw_utils.c`): Converts ray distances to wall heights, samples textures, draws to image buffer
4. **Movement** (`movement*.c`): WASD movement with collision detection using radius-based wall proximity checks
5. **Minimap** (`minimap.c`): Top-down view rendered in top-left corner (bonus only)

## Critical Patterns

### Animated Textures (Bonus Feature)
- `.cub` files can define **multiple textures per direction** by repeating the same identifier:
  ```
  NO ./textures/duck_1/output0.xpm
  NO ./textures/duck_1/output1.xpm
  ```
- Parsed into `t_sprite` structures with `texture_list` (linked list) → converted to `textures` array
- Animation cycles through frames at `FRAME_DURATION` (0.1s) intervals in `update_animation_frame()`
- Mandatory mode enforces exactly 1 texture per direction in `set_counts()`

### Map Format
- First lines: texture/color definitions (NO, SO, WE, EA, DO, F, C)
- After first non-empty non-definition line: rectangular grid starts
- Valid tiles: `1` (wall), `0` (floor), `N/S/E/W` (player spawn + direction), `D` (closed door), `d` (open door)
- Map must be enclosed by walls (`1`) - validated in `check_map()`

### Error Handling
- Use `safe_exit(map, exit_code)` for all cleanup - handles partial initialization
- `print_error(count, ...)` for variadic error messages to stderr
- Predefined exit codes: `MALLOC=2`, `MAP`, `TEXTURE`, `COLOR`, `WORNG_FILE`
- Always check `fd`, `mlx` pointers, and texture loading results

### MLX Integration
- Initialize order: `init_data()` → `parsing()` → `allocate_textures()` → `init_mlx()` → `set_player()` → `cast_rays()` → `load_all_textures()`
- Event loop: `mlx_hook()` for key press (2) and window close (17), `mlx_loop_hook()` for mouse tracking
- Rendering: Always draw to `data->img` buffer, then `mlx_put_image_to_window()` once

## Development Workflows

### Building
```bash
make            # Build mandatory version (cub3D)
make bonus      # Build bonus version (cub3D_bonus)
make clean      # Remove objects
make fclean     # Remove objects + binaries
make re         # Rebuild from scratch
```

### Testing
```bash
./cub3D maps/example.cub      # Run with static textures
./cub3D_bonus maps/duck.cub   # Run with animated textures + doors
```

### Adding New Features
1. Modify shared code in `COMMON_SRCS` files - both binaries will inherit changes
2. For bonus-only features: Guard with `if (data->map.is_bonus)` checks
3. Update `cub3d.h` for any new structures or function prototypes
4. Use `t_data` as the central struct - contains all game state

## Key Files
- `cub3d.h`: All structures, constants, function prototypes
- `init.c`: Initialization sequence and texture loading orchestration
- `cast_rays.c`: Main raycasting loop dispatching to horizontal/vertical checks
- `mlx.c`: Rendering pipeline and animation frame management
- `cleanup.c`: Centralized cleanup and error reporting
- `Makefile`: Dual-binary build system with shared sources

## Common Pitfalls
- Don't add new sources without updating `COMMON_SRCS` in Makefile
- Always initialize structures with `= (struct_type){0}` pattern
- Raycasting uses **grid coordinates** (0-indexed tiles), rendering uses **pixel coordinates**
- Door mechanics (bonus): `D` → `d` transition on proximity, `d` → `D` on distance
- MiniLibX functions return NULL on failure - always check before use
