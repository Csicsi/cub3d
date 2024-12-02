#include "../includes/cub3d.h"

static int	is_wall_at_position(t_data *data, double new_x, double new_y)
{
	int	map_x;
	int	map_y;

	map_x = (int)new_x;
	map_y = (int)new_y;
	if (map_x < 0 || map_x >= data->map_width
		|| map_y < 0 || map_y >= data->map_height)
		return (1);
	if (data->map[map_y][map_x] == '1')
		return (1);
	return (0);
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

int	key_hook(int keycode, t_data *data)
{
	if (keycode == KEY_ESC)
		close_window(data);
	mlx_destroy_image(data->mlx, data->img);
	data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	if (!data->img)
	{
		printf("Error: Failed to recreate image.\n");
		close_window(data);
	}
	data->addr = mlx_get_data_addr(data->img, &data->bpp,
			&data->line_len, &data->endian);
	update_player_position(data, keycode);
	cast_rays(data);
	render_scene(data);
	draw_minimap(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	return (0);
}