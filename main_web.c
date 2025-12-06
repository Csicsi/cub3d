/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_web.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcsicsak <dcsicsak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 20:30:00 by dcsicsak          #+#    #+#             */
/*   Updated: 2025/12/06 20:32:33 by dcsicsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <GLFW/glfw3.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

t_data	*g_data;

void	web_main_loop(void)
{
	if (glfwWindowShouldClose(g_data->mlx->window))
	{
#ifdef __EMSCRIPTEN__
		emscripten_cancel_main_loop();
#endif
		safe_exit(&g_data->map, EXIT_SUCCESS);
	}
	mlx_loop(g_data->mlx);
}

int	main(int argc, char **argv)
{
	static t_data	data;
	char			*default_argv[] = {"cub3d", "maps/duck.cub"};

#ifdef __EMSCRIPTEN__
	argc = 2;
	argv = default_argv;
#endif

	data = (t_data){0};
	init_data(&data);
	data.map.is_bonus = true;
	parsing(argc, argv, &data.map);
	allocate_textures(&data);
	init_mlx(&data);
	set_player(&data);
	cast_rays(&data);
	load_all_textures(&data);
	render_scene(&data);
	mlx_key_hook(data.mlx, key_hook, &data);
	mlx_close_hook(data.mlx, close_hook, &data);
	mlx_loop_hook(data.mlx, loop_hook, &data);
	
	g_data = &data;
	
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(web_main_loop, 0, 1);
#else
	while (!glfwWindowShouldClose(data.mlx->window))
		mlx_loop(data.mlx);
	safe_exit(&data.map, EXIT_SUCCESS);
#endif
	
	return (0);
}
