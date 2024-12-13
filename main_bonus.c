/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:43:22 by icseri            #+#    #+#             */
/*   Updated: 2024/12/13 13:39:21 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	main(int argc, char **argv)
{
	t_data	data;

	data = (t_data){0};
	init_data(&data);
	data.last_frame = get_time();
	data.map.is_bonus = true;
	parsing(argc, argv, &data.map);
	allocate_textures(&data);
	init_mlx(&data);
	mlx_mouse_move(data.mlx, data.win, WIDTH / 2, HEIGHT / 2);
	set_player(&data);
	cast_rays(&data);
	load_all_textures(&data);
	render_scene(&data);
	mlx_put_image_to_window(data.mlx, data.win, data.img, 0, 0);
	mlx_hook(data.win, 2, 1L << 0, key_hook, &data);
	mlx_hook(data.win, 17, 0, close_window, &data);
	mlx_loop_hook(data.mlx, track_mouse, &data);
	mlx_loop(data.mlx);
	safe_exit(&data.map, EXIT_SUCCESS);
}

/* int	main(int argc, char **argv)
{
	t_data	data;

	data = (t_data){0};
	init_data(&data);
	data.last_frame = get_time();
	data.map.is_bonus = true;
	parsing(argc, argv, &data.map);
	allocate_textures(&data);
	init_mlx(&data);
	mlx_mouse_move(data.mlx, data.win, WIDTH / 2, HEIGHT / 2);
	mlx_mouse_hide(data.mlx, data.win);
	set_player(&data);
	cast_rays(&data);
	load_all_textures(&data);
	render_scene(&data);
	mlx_put_image_to_window(data.mlx, data.win, data.img, 0, 0);
	mlx_hook(data.win, 2, 1L << 0, key_hook, &data);
	mlx_hook(data.win, 17, 0, close_window, &data);
	mlx_loop_hook(data.mlx, track_mouse, &data);
	mlx_loop(data.mlx);
	safe_exit(&data.map, EXIT_SUCCESS);
} */