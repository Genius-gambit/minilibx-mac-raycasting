#include "minilibx/mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define ESC 65307
#define W 119
#define D 100
#define E 101
#define S 115
#define A 97
#define A_R 65363
#define A_L 65361
#define PI 3.141592653589793238

typedef struct s_rays
{
	float x_ray_l;
	float y_ray_l;
	float ang_ray_l;
	float x_ray_r;
	float y_ray_r;
	float ang_ray_r;
} t_rays;

typedef struct s_player
{
	float x;
	float y;
	float ang;
	float dx;
	float dy;
	int	wall_frwrd;
	int	wall_bckwrd;
	t_rays r;
} t_player;

typedef struct s_vars
{
	void *mlx;
	void *win;
	int width;
	int height;
	int rows;
	int cols;
	char **map;
	t_player p;
} t_vars;

void draw_point(t_vars *vars, float x, float y)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (i < 2)
	{
		j = 0;
		while (j < 2)
		{
			mlx_pixel_put(vars->mlx, vars->win, x + i, y + j, 0X00FF00);
			j++;
		}
		i++;
	}
}

void draw_line(t_vars *vars)
{
	float slope;
	float incpt;
	float x;
	float y;

	// vars->img = mlx_new_image(vars->mlx, vars->width, vars->height);
	// vars->addr = mlx_get_data_addr(vars->img, &vars->bitspix, &vars->len, &vars->end);
	slope = ((vars->p.y - vars->p.r.y_ray_l) / (vars->p.x - vars->p.r.x_ray_l));
	incpt = vars->p.y - (slope * vars->p.x);
	printf("%f\n", slope);
	if ((vars->p.ang >= (3 * PI / 2)) && (vars->p.ang < (2 * PI)))
	{
		y = vars->p.y - 1;
		x = (y - incpt) / slope;
		while (y >= 0 && y > vars->p.r.y_ray_l && x < vars->width && vars->p.r.y_ray_l < vars->height)
		{
			draw_point(vars, x, y);
			y--;
			x = (y - incpt) / slope;
		}
	}
	// else if ((vars->p.ang >= 0) && (vars->p.ang < (PI / 2)))
	// {
	// 	x = vars->p.x + 1;
	// 	y = (slope * x) + incpt;
	// 	while (x >= 0 && x < vars->p.r.x_ray_l && x < vars->width && y < vars->height)
	// 	{
	// 		draw_point(vars, x, y);
	// 		x++;
	// 		y = (slope * x) + incpt;
	// 	}
	// }
	// else if ((vars->p.ang >= (PI / 2)) && (vars->p.ang < PI))
	// {
	// 	y = vars->p.y + 1;
	// 	x = (y - incpt) / slope;
	// 	while (y >= 0 && y < vars->p.r.y_ray_l && vars->p.r.x_ray_l < vars->width && vars->p.r.y_ray_l < vars->height)
	// 	{
	// 		draw_point(vars, x, y);
	// 		y++;
	// 		x = (y - incpt) / slope;
	// 	}
	// }
	// else if ((vars->p.ang >= PI) && (vars->p.ang < (3 * (PI / 2))))
	// {
	// 	x = vars->p.x - 1;
	// 	y = (slope * x) + incpt;
	// 	while (x >= 0 && x > vars->p.r.x_ray_l && vars->p.r.x_ray_l < vars->width && vars->p.r.y_ray_l < vars->height)
	// 	{
	// 		draw_point(vars, x, y);
	// 		x--;
	// 		y = (slope * x) + incpt;
	// 	}
	// }
	// mlx_put_image_to_window(vars->mlx, vars->win, vars->img, 0, 0);
	// init_window(vars);
}

void draw_rays(t_vars *vars)
{
	vars->p.r.ang_ray_l = vars->p.ang - (PI / 6);
	if (vars->p.r.ang_ray_l < 0)
		vars->p.r.ang_ray_l += 2 * PI;
	vars->p.r.ang_ray_r = vars->p.ang + (PI / 6);
	if (vars->p.r.ang_ray_r > 2 * PI)
		vars->p.r.ang_ray_r -= 2 * PI;
	vars->p.r.x_ray_l = vars->p.x + cos(vars->p.r.ang_ray_l) * 15;
	vars->p.r.y_ray_l = vars->p.y + sin(vars->p.r.ang_ray_l) * 15;
	vars->p.r.x_ray_r = vars->p.x + cos(vars->p.r.ang_ray_r) * 15;
	vars->p.r.y_ray_r = vars->p.y + sin(vars->p.r.ang_ray_r) * 15;
}

