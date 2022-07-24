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
	float x_map_co;
	float y_map_co;
	float	x_co;
	float	y_co;
	float ang;
	float dx;
	float dy;
	int	wall_frwrd;
	int	wall_bckwrd;
	int	wall_right;
	int	wall_left;
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
	int	bpp;
	int	len;
	int	end;
	void	*addr;
	t_player p;
} t_vars;

void	free_all(char **map)
{
	int	i;

	i = 0;
	while (map[i] != NULL)
	{
		free(map[i]);
		i++;
	}
}

void	ft_putstr_map(char **map)
{
	int	i;

	i = 0;
	while (map[i] != NULL)
	{
		printf("%s\n", map[i]);
		i++;
	}
}

void	init_window(t_vars *vars)
{
	int	i;
	int	x;
	int	y;
	int	j;
	void	*img;
	int	width;
	int	height;

	i = 0;
	j = 0;
	x = 0;
	y = 0;
	img = mlx_xpm_file_to_image(vars->mlx, "xpm_files/red.xpm", &width, &height);
	while (vars->map[i] != NULL)
	{
		j = 0;
		x = 0;
		while (vars->map[i][j])
		{
			if (vars->map[i][j] == '1')
			{
				mlx_put_image_to_window(vars->mlx, vars->win, img, x, y);
			}
			x += 88;
			j++;
		}
		y += 88;
		i++;
	}
	mlx_destroy_image(vars->mlx, img);
}

// void	my_pixel_put(t_vars *vars, int x, int y, int rgb)
// {
// 	char	*draw;

// 	draw = vars->addr
// 		+ (y * vars->len + x * (vars->bpp / 8));
// 	*(unsigned int *)draw = rgb;
// }

void draw_point(t_vars *vars, float x, float y)
{
	int i;
	int j;

	i = 0;
	j = 0;
	// while (i < 5)
	// {
	// 	j = 0;
	// 	while (j < 5)
	// 	{
		if ((x >= 0 && x <= vars->width) && (y >= 0 && y <= vars->height))
			mlx_pixel_put(vars->mlx, vars->win, x, y, 0x00FF00);
			// my_pixel_put(vars, x, y, 0X00FF00);
	// 		j++;
	// 	}
	// 	i++;
	// }
}

// void draw_line(t_vars *vars)
// {
// 	float slope;
// 	float incpt;
// 	float x;
// 	float y;

// 	// vars->img = mlx_new_image(vars->mlx, vars->width, vars->height);
// 	// vars->addr = mlx_get_data_addr(vars->img, &vars->bitspix, &vars->len, &vars->end);
// 	slope = ((vars->p.y - vars->p.r.y_ray_l) / (vars->p.x - vars->p.r.x_ray_l));
// 	incpt = vars->p.y - (slope * vars->p.x);
// 	printf("%f\n", slope);
// 	if ((vars->p.ang >= (3 * PI / 2)) && (vars->p.ang < (2 * PI)))
// 	{
// 		y = vars->p.y - 1;
// 		x = (y - incpt) / slope;
// 		while (y >= 0 && y > vars->p.r.y_ray_l && x < vars->width && vars->p.r.y_ray_l < vars->height)
// 		{
// 			draw_point(vars, x, y);
// 			y--;
// 			x = (y - incpt) / slope;
// 		}
// 	}
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
// }

void draw_rays(t_vars *vars)
{
	vars->p.r.ang_ray_l = vars->p.ang - (PI / 6);
	if (vars->p.r.ang_ray_l < 0)
		vars->p.r.ang_ray_l += 2 * PI;
	vars->p.r.ang_ray_r = vars->p.ang + (PI / 6);
	if (vars->p.r.ang_ray_r > 2 * PI)
		vars->p.r.ang_ray_r -= 2 * PI;
	vars->p.r.x_ray_l = (vars->p.x_co + cos(vars->p.r.ang_ray_l) * 0.5) * 87.5;
	vars->p.r.y_ray_l = (vars->p.y_co + sin(vars->p.r.ang_ray_l) * 0.5) * 87.5;
	vars->p.r.x_ray_r = (vars->p.x_co + cos(vars->p.r.ang_ray_r) * 0.5) * 87.5;
	vars->p.r.y_ray_r = (vars->p.y_co + sin(vars->p.r.ang_ray_r) * 0.5) * 87.5;
	draw_point(vars, vars->p.r.x_ray_l, vars->p.r.y_ray_l);
	draw_point(vars, vars->p.r.x_ray_r, vars->p.r.y_ray_r);
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
	// printf("Angle: %f\n", angle);
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
		// printf("Angle: %f\n", angle * 180 / PI);
		x = vars->p.x_co + (cos(angle) * 0.1);
		y = vars->p.y_co + (sin(angle) * 0.1);
		row = (int)x;
		col = (int)y;
		if (!vars->map[col][row] || (vars->map[col][row] && vars->map[col][row] == '1'))
			vars->p.wall_frwrd = 1;
		draw_point(vars, x * 87.5, y * 87.5);
		angle += (10 * PI / 180);
		if (angle > 2 * PI)
			angle -= 2 * PI;
		count--;
	}
}

