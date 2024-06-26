/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_tools_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aahlyel <aahlyel@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 18:55:06 by aahlyel           #+#    #+#             */
/*   Updated: 2023/12/11 14:49:56 by aahlyel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs_bonus.h"
#include "library_bonus.h"

void print_vec(char *str, t_vec vec)
{
    printf("%s x: %f, y: %f, z: %f\n", str, vec.v_x, vec.v_y, vec.v_z);
}
void	print_scean(t_data data)
{
    printf("cy's : %d, pl's : %d, sp's : %d\n", data.counter.cylender, data.counter.plane, data.counter.sphere);
    printf("camera\t:\t%.2f,%.2f,%.2f\t%.2f,%.2f,%.2f\t\t%d\n", data.camera.cord.v_x, data.camera.cord.v_y, data.camera.cord.v_z, data.camera.normalized.v_x, data.camera.normalized.v_y, data.camera.normalized.v_z, data.camera.v_field);
    printf("lighting:\t\t\t\t\t\t%.2f\t%d,%d,%d\n", data.lighting.ratio, data.lighting.clr.r, data.lighting.clr.g, data.lighting.clr.b);
    t_light *lights = data.light;
    while (lights)
    {
        printf("light\t:\t%.2f,%.2f,%.2f\t\t\t\t%.2f\t%d,%d,%d\n", lights->cord.v_x, lights->cord.v_y, lights->cord.v_z, lights->brightness, lights->clr.r, lights->clr.g, lights->clr.b);
        lights = lights->next;
    }    while (data.objects)
    {
        if (data.objects->type == SPHERE)
        {
            t_sphere	*object = (t_sphere *)data.objects->object;
            printf("sphere\t:\t%.2f,%.2f,%.2f\t\t\t\t%.2f\t%d,%d,%d\n", object->cord.v_x, object->cord.v_y, object->cord.v_z, object->diameter, object->clr.r, object->clr.g, object->clr.b);
			if (object->spec.texture)
				printf("sphere texture %s \n", object->spec.texture);
            if (object->spec.bump)
				printf("sphere bump %s \n", object->spec.bump);
		}
        else if (data.objects->type == PLANE)
        {
            t_plane	*object = (t_plane *)data.objects->object;
            printf("plane\t:\t%.2f,%.2f,%.2f\t%.2f,%.2f,%.2f\t\t\t%d,%d,%d\n", object->cord.v_x, object->cord.v_y, object->cord.v_z, object->normalized.v_x, object->normalized.v_y, object->normalized.v_z, object->clr.r, object->clr.g, object->clr.b);
			if (object->spec.texture)
				printf("sphere texture %s \n", object->spec.texture);
            if (object->spec.bump)
				printf("sphere bump %s \n", object->spec.bump);
        }
        else if (data.objects->type == CYLENDER)
        {
            t_cylender	*object = (t_cylender *)data.objects->object;
            printf("cylender:\t%.1f,%.1f,%.1f\t%.1f,%.1f,%.1f\t%.1f\t%.1f\t%d,%d,%d\n", object->cord.v_x, object->cord.v_y, object->cord.v_z, object->normalized.v_x, object->normalized.v_y, object->normalized.v_z, \
            object->diameter, object->height,  object->clr.r, object->clr.g, object->clr.b);
			if (object->spec.texture)
				printf("sphere texture %s \n", object->spec.texture);
            if (object->spec.bump)
				printf("sphere bump %s \n", object->spec.bump);
        }
        else if (data.objects->type == CONE)
        {
            t_cone	*object = (t_cone *)data.objects->object;
            printf("cone:\t%.1f,%.1f,%.1f\t%.1f,%.1f,%.1f\t%.1f\t%.1f\t%d,%d,%d\n", object->cord.v_x, object->cord.v_y, object->cord.v_z, object->normalized.v_x, object->normalized.v_y, object->normalized.v_z, \
            object->half_angle, object->height,  object->clr.r, object->clr.g, object->clr.b);
			if (object->spec.texture)
				printf("sphere texture %s \n", object->spec.texture);
            if (object->spec.bump)
				printf("sphere bump %s \n", object->spec.bump);
        }

        data.objects = data.objects->next;
    }
}
