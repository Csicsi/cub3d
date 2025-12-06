/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcsicsak <dcsicsak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 13:01:37 by icseri            #+#    #+#             */
/*   Updated: 2025/12/06 14:34:11 by dcsicsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_rectangle(t_data *data, t_rectangle *rect)
{
	int	i;
	int	j;

	j = 0;
	while (j <= rect->height)
	{
		i = 0;
		while (i <= rect->width)
		{
			if (j == 0 || j == rect->height || i == 0 || i == rect->width)
				my_mlx_pixel_put(data, rect->x + i, rect->y + j, GRID_COLOR);
			else
				my_mlx_pixel_put(data, rect->x + i, rect->y + j, rect->color);
			i++;
		}
		j++;
	}
}

static void	draw_line_with_params(t_data *data,
	t_line *line, t_line_params *params)
{
	while (1)
	{
		my_mlx_pixel_put(data, line->start_x, line->start_y, line->color);
		if (line->start_x == line->end_x && line->start_y == line->end_y)
			break ;
		params->error2 = params->error * 2;
		if (params->error2 > -params->delta_y)
		{
			params->error -= params->delta_y;
			line->start_x += params->step_x;
		}
		if (params->error2 < params->delta_x)
		{
			params->error += params->delta_x;
			line->start_y += params->step_y;
		}
	}
}

void	draw_line(t_data *data, t_line *line)
{
	t_line_params	params;

	params.delta_x = abs(line->end_x - line->start_x);
	params.delta_y = abs(line->end_y - line->start_y);
	if (line->start_x < line->end_x)
		params.step_x = 1;
	else
		params.step_x = -1;
	if (line->start_y < line->end_y)
		params.step_y = 1;
	else
		params.step_y = -1;
	params.error = params.delta_x - params.delta_y;
	draw_line_with_params(data, line, &params);
}

void	draw_texture_column(double step, int x, int *wall, t_texture *texture)
{
	int			texture_y;
	int			texture_x;
	int			y;
	uint32_t	color;
	uint32_t	ceiling;
	uint32_t	floor_color;

	ceiling = texture->data_struct->map.ceiling | 0xFF000000;
	floor_color = texture->data_struct->map.floor | 0xFF000000;
	y = 0;
	while (y < wall[0])
		my_mlx_pixel_put(texture->data_struct, x, y++, ceiling);
	while (y <= wall[1])
	{
		if (texture->height == 0 || texture->width == 0 || !texture->img)
			break ;
		texture_y = (int)texture->position % texture->height;
		texture_x = ((int)(texture->data_struct->texture_x[x] * texture->width)
				% texture->width);
		if (texture_y < 0)
			texture_y = 0;
		if (texture_x < 0)
			texture_x = 0;
		color = ((uint32_t *)texture->img->pixels)[texture_y
			* texture->width + texture_x];
		my_mlx_pixel_put(texture->data_struct, x, y++, color);
		texture->position += step;
	}
	while (y < HEIGHT)
		my_mlx_pixel_put(texture->data_struct, x, y++, floor_color);
}

void	draw_vertical_line(t_data *data, int x, int wall_height)
{
	int			wall[2];
	double		step;
	int			original_wall_top;
	t_texture	*texture;
	int			frame_idx;

	if (data->ray_dir[x] < 0 || data->ray_dir[x] >= 5)
		data->ray_dir[x] = NORTH;
	if (data->frame_count[data->ray_dir[x]] == 0)
		return ;
	frame_idx = data->frame % data->frame_count[data->ray_dir[x]];
	texture = &data->textures[data->ray_dir[x]][frame_idx];
	if (!texture || !texture->img || texture->height == 0 || wall_height <= 0)
		return ;
	texture->data_struct = data;
	wall[0] = (HEIGHT - wall_height) / 2;
	wall[1] = wall[0] + wall_height - 1;
	original_wall_top = wall[0];
	if (wall[0] < 0)
		wall[0] = 0;
	if (wall[1] >= HEIGHT)
		wall[1] = HEIGHT - 1;
	step = (double)texture->height / wall_height;
	if (original_wall_top < 0)
		texture->position = -original_wall_top * step;
	else
		texture->position = 0;
	draw_texture_column(step, x, wall, texture);
}