void Wall_C_bckwrd(t_vars *vars)
{
	float	angle;
	float	x;
	float	y;
	int		count = 7;
	int		row;
	int		col;
	
	
	angle = vars->p.ang - (30 * PI / 180) - PI;
	if (angle < 0)
		angle += 2 * PI;
	x = 0;
	row = 0;
	col = 0;
	y = 0;
	vars->p.wall_bckwrd = 0;
	while (count > 0)
	{
		// printf("Angle: %f\n", angle * 180 / PI);
		x = vars->p.x_co + (cos(angle) * 0.1);
		y = vars->p.y_co + (sin(angle) * 0.1);
		row = (int)x;
		col = (int)y;
		if (!vars->map[col][row] || (vars->map[col][row] && vars->map[col][row] == '1'))
			vars->p.wall_bckwrd = 1;
		draw_point(vars, x * 87.5, y * 87.5);
		angle += (10 * PI / 180);
		if (angle > 2 * PI)
			angle -= 2 * PI;
		count--;
	}
}

void Wall_C_left(t_vars *vars)
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
	angle += (3 * PI / 2);
	if (angle > 2 * PI)
		angle -= 2 * PI;
	x = 0;
	row = 0;
	col = 0;
	y = 0;
	// glColor3f(1, 0, 1);
	// glPointSize(2);
	vars->p.wall_left = 0;
	// printf("Angle: %f\n", angle * 180 / PI);
	while (count > 0)
	{
		// printf("Angle: %f\n", angle * 180 / PI);
		x = vars->p.x_co + (cos(angle) * 0.1);
		y = vars->p.y_co + (sin(angle) * 0.1);
		row = (int)x;
		col = (int)y;
		if (!vars->map[col][row] || (vars->map[col][row] && vars->map[col][row] == '1'))
			vars->p.wall_left = 1;
		// glBegin(GL_POINTS);
		// glVertex2f(x, y);
		// glEnd();
		draw_point(vars, x * 87.5, y * 87.5);
		angle += (10 * PI / 180);
		if (angle > 2 * PI)
			angle -= 2 * PI;
		count--;
	}
}

void Wall_C_right(t_vars *vars)
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
	angle += (PI / 2);
	if (angle > 2 * PI)
		angle -= 2 * PI;
	x = 0;
	row = 0;
	col = 0;
	y = 0;
	// glColor3f(1, 0, 1);
	// glPointSize(2);
	vars->p.wall_right = 0;
	// printf("Angle: %f\n", angle * 180 / PI);
	while (count > 0)
	{
		// printf("Angle: %f\n", angle * 180 / PI);
		x = vars->p.x_co + (cos(angle) * 0.1);
		y = vars->p.y_co + (sin(angle) * 0.1);
		row = (int)x;
		col = (int)y;
		if (!vars->map[col][row] || (vars->map[col][row] && vars->map[col][row] == '1'))
			vars->p.wall_right = 1;
		// glBegin(GL_POINTS);
		// glVertex2f(x, y);
		// glEnd();
		draw_point(vars, x * 87.5, y * 87.5);
		angle += (10 * PI / 180);
		if (angle > 2 * PI)
			angle -= 2 * PI;
		count--;
	}
}

int	key_block(int c)
{
	if (c == 0)
		return (1);
	else if (c == '1')
		return (1);
	else if (c == ' ')
		return(1);
	return(0);
}

