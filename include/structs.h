#ifndef STRUCTS_H
#define STRUCTS_H
#include <stdio.h>

#define t_cord t_vector
typedef struct s_cord
{
	double	x;
	double	y;
	double	z;
}	t_cord;

typedef struct s_vec
{
	double	v_x;
	double	v_y;
	double	v_z;
}	t_vec;
typedef struct s_color
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
}	t_color;

typedef struct s_lighting
{
	t_color	clr;
	double	ratio;
}	t_lighting;

typedef struct s_camera
{
	unsigned char	v_field;
	t_cord			cord;
	t_cord			normalized;
}	t_camera;

typedef struct s_light
{
	t_color	clr;
	double	brightness;
	t_cord	cord;
}	t_light;

typedef struct s_plane
{
	t_color		clr;
	t_cord		cord;
	t_vector	normalized;
}	t_plane;

typedef struct s_cylender
{
	t_color		clr;
	double		diameter;
	double		height;
	t_cord		cord;
	t_vector	normalized;
}	t_cylender;

typedef struct s_sphere
{
	t_color	clr;
	double	diameter;
	t_cord	cord;
}	t_sphere;

typedef	struct s_object
{
	unsigned char	type;
	void			*object;
	struct s_object	*next;
}	t_objects;

typedef struct s_counter
{
	int sphere;
	int cylender;
	int plane;
}	t_counter;

typedef	struct s_data
{
	t_objects	*objects;
	t_lighting	lighting;
	t_light		light;
	t_counter	counter;
	t_camera	camera;
}	t_data;


#endif