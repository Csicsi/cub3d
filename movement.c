/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csicsi <csicsi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 13:08:57 by icseri            #+#    #+#             */
/*   Updated: 2024/12/08 13:18:41 by csicsi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	is_wall_at_position(t_data *data, double new_x, double new_y)
{
	t_proximity	params;
	int			map_x;
	int			map_y;

	map_x = (int)new_x;
	map_y = (int)new_y;
	if (map_x < 0 || map_x >= data->map.column || map_y < 0 || map_y >= data->map.row)
		return (1);
	params.radius = 0.2;
	params.dx = -params.radius;
	while (params.dx <= params.radius)
	{
		params.dy = -params.radius;
		while (params.dy <= params.radius)
		{
			params.check_x = new_x + params.dx;
			params.check_y = new_y + params.dy;
			if (sqrt(params.dx * params.dx + params.dy * params.dy) > params.radius)
			{
				params.dy += 0.1;
				continue ;
			}
			params.adj_x = (int)params.check_x;
			params.adj_y = (int)params.check_y;
			if (params.adj_x < 0 || params.adj_x >= data->map.column || params.adj_y < 0 || params.adj_y >= data->map.row)
			{
				params.dy += 0.1;
				continue ;
			}
			if (data->map.map[params.adj_y][params.adj_x] == '1')
				return (1);
			params.dy += 0.1;
		}
		params.dx += 0.1;
	}
	return (0);
}

static void	check_and_open_door_nearby(t_data *data, double new_x, double new_y)
{
	t_proximity	params;
	int			map_x;
	int			map_y;

	map_x = (int)new_x;
	map_y = (int)new_y;
	if (map_x < 0 || map_x >= data->map.column || map_y < 0 || map_y >= data->map.row)
		return ;
	params.radius = 1.5;
	params.dx = -params.radius;
	while (params.dx <= params.radius)
	{
		params.dy = -params.radius;
		while (params.dy <= params.radius)
		{
			params.check_x = new_x + params.dx;
			params.check_y = new_y + params.dy;
			if (sqrt(params.dx * params.dx + params.dy * params.dy) > params.radius)
			{
				params.dy += 0.1;
				continue ;
			}
			params.adj_x = (int)params.check_x;
			params.adj_y = (int)params.check_y;
			if (params.adj_x < 0 || params.adj_x >= data->map.column || params.adj_y < 0 || params.adj_y >= data->map.row)
			{
				params.dy += 0.1;
				continue ;
			}
			if (data->map.map[params.adj_y][params.adj_x] == 'D')
			{
				data->map.map[params.adj_y][params.adj_x] = 'd';
				render_scene(data);
			}
			params.dy += 0.1;
		}
		params.dx += 0.1;
	}
}

static void	move_player(t_data *data, double move_x, double move_y)
{
	double	new_player_x;
	double	new_player_y;

	new_player_x = data->player_x + move_x;
	new_player_y = data->player_y + move_y;
	if (!is_wall_at_position(data, new_player_x + 0.5, data->player_y + 0.5))
		data->player_x = new_player_x;
	if (!is_wall_at_position(data, data->player_x + 0.5, new_player_y + 0.5))
		data->player_y = new_player_y;
	check_and_open_door_nearby(data, new_player_x, new_player_y);
}

static void	turn_player(t_data *data, int keycode)
{
	if (keycode == ARROW_LEFT)
		data->player_angle -= TURN_SPEED;
	else if (keycode == ARROW_RIGHT)
		data->player_angle += TURN_SPEED;
	if (data->player_angle < 0)
		data->player_angle += 2 * M_PI;
	else if (data->player_angle >= 2 * M_PI)
		data->player_angle -= 2 * M_PI;
}

void	update_player_position(t_data *data, int keycode)
{
	double	move_x;
	double	move_y;
	double	strafe_x;
	double	strafe_y;

	move_x = cos(data->player_angle) * MOVE_SPEED;
	move_y = sin(data->player_angle) * MOVE_SPEED;
	strafe_x = cos(data->player_angle + M_PI_2) * MOVE_SPEED;
	strafe_y = sin(data->player_angle + M_PI_2) * MOVE_SPEED;
	if (keycode == KEY_W)
		move_player(data, move_x, move_y);
	else if (keycode == KEY_S)
		move_player(data, -move_x, -move_y);
	else if (keycode == KEY_A)
		move_player(data, -strafe_x, -strafe_y);
	else if (keycode == KEY_D)
		move_player(data, strafe_x, strafe_y);
	else
		turn_player(data, keycode);
}
