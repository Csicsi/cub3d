/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 12:32:37 by icseri            #+#    #+#             */
/*   Updated: 2024/12/06 15:44:07 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

bool	is_in(int row, int col, t_map *map)
{
	if (row == 0 || col == 0 || row == map->row - 1 || col == map->column - 1
		|| map->map[row][col - 1] == ' ' || map->map[row][col + 1] == ' '
		|| map->map[row - 1][col] == ' ' || map->map[row + 1][col] == ' ')
		return (false);
	return (true);
}

bool	door_is_good(int row, int col, t_map *map)
{
	bool	east;
	bool	west;
	bool	south;
	bool	north;

	east = map->map[row][col + 1] == '1';
	west = map->map[row][col - 1] == '1';
	south = map->map[row + 1][col] == '1';
	north = map->map[row - 1][col] == '1';
	if (is_in(row, col, map) == false
		|| (east && (west == false || south || north))
		|| (north && (south == false || east || west))
		|| (east == false && north == false))
		return (false);
	return (true);
}

bool	is_valid(int row, int col, int *player_count, t_map *map)
{
	if (!ft_strchr("WENS01XD ", map->map[row][col]))
		return (false);
	else if (!ft_strchr("D1 ", map->map[row][col]))
	{
		if (map->map[row][col] == 'X')
		{
			map->enemy[0] = row;
			map->enemy[1] = col;
		}
		else if (map->map[row][col] != '0')
		{
			(*player_count)++;
			if (*player_count > 1)
				return (false);
			map->player[0] = row;
			map->player[1] = col;
		}
		return (is_in(row, col, map));
	}
	else if (map->map[row][col] == 'D')
		return (door_is_good(row, col, map));
	return (true);
}

void	check_map(t_map *map)
{
	int		player_count;
	int		row;
	int		column;

	player_count = 0;
	row = -1;
	while (map->map[++row])
	{
		column = -1;
		while (map->map[row][++column])
		{
			if (!is_valid(row, column, &player_count, map))
				safe_exit(map, COLOR);
		}
	}
	if (player_count == 0)
		safe_exit(map, COLOR);
}

void	list_to_arr(t_list **map_list, t_map *map)
{
	int		row;
	t_list	*current;

	map->map = ft_calloc(sizeof(char *), map->row + 1);
	if (!map->map)
		return (free_list(map_list), safe_exit(map, MALLOC));
	current = *map_list;
	while (current)
	{
		if ((int)(ft_strlen(current->content)) > map->column)
			map->column = ft_strlen(current->content);
		current = current->next;
	}
	current = *map_list;
	row = -1;
	while (current)
	{
		map->map[++row] = ft_calloc(map->column + 1, 1);
		if (!map->map[row])
			return (free_list(map_list), safe_exit(map, MALLOC));
		ft_memset(map->map[row], ' ', map->column);
		ft_memcpy(map->map[row], current->content, ft_strlen(current->content));
		current = current->next;
	}
	free_list(map_list);
}