void Wall_C_forwrd(t_vars *vars)
{
	float	angle;
	float	x;
	float	y;
	int		count = 7;
	int		row;
	int		col;
	
	
	angle = vars->p.ang - (30 * PI / 180);
	if (angle < 0)
		angle += 2 * PI;
	x = 0;
	row = 0;
	col = 0;
	y = 0;
	vars->p.wall_frwrd = 0;
	// printf("Angle: %f\n", angle * 180 / PI);
	while (count > 0)
	{
		printf("Angle: %f\n", angle * 180 / PI);
		x = vars->p.x + (cos(angle) * 12);
		y = vars->p.y + (sin(angle) * 12);
		row = (int)(x * 8 / vars->width);
		col = (int)(y * 8 / vars->height);
		if (vars->map[col][row] == '1')
			vars->p.wall_frwrd = 1;
		draw_point(vars, x, y);
		angle += (10 * PI / 180);
		if (angle > 2 * PI)
			angle -= 2 * PI;
		count--;
	}
}

void raycasting_up(t_vars *vars)
{
	int	row;
	int	col;
	vars->p.dx = cos(vars->p.ang) * 5;
	vars->p.dy = sin(vars->p.ang) * 5;
	if ((vars->p.x + vars->p.dx > 0) && (vars->p.x + vars->p.dx < vars->width) && (vars->p.y + vars->p.dy > 0) && (vars->p.y + vars->p.dy < vars->height))
	{
		row = (int)((vars->p.x + vars->p.dx) * 8 / vars->width);
		col = (int)((vars->p.y + vars->p.dy) * 8 / vars->height);
		if (vars->map[col][row] == '1' || vars->p.wall_frwrd)
			return ;
		vars->p.x += vars->p.dx;
		vars->p.y += vars->p.dy;
		vars->map[col][row] = '0';
	}
}

void raycasting_down(t_vars *vars)
{
	int	row;
	int	col;
	vars->p.dx = cos(vars->p.ang) * 5;
	vars->p.dy = sin(vars->p.ang) * 5;
	if ((vars->p.x - vars->p.dx > 0) && (vars->p.x - vars->p.dx < vars->width) && (vars->p.y - vars->p.dy > 0) && (vars->p.y - vars->p.dy < vars->height))
	{
		row = (int)((vars->p.x - vars->p.dx) * 8 / vars->width);
		col = (int)((vars->p.y - vars->p.dy) * 8 / vars->height);
		if (vars->map[col][row] == '1' || vars->p.wall_bckwrd)
			return ;
		vars->p.x -= vars->p.dx;
		vars->p.y -= vars->p.dy;
		vars->map[col][row] = '0';
	}
}

void raycasting_right(t_vars *vars)
{
	vars->p.dx = cos((PI / 2) + (vars->p.ang)) * 5;
	vars->p.dy = sin((PI / 2) + (vars->p.ang)) * 5;
	if ((vars->p.x + vars->p.dx > 0) && (vars->p.x + vars->p.dx < vars->width) && (vars->p.y + vars->p.dy > 0) && (vars->p.y + vars->p.dy < vars->height))
	{
		// row = (int)((vars.p.x + vars.p.dx) * 8 / vars->width);
		// col = (int)((vars.p.y + vars.p.dy) * 8 / vars->height);
		// if (vars.maps[col][row] == '1' || vars.p.stop_right)
		// 	return ;
		vars->p.x += vars->p.dx;
		vars->p.y += vars->p.dy;
		// vars.maps[col][row] = '0';
	}
}

void raycasting_left(t_vars *vars)
{
	vars->p.dx = cos((PI / 2) + (vars->p.ang)) * 5;
	vars->p.dy = sin((PI / 2) + (vars->p.ang)) * 5;
	if ((vars->p.x - vars->p.dx > 0) && (vars->p.x - vars->p.dx < vars->width) && (vars->p.y - vars->p.dy > 0) && (vars->p.y - vars->p.dy < vars->height))
	{
		// row = (int)((vars.p.x + vars.p.dx) * 8 / vars->width);
		// col = (int)((vars.p.y + vars.p.dy) * 8 / vars->height);
		// if (vars.maps[col][row] == '1' || vars.p.stop_right)
		// 	return ;
		vars->p.x -= vars->p.dx;
		vars->p.y -= vars->p.dy;
		// vars.maps[col][row] = '0';
	}
}

