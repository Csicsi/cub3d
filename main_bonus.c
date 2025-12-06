/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcsicsak <dcsicsak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:43:22 by icseri            #+#    #+#             */
/*   Updated: 2024/12/19 08:06:46 by dcsicsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	main(int argc, char **argv)
{
	t_data	data;

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
	mlx_loop(data.mlx);
	safe_exit(&data.map, EXIT_SUCCESS);
}
