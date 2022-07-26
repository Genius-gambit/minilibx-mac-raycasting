#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "minilibx/mlx.h"
#include <math.h>
#define W 13
#define D 2
#define A 0
#define S 1
#define P 35
#define ARROW_L 123
#define ARROW_R 124
#define ESC 53
#define PI 3.14159265358979323846

typedef struct s_wall
{
	float	angle;
	float	x;
	float	y;
	int		count;
	int		row;
	int		col;
	int		wall_front;
	int		wall_back;
	int		wall_right;
	int		wall_left;
}				t_wall;

typedef struct s_rays
{
	float	dist;
	float	ang;
	float	x;
	float	y;
	float	width;
	float	height;
}				t_rays;

typedef struct s_player
{
	float	x_co;
	float	y_co;
	float	ang;
	float	dx;
	float	dy;
	t_wall	wall;
}				t_player;

typedef struct s_vars
{
	char		**map;
	void		*mlx;
	void		*win;
	void		*img;
	void		*addr;
	int			width;
	int			height;
	int			pause;
	t_player	p;
	t_rays		rays[60];
}				t_vars;

void	ft_putstr_map(char **map)
{
	int	i;

	i = 0;
	while (map[i] != NULL)
	{
		printf("%s\n", map[i]);
		i++;
	}
	printf("\n");
}

void	draw_point(t_vars *vars, float x, float y)
{
	int	i;
	int	j;

	i = 5;
	j = 3;
	while (j > 0)
	{
		i = 3;
		while (i > 0)
		{
			mlx_pixel_put(vars->mlx, vars->win, x + i, y + j, 0x00FF00);
			i--;
		}
		j--;
	}
}

char	**get_map2d(void)
{
	char	**map;

	map = malloc(sizeof(char *) * 9);
	map[0] = strdup("111");
	map[1] = strdup("10000001");
	map[2] = strdup("10000001");
	map[3] = strdup("10000001");
	map[4] = strdup("1001P101");
	map[5] = strdup("10001001");
	map[6] = strdup("10000001");
	map[7] = strdup("11111111");
	map[8] = NULL;
	return (map);
}

void	init_rays(t_vars *vars)
{
	int	i;

	i = 0;
	while (i < 60)
	{
		vars->rays[i].ang = 0;
		vars->rays[i].dist = 0;
		vars->rays[i].height = 0;
		vars->rays[i].width = 0;
		vars->rays[i].x = 0;
		vars->rays[i].y = 0;
		i++;
	}
}

void	init(t_vars *vars)
{
	vars->addr = NULL;
	vars->height = 0;
	vars->img = NULL;
	vars->map = NULL;
	vars->mlx = NULL;
	vars->width = 0;
	vars->win = NULL;
	vars->p.x_co = 0;
	vars->p.y_co = 0;
	vars->p.ang = 0;
	vars->p.dx = 0;
	vars->p.dy = 0;
	vars->pause = 0;
	vars->p.wall.wall_back = 0;
	vars->p.wall.wall_front = 0;
	vars->p.wall.wall_left = 0;
	vars->p.wall.wall_right = 0;
	init_rays(vars);
}

void	init_wall_coll(t_vars *vars)
{
	vars->p.wall.count = 7;
	vars->p.wall.row = 0;
	vars->p.wall.col = 0;
	vars->p.wall.x = 0;
	vars->p.wall.y = 0;
	vars->p.wall.angle = 0;
}

void	wall_coll_forwrd(t_vars *vars)
{
	init_wall_coll(vars);
	vars->p.wall.wall_front = 0;
	vars->p.wall.angle = vars->p.ang - (30 * PI / 180);
	if (vars->p.wall.angle < 0)
		vars->p.wall.angle += 2 * PI;
	while (vars->p.wall.count > 0)
	{
		vars->p.wall.x = vars->p.x_co + (cos(vars->p.wall.angle) * 0.1);
		vars->p.wall.y = vars->p.y_co + (sin(vars->p.wall.angle) * 0.1);
		vars->p.wall.row = (int)vars->p.wall.x;
		vars->p.wall.col = (int)vars->p.wall.y;
		if (!vars->map[vars->p.wall.col][vars->p.wall.row]
			|| (vars->map[vars->p.wall.col][vars->p.wall.row]
			&& vars->map[vars->p.wall.col][vars->p.wall.row] == '1'))
			vars->p.wall.wall_front = 1;
		draw_point(vars, vars->p.wall.x * 155, vars->p.wall.y * 155);
		vars->p.wall.angle += (10 * PI / 180);
		if (vars->p.wall.angle > 2 * PI)
			vars->p.wall.angle -= 2 * PI;
		vars->p.wall.count--;
	}
}

