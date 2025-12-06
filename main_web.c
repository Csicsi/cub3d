/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_web.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcsicsak <dcsicsak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by dcsicsak          #+#    #+#             */
/*   Updated: 2025/12/06 19:13:50 by dcsicsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdio.h>

int	main(int argc, char **argv)
{
	t_data	data;
	char	*default_map = "maps/duck.cub";
	char	*map_file;

	printf("Starting cub3D WebAssembly build...\n");
	data = (t_data){0};
	init_data(&data);
	data.map.is_bonus = true;
	
	if (argc > 1)
		map_file = argv[1];
	else
		map_file = default_map;
	
	printf("Loading map: %s\n", map_file);
	char *fake_argv[] = {"cub3D", map_file};
	parsing(2, fake_argv, &data.map);
	printf("Map parsed successfully\n");
	allocate_textures(&data);
	printf("Textures allocated\n");
	init_mlx(&data);
	printf("MLX initialized\n");
	set_player(&data);
	printf("Player set\n");
	cast_rays(&data);
	printf("Rays cast\n");
	load_all_textures(&data);
	printf("Textures loaded\n");
	render_scene(&data);
	printf("Scene rendered\n");
	mlx_key_hook(data.mlx, key_hook, &data);
	mlx_close_hook(data.mlx, close_hook, &data);
	mlx_loop_hook(data.mlx, loop_hook, &data);
	printf("Entering main loop...\n");
	mlx_loop(data.mlx);
	safe_exit(&data.map, EXIT_SUCCESS);
}
