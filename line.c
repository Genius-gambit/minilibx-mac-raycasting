#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "minilibx/mlx.h"
#include <math.h>
#define ESC 65307

typedef struct s_vars
{
	void		*mlx;
	void		*win;
	void		*img;
	int			width;
	int			height;
}				t_vars;

void	init(t_vars *vars)
{
	vars->height = 0;
	vars->img = NULL;
	vars->mlx = NULL;
	vars->width = 0;
	vars->win = NULL;
}

void	draw_point(t_vars *vars, float x, float y)
{
	int	i;
	int	j;

	i = 5;
	j = 5;
	while (j > 0)
	{
		i = 20;
		while (i > 0)
		{
			mlx_pixel_put(vars->mlx, vars->win, x + i, y + j, 0x00FF00);
			i--;
		}
		j--;
	}
}

void	make_line(t_vars *vars, float *x, float *y)
{
	*x = 350;
	*y = 20;
	draw_point(vars, *x, *y);
	while (*y < 701)
	{
		draw_point(vars, *x, *y);
		(*y) += 5;
	}
}

int	key_hook(int keycode, t_vars *vars)
{
	if (keycode == ESC)
	{
		mlx_destroy_window(vars->mlx, vars->win);
		exit (0);
	}
}

int main(void)
{
	t_vars	vars;
	float	x;
	float	y;

	init(&vars);
	x = 0;
	y = 0;
	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, 700, 700, "Line");
	make_line(&vars, &x, &y);
	mlx_key_hook(vars.win, key_hook, &vars);
	mlx_loop(vars.mlx);
	return (0);
}