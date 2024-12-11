/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcsicsak <dcsicsak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 13:08:57 by icseri            #+#    #+#             */
/*   Updated: 2024/12/11 14:03:17 by dcsicsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	is_wall(t_data *data, double check_x, double check_y)
{
	int	adj_x;
	int	adj_y;

	adj_x = (int)check_x;
	adj_y = (int)check_y;
	if (adj_x < 0 || adj_x >= data->map.column || adj_y < 0
		|| adj_y >= data->map.row)
		return (0);
	if (ft_strchr("1D", data->map.map[adj_y][adj_x]) != 0)
		return (1);
	return (0);
}

static int	check_wall_in_radius(t_data *data, double new_x, double new_y,
	double radius)
{
	t_proximity	params;

	params.radius = radius;
	params.dx = -params.radius;
	while (params.dx <= params.radius)
	{
		params.dy = -params.radius;
		while (params.dy <= params.radius)
		{
			if (sqrt(params.dx * params.dx + params.dy * params.dy)
				> params.radius)
			{
				params.dy += 0.1;
				continue ;
			}
			params.check_x = new_x + params.dx;
			params.check_y = new_y + params.dy;
			if (is_wall(data, params.check_x, params.check_y))
				return (1);
			params.dy += 0.1;
		}
		params.dx += 0.1;
	}
	return (0);
}

static int	is_wall_at_position(t_data *data, double new_x, double new_y)
{
	int	map_x;
	int	map_y;

	map_x = (int)new_x;
	map_y = (int)new_y;
	if (map_x < 0 || map_x >= data->map.column || map_y < 0
		|| map_y >= data->map.row)
		return (1);
	return (check_wall_in_radius(data, new_x, new_y, 0.2));
}

void	check_door_at_position(t_data *data, int adj_x, int adj_y)
{
	if (adj_x < 0 || adj_x >= data->map.column || adj_y < 0
		|| adj_y >= data->map.row)
		return ;
	if (data->map.map[adj_y][adj_x] == 'D')
	{
		data->map.map[adj_y][adj_x] = 'd';
		render_scene(data);
	}
}

void	check_door_within_radius(t_data *data, double new_x, double new_y,
	t_proximity *params)
{
	int		adj_x;
	int		adj_y;

	params->dx = -params->radius;
	while (params->dx <= params->radius)
	{
		params->dy = -params->radius;
		while (params->dy <= params->radius)
		{
			if (sqrt(params->dx * params->dx + params->dy * params->dy)
				> params->radius)
			{
				params->dy += 0.1;
				continue ;
			}
			params->check_x = new_x + params->dx;
			params->check_y = new_y + params->dy;
			adj_x = (int)params->check_x;
			adj_y = (int)params->check_y;
			check_door_at_position(data, adj_x, adj_y);
			params->dy += 0.1;
		}
		params->dx += 0.1;
	}
}

void	check_and_open_door_nearby(t_data *data, double new_x, double new_y)
{
	t_proximity	params;
	int			map_x;
	int			map_y;

	map_x = (int)new_x;
	map_y = (int)new_y;
	if (map_x < 0 || map_x >= data->map.column || map_y < 0
		|| map_y >= data->map.row)
		return ;
	params.radius = 1.5;
	check_door_within_radius(data, new_x, new_y, &params);
}

static void	check_and_close_doors(t_data *data)
{
	double	player_x;
	double	player_y ;
	double	dist;
	int		x;
	int		y;

	player_x = data->player_x;
	player_y = data->player_y;
	y = -1;
	while (++y < data->map.row)
	{
		x = -1;
		while (++x < data->map.column)
		{
			if (data->map.map[y][x] == 'd')
			{
				dist = sqrt(pow(player_x - x, 2) + pow(player_y - y, 2));
				if (dist > 1.5)
					data->map.map[y][x] = 'D';
			}
		}
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
	check_and_close_doors(data);
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