void	wall_coll_bckwrd(t_vars *vars)
{
	init_wall_coll(vars);
	vars->p.wall.wall_back = 0;
	vars->p.wall.angle = vars->p.ang - (30 * PI / 180) - PI;
	while (vars->p.wall.count > 0)
	{
		vars->p.wall.x = vars->p.x_co + (cos(vars->p.wall.angle) * 0.1);
		vars->p.wall.y = vars->p.y_co + (sin(vars->p.wall.angle) * 0.1);
		vars->p.wall.row = (int)vars->p.wall.x;
		vars->p.wall.col = (int)vars->p.wall.y;
		if (!vars->map[vars->p.wall.col][vars->p.wall.row]
			|| (vars->map[vars->p.wall.col][vars->p.wall.row]
			&& vars->map[vars->p.wall.col][vars->p.wall.row] == '1'))
			vars->p.wall.wall_back = 1;
		draw_point(vars, vars->p.wall.x * 155, vars->p.wall.y * 155);
		vars->p.wall.angle += (10 * PI / 180);
		if (vars->p.wall.angle > 2 * PI)
			vars->p.wall.angle -= 2 * PI;
		vars->p.wall.count--;
	}
}

void	wall_coll_left(t_vars *vars)
{
	init_wall_coll(vars);
	vars->p.wall.wall_left = 0;
	vars->p.wall.angle = vars->p.ang - (30 * PI / 180);
	if (vars->p.wall.angle < 0)
		vars->p.wall.angle += 2 * PI;
	vars->p.wall.angle += (3 * PI / 2);
	if (vars->p.wall.angle > 2 * PI)
		vars->p.wall.angle -= 2 * PI;
	while (vars->p.wall.count > 0)
	{
		vars->p.wall.x = vars->p.x_co + (cos(vars->p.wall.angle) * 0.1);
		vars->p.wall.y = vars->p.y_co + (sin(vars->p.wall.angle) * 0.1);
		vars->p.wall.row = (int)vars->p.wall.x;
		vars->p.wall.col = (int)vars->p.wall.y;
		if (!vars->map[vars->p.wall.col][vars->p.wall.row]
			|| (vars->map[vars->p.wall.col][vars->p.wall.row]
			&& vars->map[vars->p.wall.col][vars->p.wall.row] == '1'))
			vars->p.wall.wall_left = 1;
		draw_point(vars, vars->p.wall.x * 155, vars->p.wall.y * 155);
		vars->p.wall.angle += (10 * PI / 180);
		if (vars->p.wall.angle > 2 * PI)
			vars->p.wall.angle -= 2 * PI;
		vars->p.wall.count--;
	}
}

void	wall_coll_right(t_vars *vars)
{
	init_wall_coll(vars);
	vars->p.wall.wall_right = 0;
	vars->p.wall.angle = vars->p.ang - (30 * PI / 180);
	if (vars->p.wall.angle < 0)
		vars->p.wall.angle += 2 * PI;
	vars->p.wall.angle += (PI / 2);
	if (vars->p.wall.angle > 2 * PI)
		vars->p.wall.angle -= 2 * PI;
	while (vars->p.wall.count > 0)
	{
		vars->p.wall.x = vars->p.x_co + (cos(vars->p.wall.angle) * 0.1);
		vars->p.wall.y = vars->p.y_co + (sin(vars->p.wall.angle) * 0.1);
		vars->p.wall.row = (int)vars->p.wall.x;
		vars->p.wall.col = (int)vars->p.wall.y;
		if (!vars->map[vars->p.wall.col][vars->p.wall.row]
			|| (vars->map[vars->p.wall.col][vars->p.wall.row]
			&& vars->map[vars->p.wall.col][vars->p.wall.row] == '1'))
			vars->p.wall.wall_right = 1;
		draw_point(vars, vars->p.wall.x * 155, vars->p.wall.y * 155);
		vars->p.wall.angle += (10 * PI / 180);
		if (vars->p.wall.angle > 2 * PI)
			vars->p.wall.angle -= 2 * PI;
		vars->p.wall.count--;
	}
}

int	handle_wall_collision(t_vars *vars)
{
	wall_coll_forwrd(vars);
	wall_coll_bckwrd(vars);
	wall_coll_left(vars);
	wall_coll_right(vars);
	return (0);
}

int	check_wall(t_vars *vars, float x, float y)
{
	int	col;
	int	row;

	if ((y >= 0) && x < strlen(vars->map[(int)(y)]))
	{
		row = x;
		col = y;
		if (vars->map[col][row] == '1')
			return (1);
		return (0);
	}
	return (1);
}