void move_player(int keycode, t_vars *vars)
{
	if (keycode == W)
		raycasting_up(vars);
	else if (keycode == S)
		raycasting_down(vars);
	else if (keycode == D)
		raycasting_right(vars);
	else if (keycode == A)
		raycasting_left(vars);
}

void rotation(int keycode, t_vars *vars)
{
	if (keycode == A_R)
		vars->p.ang += 0.1;
	if (vars->p.ang > 2 * PI)
		vars->p.ang -= 2 * PI;
	if (keycode == A_L)
		vars->p.ang -= 0.1;
	if (vars->p.ang < 2 * PI)
		vars->p.ang += 2 * PI;
}

int key_hook(int keycode, t_vars *vars)
{
	// printf("Keycode: %d\n", keycode);
	void *img;

	img = mlx_new_image(vars->mlx, vars->width, vars->height);
	mlx_put_image_to_window(vars->mlx, vars->win, img, 0, 0);
	if (keycode == ESC)
		exit(1);
	else if (keycode == E)
		printf("E: Successful\n");
	move_player(keycode, vars);
	rotation(keycode, vars);
	draw_point(vars, vars->p.x, vars->p.y);
	draw_rays(vars);
	draw_point(vars, vars->p.r.x_ray_l, vars->p.r.y_ray_l);
	draw_point(vars, vars->p.r.x_ray_r, vars->p.r.y_ray_r);
	draw_line(vars);
	return (0);
}

void init(t_vars *vars)
{
	vars->p.x = 320;
	vars->p.y = 320;
	vars->p.ang = 3 * (PI / 2);
	vars->p.r.ang_ray_l = 0;
	vars->p.r.ang_ray_r = 0;
	vars->p.r.x_ray_l = 0;
	vars->p.r.x_ray_r = 0;
	vars->p.r.y_ray_l = 0;
	vars->p.r.y_ray_r = 0;
	vars->p.dx = 0;
	vars->p.dy = 0;
	vars->p.wall_frwrd = 0;
	vars->p.wall_bckwrd = 0;
	draw_rays(vars);
}

int handle_no_event(t_vars *vars)
{
	draw_rays(vars);
	Wall_C_forwrd(vars);
}

char **get_map2d(t_vars *vars)
{
	char **map;

	vars->rows = 8;
	vars->cols = 8;
	map = malloc(sizeof(char *) * (vars->cols + 1));
	map[0] = strdup("11111111");
	map[1] = strdup("10000001");
	map[2] = strdup("10000001");
	map[3] = strdup("10000001");
	map[4] = strdup("10000001");
	map[5] = strdup("100000P1");
	map[6] = strdup("10000001");
	map[7] = strdup("11111111");
	map[8] = NULL;
	return (map);
}

void	parse_map(t_vars *vars)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	vars->p.x = 0;
	vars->p.y = 0;
	while (vars->map[i] != NULL)
	{
		j = 0;
		vars->p.x = 0;
		while (vars->map[i][j])
		{
			if (vars->map[i][j] == 'P')
				break;
			vars->p.x += 87.5;
			j++;
		}
		if (vars->map[i][j] == 'P')
			break;
		vars->p.y += 87.5;
		i++;
	}
}

int main(void)
{
	t_vars vars;

	vars.mlx = mlx_init();
	init(&vars);
	vars.width = 700;
	vars.height = 700;
	vars.map = get_map2d(&vars);
	parse_map(&vars);
	draw_rays(&vars);
	vars.win = mlx_new_window(vars.mlx, vars.width, vars.height, "Cub3D");
	draw_point(&vars, vars.p.x, vars.p.y);
	draw_point(&vars, vars.p.r.x_ray_l, vars.p.r.y_ray_l);
	draw_point(&vars, vars.p.r.x_ray_r, vars.p.r.y_ray_r);
	mlx_hook(vars.win, 2, (1L << 0), key_hook, &vars);
	mlx_loop_hook(vars.mlx, handle_no_event, &vars);
	mlx_loop(vars.mlx);
	return (0);
}