void raycasting_up(t_vars *vars)
{
	int	row;
	int	col;

	vars->p.dx = cos(vars->p.ang) * (0.0556);
	vars->p.dy = sin(vars->p.ang) * (0.0556);
	if ((((vars->p.x_co + vars->p.dx) * 87.5) > 0)
		&& (((vars->p.x_co + vars->p.dx) * 87.5) < vars->width)
		&& (((vars->p.y_co + vars->p.dy) * 87.5) > 0)
		&& (((vars->p.y_co + vars->p.dy) * 87.5) < vars->height))
	{
		printf("Change in x: %f, Change in y: %f\n", vars->p.dx, vars->p.dy);
		row = (int)(vars->p.x_co + vars->p.dx);
		col = (int)(vars->p.y_co + vars->p.dy);
		// if ((vars->map[col][row] && vars->map[col][row] == '1') || vars->p.wall_frwrd)
		if ((vars->map[col][row] && key_block(vars->map[col][row])) || vars->p.wall_frwrd)
			return ;
		row = (int)(vars->p.x_co);
		col = (int)(vars->p.y_co);
		vars->map[col][row] = '0';
		vars->p.x_co += vars->p.dx;
		vars->p.y_co += vars->p.dy;
		row = (int)(vars->p.x_co + vars->p.dx);
		col = (int)(vars->p.y_co + vars->p.dy);
		vars->map[col][row] = 'P';
		vars->p.x_map_co = vars->p.x_co * 87.5;
		vars->p.y_map_co = vars->p.y_co * 87.5;
	}
}

void raycasting_down(t_vars *vars)
{
	int	row;
	int	col;

	vars->p.dx = cos(vars->p.ang) * (0.0556);
	vars->p.dy = sin(vars->p.ang) * (0.0556);
	if ((((vars->p.x_co - vars->p.dx) * 87.5) > 0)
		&& (((vars->p.x_co - vars->p.dx) * 87.5) < vars->width)
		&& (((vars->p.y_co - vars->p.dy) * 87.5) > 0)
		&& (((vars->p.y_co - vars->p.dy) * 87.5) < vars->height))
	{
		printf("Change in x: %f, Change in y: %f\n", vars->p.dx, vars->p.dy);
		row = (int)(vars->p.x_co - vars->p.dx);
		col = (int)(vars->p.y_co - vars->p.dy);
		if (vars->map[col][row] && key_block(vars->map[col][row]) || vars->p.wall_bckwrd)
			return ;
		printf("Char: %c\n", vars->map[col][row]);
		// if ((vars->map[col][row] && vars->map[col][row] == '1') || vars->p.wall_bckwrd)
		row = (int)(vars->p.x_co);
		col = (int)(vars->p.y_co);
		vars->map[col][row] = '0';
		vars->p.x_co -= vars->p.dx;
		vars->p.y_co -= vars->p.dy;
		row = (int)(vars->p.x_co - vars->p.dx);
		col = (int)(vars->p.y_co - vars->p.dy);
		vars->map[col][row] = 'P';
		vars->p.x_map_co = vars->p.x_co * 87.5;
		vars->p.y_map_co = vars->p.y_co * 87.5;
	}
}

void raycasting_right(t_vars *vars)
{
	int	row;
	int	col;

	vars->p.dx = cos((PI / 2) + (vars->p.ang)) * 0.0556;
	vars->p.dy = sin((PI / 2) + (vars->p.ang)) * 0.0556;
	if ((((vars->p.x_co - vars->p.dx) * 87.5) > 0)
		&& (((vars->p.x_co - vars->p.dx) * 87.5) < vars->width)
		&& (((vars->p.y_co - vars->p.dy) * 87.5) > 0)
		&& (((vars->p.y_co - vars->p.dy) * 87.5) < vars->height))
	{
		row = (int)(vars->p.x_co + vars->p.dx);
		col = (int)(vars->p.y_co + vars->p.dy);
		if (vars->map[col][row] && key_block(vars->map[col][row]) || vars->p.wall_right)
			return ;
		row = (int)(vars->p.x_co);
		col = (int)(vars->p.y_co);
		vars->map[col][row] = '0';
		vars->p.x_co += vars->p.dx;
		vars->p.y_co += vars->p.dy;
		row = (int)(vars->p.x_co + vars->p.dx);
		col = (int)(vars->p.y_co + vars->p.dy);
		vars->map[col][row] = 'P';
		vars->p.x_map_co = vars->p.x_co * 87.5;
		vars->p.y_map_co = vars->p.y_co * 87.5;
	}
}

