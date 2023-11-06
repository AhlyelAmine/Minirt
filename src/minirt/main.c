/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbrahm <aelbrahm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 04:41:56 by aahlyel           #+#    #+#             */
/*   Updated: 2023/11/06 11:28:01 by aelbrahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "libft.h"
#include "library.h"
#include "minirt.h"
#include "vector.h"
#include <mlx.h>
#define WIDTH 1280
#define HEIGHT 820
#define FRAME 3
#define M_D 1.79769e+308
#define eps 1e-3
typedef struct s_hit_record
{
	double	t;
	t_vec	pHit;
	t_vec	nHit;	
}	t_hit_record;


void	clearobjs(t_objects **lst)
{
	t_objects	*tmp;

	if (!lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		free ((*lst)->object);
		free(*lst);
		*lst = tmp;
	}
}

void	my_mlx_put(t_mrt *rt, int x, int y, int color)
{
	int	iter;

	if (x < (WIDTH - FRAME) && x >= FRAME && y >= FRAME && y < (HEIGHT - FRAME))
	{
		iter = (x * rt->bit_per_px / 8) + (y * rt->line_len);
		// printf("rt->bit_per_px: %d rt->line_len: %d\n", rt->bit_per_px, rt->line_len);
		rt->mlx_add[iter] = color;
		rt->mlx_add[++iter] = (color >> 8) & 0xFF;
		rt->mlx_add[++iter] = (color >> 16) & 0xFF;
	}
	return ;
}

int	get_pixel(t_mrt *rt, int x, int y)
{
	int	iter;
	int	color;

	if (x < WIDTH && x >= 0 && y >= 0 && y < HEIGHT)
	{
		iter = (x * rt->bit_per_px / 8) + (y * rt->line_len);
		color = rt->mlx_add[iter];
		color |= rt->mlx_add[++iter] << 8;
		color |= rt->mlx_add[++iter] << 16;
		return (color);
	}
	return (0);
}

int	rgb_to_int(t_coord color)
{
	int	r;
	int	g;
	int	b;
	
	r = (int)(255.9 * (color.v_x));
	g = (int)(255.9 * (color.v_y));
	b = (int)(255.9 * color.v_z);
	return (r << 16 | g << 8 | b);
}

void	lookat(double matrix[4][4], t_mrt *rt, t_camera *cam)
{
	t_vec	tmp = {0, 1, 0};
	t_vec	forword = normalize(&cam->normalized);
	cam->right = cross_product(tmp, forword);
	cam->up = cross_product(forword, cam->right);
	rt->cam_matrix[0][0] = cam->right.v_x;
	rt->cam_matrix[0][1] = cam->right.v_y;
	rt->cam_matrix[0][2] = cam->right.v_z;
	rt->cam_matrix[1][0] = cam->up.v_x;
	rt->cam_matrix[1][1] = cam->up.v_y;
	rt->cam_matrix[1][2] = cam->up.v_z;
	rt->cam_matrix[2][0] = forword.v_x;
	rt->cam_matrix[2][1] = forword.v_y;
	rt->cam_matrix[2][2] = forword.v_z;
}

t_vec	cam_to_world(double matrix[4][4], t_vec *dir)
{
	t_vec	v;	
	v.v_x = dir->v_x * matrix[0][0] + dir->v_y * matrix[1][0] + dir->v_z * matrix[2][0];
	v.v_y = dir->v_x * matrix[0][1] + dir->v_y * matrix[1][1] + dir->v_z * matrix[2][1];
	v.v_z = dir->v_x * matrix[0][2] + dir->v_y * matrix[1][2] + dir->v_z * matrix[2][2];
	return v;
}
void	Prime_ray(t_mrt *rt ,int x, int y, t_ray *ray,t_camera *cam)
{

	
	// ndcX = ((double)x + 0.5) / WIDTH;
	// ndcY = ((double)y + 0.5) / HEIGHT;
	// t_vec	lower_left_corner = { -2.0, -1.0, -1.0 };
	// t_vec	horizontal = { 4.0, 0.0, 0.0 };
	// t_vec	vertical = { 0.0, 2.0, 0.0 };
	// ray->origin = cam->cord;
	// ray->direction = vec_addition(lower_left_corner, vec_addition(scalar_mult(horizontal, ndcX), scalar_mult(vertical, ndcY)));
	t_vec	direction;
	double	ndcX;
	double	ndcY;
	double	aspect_ratio;

	aspect_ratio = WIDTH / (double)HEIGHT;
	ray->origin = cam->cord;
	ndcX = ((double)x + 0.5) / WIDTH;
	ndcY = ((double)y + 0.5) / HEIGHT;
	direction.v_x = (2 * ndcX - 1) * tan(((double)(cam->v_field) / 2) * M_PI / 180) * aspect_ratio;
	direction.v_y = (2 * ndcY - 1) * tan(((double)(cam->v_field) / 2) * M_PI / 180);
	direction.v_z = -1.0;
	// ray->direction = normalize(&direction);
	ray->direction.v_x = cam->normalized.v_x + direction.v_x * cam->right.v_x + direction.v_y * cam->up.v_x;
	ray->direction.v_y = cam->normalized.v_y + direction.v_x * cam->right.v_y + direction.v_y * cam->up.v_y;
	ray->direction.v_z = cam->normalized.v_z + direction.v_x * cam->right.v_z + direction.v_y * cam->up.v_z;

}

int sphere_intersect(t_sphere sphere, t_ray ray, t_vec *pHit, t_vec *nHit) {
    t_vec	oc;
	t_cord	p;
    double	discriminant;

	oc = vec_sub(ray.origin, sphere.cord);
    p.a = dot_product(ray.direction, ray.direction);
   	p.b = 2.0 * dot_product(oc, ray.direction);
    p.c = dot_product(oc, oc) - ((sphere.diameter / 2) * (sphere.diameter / 2));
	discriminant = p.b * p.b - 4 * p.a * p.c;
    if (discriminant > 0.0) {
        double t1 = (-p.b - sqrt(discriminant)) / (2.0f * p.a);
        double t2 = (-p.b + sqrt(discriminant)) / (2.0f * p.a);
        double t = (t1 < t2) ? t1 : t2;
        pHit->v_x = ray.origin.v_x + t * ray.direction.v_x;
        pHit->v_y = ray.origin.v_y + t * ray.direction.v_y;
        pHit->v_z = ray.origin.v_z + t * ray.direction.v_z;
        *nHit = normalize(pHit);
        return 1;
    }
    return 0;
}

bool	plan_hit(t_ray *ray, t_plane *plan, t_hit_record *rec)
{
	double	denom;
	double	t;
	t_vec	p;

	denom = dot_product(plan->normalized, ray->direction);
	if (fabs(denom) > eps)
	{
		p = vec_sub(plan->cord, ray->origin);
		t = dot_product(p, plan->normalized) / denom;
		if (t > eps)
		{
			rec->t = t;
			rec->pHit = at(rec->t, *ray);
			rec->nHit = plan->normalized;
			return (true);
		}
	}
	return (false);
}



bool	sphere_hit(t_ray *ray, t_sphere *sphere, t_hit_record *rec)
{
	t_vec	oc;
	t_cord	p;
	double	discriminant;
	double	tmp;

	oc = vec_sub(ray->origin, sphere->cord);
	p.a = dot_product(ray->direction, ray->direction);
	p.b = dot_product(oc, ray->direction);

	p.c = dot_product(oc, oc) - (sphere->diameter/2) * (sphere->diameter/2);
	discriminant = p.b * p.b - (p.a * p.c);
	if (discriminant < 0)
		return (false);
	if (discriminant > 0)
	{
		tmp = (-p.b - sqrt(discriminant)) / (p.a);
		if (tmp <= 0.0 || tmp >= M_D)
		{
			tmp = (-p.b + sqrt(discriminant)) / (p.a);
			if (tmp <= 0.0 || tmp >= M_D)
				return (false);		
		}
	}else
	{
		tmp = -p.b / p.a;
		if (tmp <= 0.0 || tmp >= M_D)
			return (false);
	}
	rec->t = tmp;
	rec->pHit = at(rec->t, *ray);
	rec->nHit = scalar_div(vec_sub(rec->pHit, sphere->cord), (sphere->diameter / 2));
	return (true);
}
t_coord	ray_color(t_ray *ray, t_sphere *sphere, t_hit_record *rec)
{
		
	if (sphere_hit(ray, sphere, rec))
	{
		t_vec	n = vec_sub(rec->nHit, sphere->cord);
		return (scalar_mult((t_coord){n.v_x + 1, n.v_y + 1, n.v_z + 1}, 0.5));
	}
		
	t_coord	unit_direction = normalized(ray->direction);
	double t = 0.5 * (unit_direction.v_y + 1.0);
	t_coord	c_start = scalar_mult((t_coord){1.0,1.0,1.0}, (1.0 - t));
	t_coord	c_end = scalar_mult((t_coord){0.5, 0.7, 1.0}, t);
	return (scalar_mult((t_coord){0.3,0.3,0.3}, (0.1)));
}

bool	cylinder_hit(t_ray *ray, t_cylender *cylinder, t_hit_record *rec)
{
	t_vec	oc;
	t_cord	p;
	double	discriminant;
	double	tmp;

	oc = vec_sub(ray->origin, cylinder->cord);
	p.a = dot_product(ray->direction, ray->direction) - dot_product(ray->direction, cylinder->normalized) * dot_product(ray->direction, cylinder->normalized);
	p.b = 2 * (dot_product(ray->direction, oc) - dot_product(ray->direction, cylinder->normalized) * dot_product(oc, cylinder->normalized));
	p.c = dot_product(oc, oc) - dot_product(oc, cylinder->normalized) * dot_product(oc, cylinder->normalized) - (cylinder->diameter / 2) * (cylinder->diameter / 2);
	discriminant = p.b * p.b - (p.a * p.c);
	if (discriminant < 0)
		return (false);
	if (discriminant > 0)
	{
		tmp = (-p.b - sqrt(discriminant)) / (p.a);
		if (tmp <= 0.0 || tmp >= M_D)
		{
			tmp = (-p.b + sqrt(discriminant)) / (p.a);
			if (tmp <= 0.0 || tmp >= M_D)
				return (false);		
		}
	}
	else
	{
		tmp = -p.b / p.a;
		if (tmp <= 0.0 || tmp >= M_D)
			return (false);
	}
	rec->t = tmp;
	rec->pHit = at(rec->t, *ray);
	rec->nHit = scalar_div(vec_sub(rec->pHit, cylinder->cord), (cylinder->diameter / 2));
	return (true);
}
void	draw(t_mrt *m_rt, t_ray *ray, t_camera *cam, t_data data)
{
	int	x;
	int	y;
	int	nx = WIDTH;
	int ny = HEIGHT;
	t_hit_record	rec;
	cam->right = (t_vec){1, 0, 0};
	cam->up.v_x = cam->normalized.v_y * cam->right.v_z - cam->normalized.v_z * cam->right.v_y;
    cam->up.v_y = cam->normalized.v_z * cam->right.v_x - cam->normalized.v_x * cam->right.v_z;
    cam->up.v_z = cam->normalized.v_x * cam->right.v_y - cam->normalized.v_y * cam->right.v_x;
	while (data.objects->type != SPHERE)
		data.objects = data.objects->next;
	t_sphere *sphere = (t_sphere *)data.objects->object;
	printf("%.2f\n", sphere->diameter);
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			Prime_ray(m_rt, i, j, ray, cam);
			// printf("%d\n",rgb_to_int(ray_color(ray)));
			my_mlx_put(m_rt, i, j, rgb_to_int(ray_color(ray, sphere, &rec)));
		}
	}
	// // t_camera *cam = (t_camera *)data.objects->object;
	// // lookat(m_rt->cam_matrix, m_rt, cam);	
	// y = HEIGHT;
	// while (y--)
	// {
	// 	x = 0;
	// 		// double pxY = (HEIGHT - y - 0.5) / HEIGHT;
	// 	while (x < WIDTH)
	// 	{
	// 		// puts("alo");
	// 		Prime_ray(m_rt, x, y, ray, cam);
	// 		t_vec pHit, nHit;
	// 		double minDistance = INFINITY;
	// 		if (sphere_intersect(*sphere, *ray, &pHit, &nHit))
	// 			my_mlx_put(m_rt, x, y, 0xffe);
	// 		else
	// 			my_mlx_put(m_rt, x, y, 0xffff);
			
	// 		// printf("%.8f  %.8f %.8f\n", ray->direction.v_x, ray->direction.v_x, ray->direction.v_x);
	// 		// usleep(10000);
	// 		x++;
	// 	}
	// }
	mlx_put_image_to_window(m_rt->mlx, m_rt->mlx_win, m_rt->mlx_img, 0, 0);
}
int main(int ac, char **av)
{
	t_data	data;
	t_data	data_d;
	t_camera	*cam;
	ft_memset(&data, 0, sizeof(t_data));
	if (ac == 2)
	{
		if (!parcer(av[1], &data))
			return (clearobjs(&data.objects),  1);
		printf("cy's : %d, pl's : %d, sp's : %d\n", data.counter.cylender, data.counter.plane, data.counter.sphere);
		data_d = data;
		while (data.objects)
		{
			if (data.objects->type == SPHERE)
			{
				t_sphere	*object = (t_sphere *)data.objects->object;
				printf("sphere\t:\t%.1f,%.1f,%.1f\t\t\t\t%.1f\t%d,%d,%d\n", object->cord.v_x, object->cord.v_y, object->cord.v_z, object->diameter, object->clr.r, object->clr.g, object->clr.b);
			}
			else if (data.objects->type == PLANE)
			{
				t_plane	*object = (t_plane *)data.objects->object;
				printf("plane\t:\t%.1f,%.1f,%.1f\t%.1f,%.1f,%.1f\t\t\t%d,%d,%d\n", object->cord.v_x, object->cord.v_y, object->cord.v_z, object->normalized.v_x, object->normalized.v_y, object->normalized.v_z, object->clr.r, object->clr.g, object->clr.b);
			}
			else if (data.objects->type == CYLENDER)
			{
				t_cylender	*object = (t_cylender *)data.objects->object;
				printf("cylender:\t%.1f,%.1f,%.1f\t%.1f,%.1f,%.1f\t%.1f\t%.1f\t%d,%d,%d\n", object->cord.v_x, object->cord.v_y, object->cord.v_z, object->normalized.v_x, object->normalized.v_y, object->normalized.v_z, \
				object->diameter, object->height,  object->clr.r, object->clr.g, object->clr.b);
			}
			else if (data.objects->type == CAMERA)
			{
				t_camera	*object = (t_camera *)data.objects->object;
				cam = object;
				printf("camera\t:\t%.1f,%.1f,%.1f\t%.1f,%.1f,%.1f\t\t%d\n", object->cord.v_x, object->cord.v_y, object->cord.v_z, object->normalized.v_x, object->normalized.v_y, object->normalized.v_z, \
				object->v_field);
			}
			else if (data.objects->type == LIGHT)
			{
				t_light	*object = (t_light *)data.objects->object;
				printf("light\t:\t%.1f,%.1f,%.1f\t\t\t\t%.1f\t%d,%d,%d\n", object->cord.v_x, object->cord.v_y, object->cord.v_z, object->brightness, \
				object->clr.r, object->clr.g, object->clr.b);
			}
			else if (data.objects->type == LIGHTING)
			{
				t_lighting	*object = (t_lighting *)data.objects->object;
				printf("lighting:\t\t\t\t\t\t%.1f\t%d,%d,%d\n", object->ratio, object->clr.r, object->clr.g, object->clr.b);
			}
			data.objects = data.objects->next;
		}
	}

	t_mrt scean;
	t_ray ray;
	
	scean.mlx = mlx_init();
	scean.mlx_win = mlx_new_window(scean.mlx, WIDTH, HEIGHT, "MINI_RT");
	scean.mlx_img = mlx_new_image(scean.mlx, WIDTH, HEIGHT);
	scean.mlx_add = mlx_get_data_addr(scean.mlx_img, &(scean.bit_per_px), &(scean.line_len), &(scean.endian));
	draw(&scean, &ray, cam, data_d);
	mlx_loop(scean.mlx);
	return 0;
}