float	sq(float num)
{
	return (num * num);
}

float	get_dist(float x_one, float y_one, float x_two, float y_two)
{
	float	dist;

	dist = sqrt(sq(x_two - x_one) + sq(y_two - y_one));
	return (dist);
}

void	print_points(float x, float y)
{
	printf("X: %f, Y: %f\n", x, y);
}

void	print_rays(t_vars *vars)
{
	float	angle;
	float	x;
	float	y;
	int		wall;
	int		count;

	angle = vars->p.ang - (PI / 6);
	if (angle < 0)
		angle += 2 * PI;
	count = 60;
	while (count > 0)
	{
		x = vars->p.x_co;
		y = vars->p.y_co;
		wall = 0;
		while (!wall)
		{
			x += (cos(angle) * 0.1);
			y += (sin(angle) * 0.1);
			wall = check_wall(vars, x, y);
		}
		// draw_point(vars, x * 155, y * 155);
		// print_points(x, y);
		// print_points(vars->p.x_co, vars->p.y_co);
		// printf("Dist: %f\n", get_dist(vars->p.x_co, vars->p.y_co, x, y));
		angle += (PI / 180);
		count--;
	}
	// printf("X: %f, Y: %f\n", x, y);
}

void	free_map(char **map)
{
	int	i;

	i = 0;
	while (map[i] != NULL)
	{
		free(map[i]);
		i++;
	}
	free(map);
}

void	exit_game(t_vars *vars)
{
	mlx_destroy_window(vars->mlx, vars->win);
	free_map(vars->map);
	exit(0);
}

void	move_frwrd(t_vars *vars)
{
	int	row;
	int	col;

	vars->p.dx = cos(vars->p.ang) * 0.0556;
	vars->p.dy = sin(vars->p.ang) * 0.0556;
	if ((vars->p.y_co + vars->p.dy >= 0)
		&& vars->p.x_co + vars->p.dx
		< strlen(vars->map[(int)(vars->p.y_co + vars->p.dy)]))
	{
		row = vars->p.x_co + vars->p.dx;
		col = vars->p.y_co + vars->p.dy;
		if (vars->map[col][row] == '1' || vars->p.wall.wall_front)
		{
			draw_point(vars, (vars->p.x_co * 155), (vars->p.y_co * 155));
			return ;
		}
		vars->map[(int)vars->p.y_co][(int)vars->p.x_co] = '0';
		vars->p.x_co += vars->p.dx;
		vars->p.y_co += vars->p.dy;
		vars->map[col][row] = 'P';
	}
}

void	move_right(t_vars *vars)
{
	int	row;
	int	col;

	vars->p.dx = cos((PI / 2) + (vars->p.ang)) * 0.0556;
	vars->p.dy = sin((PI / 2) + (vars->p.ang)) * 0.0556;
	if ((vars->p.y_co + vars->p.dy >= 0)
		&& vars->p.x_co + vars->p.dx
		< strlen(vars->map[(int)(vars->p.y_co + vars->p.dy)]))
	{
		row = vars->p.x_co + vars->p.dx;
		col = vars->p.y_co + vars->p.dy;
		if (vars->map[col][row] == '1' || vars->p.wall.wall_right)
		{
			draw_point(vars, (vars->p.x_co * 155), (vars->p.y_co * 155));
			return ;
		}
		vars->map[(int)vars->p.y_co][(int)vars->p.x_co] = '0';
		vars->p.x_co += vars->p.dx;
		vars->p.y_co += vars->p.dy;
		vars->map[col][row] = 'P';
	}
}

void	move_bckwrd(t_vars *vars)
{
	int	row;
	int	col;

	vars->p.dx = cos(vars->p.ang) * 0.0556;
	vars->p.dy = sin(vars->p.ang) * 0.0556;
	if ((vars->p.y_co - vars->p.dy >= 0)
		&& vars->p.x_co - vars->p.dx
		< strlen(vars->map[(int)(vars->p.y_co - vars->p.dy)]))
	{
		row = vars->p.x_co - vars->p.dx;
		col = vars->p.y_co - vars->p.dy;
		if (vars->map[col][row] == '1' || vars->p.wall.wall_back)
		{
			draw_point(vars, (vars->p.x_co * 155), (vars->p.y_co * 155));
			return ;
		}
		vars->map[(int)vars->p.y_co][(int)vars->p.x_co] = '0';
		vars->p.x_co -= vars->p.dx;
		vars->p.y_co -= vars->p.dy;
		vars->map[col][row] = 'P';
	}
}