void raycasting_left(t_vars *vars)
{
	int	row;
	int	col;

	vars->p.dx = cos((PI / 2) + (vars->p.ang)) * 0.0556;
	vars->p.dy = sin((PI / 2) + (vars->p.ang)) * 0.0556;
	if ((((vars->p.x_co - vars->p.dx) * 87.5) > 0)
		&& (((vars->p.x_co - vars->p.dx) * 87.5) < vars->width)
		&& (((vars->p.y_co - vars->p.dy) * 87.5) > 0)
		&& (((vars->p.y_co - vars->p.dy) * 87.5) < vars->height))
	{
		row = (int)(vars->p.x_co - vars->p.dx);
		col = (int)(vars->p.y_co - vars->p.dy);
		if (vars->map[col][row] && key_block(vars->map[col][row]) || vars->p.wall_left)
			return ;
		row = (int)(vars->p.x_co);
		col = (int)(vars->p.y_co);
		vars->map[col][row] = '0';
		vars->p.x_co -= vars->p.dx;
		vars->p.y_co -= vars->p.dy;
		row = (int)(vars->p.x_co - vars->p.dx);
		col = (int)(vars->p.y_co - vars->p.dy);
		vars->map[col][row] = 'P';
		vars->p.x_map_co = vars->p.x_co * 87.5;
		vars->p.y_map_co = vars->p.y_co * 87.5;
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

void	exit_game(t_vars *vars)
{
	free_all(vars->map);
	// mlx_destroy_image(vars.img);
	mlx_destroy_window(vars->mlx, vars->win);
	exit(0);
}

int handle_no_event(t_vars *vars)
{
	draw_rays(vars);
	Wall_C_forwrd(vars);
	Wall_C_bckwrd(vars);
	Wall_C_left(vars);
	Wall_C_right(vars);
}

int key_hook(int keycode, t_vars *vars)
{
	// printf("Keycode: %d\n", keycode);
	void *img;

	img = mlx_new_image(vars->mlx, vars->width, vars->height);
	vars->addr = mlx_get_data_addr(img, &vars->bpp, &vars->len, &vars->end);
	printf("%s\n", (unsigned char *)vars->addr);
	mlx_put_image_to_window(vars->mlx, vars->win, img, 0, 0);
	mlx_destroy_image(vars->mlx, img);
	if (keycode == ESC)
		exit_game(vars);
	else if (keycode == E)
		printf("E: Successful\n");
	move_player(keycode, vars);
	rotation(keycode, vars);
	draw_point(vars, vars->p.x_map_co, vars->p.y_map_co);
	draw_rays(vars);
	draw_point(vars, vars->p.r.x_ray_l, vars->p.r.y_ray_l);
	draw_point(vars, vars->p.r.x_ray_r, vars->p.r.y_ray_r);
	init_window(vars);
	handle_no_event(vars);
	ft_putstr_map(vars->map);
	return (0);
}

void init(t_vars *vars)
{
	vars->p.x_co = 320;
	vars->p.y_co = 320;
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
	vars->p.wall_left = 0;
	vars->p.wall_right = 0;
	// vars->bpp = 0;
	// vars->len = 0;
	// vars->end = 0;
	// draw_rays(vars);
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
	map[3] = strdup("11101001");
	map[4] = strdup("10010001");
	map[5] = strdup("100P0001");
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
	vars->p.x_co = 0;
	vars->p.y_co = 0;
	while (vars->map[i] != NULL)
	{
		j = 0;
		while (vars->map[i][j])
		{
			if (vars->map[i][j] == 'P')
				break;
			j++;
		}
		if (vars->map[i][j] == 'P')
			break;
		i++;
	}
	vars->p.x_co = j + 0.5;
	vars->p.y_co = i + 0.5;
	vars->p.x_map_co = vars->p.x_co * 87.5;
	vars->p.y_map_co = vars->p.y_co * 87.5;
}

int	mouse_hook(int x, int y, t_vars *vars)
{
	mlx_mouse_get_pos(vars->mlx, vars->win, &x, &y);
	if (x < 350)
		key_hook(A_L, vars);
	else if (x > 350)
		key_hook(A_R, vars);
	// printf("Mouse Coordinates: %d, %d\n", x, y);
	mlx_mouse_move(vars->mlx, vars->win, 350, 350);
	return (0);
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
	printf("Row: %f, Col: %f\n", vars.p.x_co, vars.p.y_co);
	printf("Row: %f, Col: %f\n", vars.p.x_map_co, vars.p.y_map_co);
	vars.win = mlx_new_window(vars.mlx, vars.width, vars.height, "Cub3D");
	draw_rays(&vars);
	draw_point(&vars, vars.p.x_map_co, vars.p.y_map_co);
	init_window(&vars);
	mlx_hook(vars.win, 2, (1L << 0), key_hook, &vars);
	mlx_hook(vars.win, 6, (1L<<6), mouse_hook, &vars);
	mlx_loop_hook(vars.mlx, handle_no_event, &vars);
	mlx_loop(vars.mlx);
	return (0);
}