void	move_left(t_vars *vars)
{
	int	row;
	int	col;

	vars->p.dx = cos((PI / 2) + (vars->p.ang)) * 0.0556;
	vars->p.dy = sin((PI / 2) + (vars->p.ang)) * 0.0556;
	if ((vars->p.y_co - vars->p.dy >= 0)
		&& vars->p.x_co - vars->p.dx
		< strlen(vars->map[(int)(vars->p.y_co - vars->p.dy)]))
	{
		row = vars->p.x_co - vars->p.dx;
		col = vars->p.y_co - vars->p.dy;
		if (vars->map[col][row] == '1' || vars->p.wall.wall_left)
		{
			draw_point(vars, (vars->p.x_co * 155), (vars->p.y_co * 155));
			return ;
		}
		vars->map[(int)vars->p.y_co][(int)vars->p.x_co] = '0';
		vars->p.x_co -= vars->p.dx;
		vars->p.y_co -= vars->p.dy;
		vars->map[col][row] = 'P';
	}
}

void	print_window(t_vars *vars)
{
	int	j;
	int	i;
	int	col;

	j = 0;
	col = 0x00AAFF;
	while (j < 1240)
	{
		i = 0;
		if (j == 620)
			col = 0XFFAA55;
		while (i < 1240)
		{
			mlx_pixel_put(vars->mlx, vars->win, i, j, col);
			i++;
		}
		j++;
	}
}

void	move_player(int keycode, t_vars *vars)
{
	if (keycode == ESC)
		exit_game(vars);
	// else if (keycode == W)
	// 	move_frwrd(vars);
	// else if (keycode == S)
	// 	move_bckwrd(vars);
	// else if (keycode == A)
	// 	move_left(vars);
	// else if (keycode == D)
	// 	move_right(vars);
	// else if (keycode == P)
	// {
	// 	if (vars->pause == 1)
	// 		vars->pause = 0;
	// 	else
	// 		vars->pause = 1;
	// }
}

void	rotate_player(int keycode, t_vars *vars)
{
	if (keycode == ARROW_R)
		vars->p.ang += 0.1;
	if (vars->p.ang > 2 * PI)
		vars->p.ang -= 2 * PI;
	if (keycode == ARROW_L)
		vars->p.ang -= 0.1;
	if (vars->p.ang < 2 * PI)
		vars->p.ang += 2 * PI;
}

int	key_hook(int keycode, t_vars *vars)
{
	vars->img = mlx_new_image(vars->mlx, 1240, 1240);
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img, 0, 0);
	mlx_destroy_image(vars->mlx, vars->img);
	print_window(vars);
	move_player(keycode, vars);
	rotate_player(keycode, vars);
	draw_point(vars, (vars->p.x_co * 155), (vars->p.y_co * 155));
	handle_wall_collision(vars);
	print_rays(vars);
	ft_putstr_map(vars->map);
	return (0);
}

int	mouse_hook(int x, int y, t_vars *vars)
{
	if (vars->pause == 0)
	{
		mlx_mouse_get_pos(vars->win, &x, &y);
		if (x < 350)
			key_hook(ARROW_L, vars);
		else if (x > 350)
			key_hook(ARROW_R, vars);
		mlx_mouse_move(vars->win, 350, 350);
		mlx_mouse_hide(vars->mlx, vars->win);
	}
	else
		mlx_mouse_show(vars->mlx, vars->win);
	return (0);
}

void	print_player(t_vars *vars)
{
	int	j;
	int	i;

	j = 0;
	vars->p.ang = 0 * (PI / 180);
	while (vars->map[j] != NULL)
	{
		i = 0;
		while (vars->map[j][i])
		{
			if (vars->map[j][i] == 'P')
				break ;
			i++;
		}
		if (vars->map[j][i] == 'P')
			break ;
		j++;
	}
	vars->p.x_co = i + 0.5;
	vars->p.y_co = j + 0.5;
	draw_point(vars, (vars->p.x_co * 155), (vars->p.y_co * 155));
	handle_wall_collision(vars);
}

int	main(void)
{
	t_vars	vars;

	init(&vars);
	vars.map = get_map2d();
	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, 1240, 1240, "Cub3D");
	print_window(&vars);
	// print_player(&vars);
	// print_rays(&vars);
	mlx_hook(vars.win, 2, (1L << 0), key_hook, &vars);
	// mlx_hook(vars.win, 6, (1L << 6), mouse_hook, &vars);
	mlx_loop(vars.mlx);
	return (0);
}
