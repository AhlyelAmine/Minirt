/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylender_parcer_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aahlyel <aahlyel@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 17:21:32 by aahlyel           #+#    #+#             */
/*   Updated: 2023/11/30 10:01:54 by aahlyel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs_bonus.h"
#include "parcer_bonus.h"
#include "tools_bonus.h"
#include "libft.h"
#include "library_bonus.h"
#include <stdbool.h>
#include <limits.h>

bool cylender_parcer(char *line, t_data *data)
{
	t_objects	*object;
	t_cylender	*cylender;

	cylender = ft_calloc(sizeof(t_cylender), 1);
	if (!cylender)
		return (false);
	data->counter.cylender += 1;
	line += 3;
	if (!cordinate_parcer(&line, &cylender->cord, INT_MAX, INT_MIN))
		return (ft_putendl_fd("minirt:  cy invalid cordinate format", 2), false);
	if (!cordinate_parcer(&line, &cylender->normalized, 1, -1))
		return (ft_putendl_fd("minirt: invalid normalized format", 2), false);
	if (!ft_atod(&line, &cylender->diameter, INT_MAX, INT_MIN))
		return (ft_putendl_fd("minirt: invalid diameter format", 2), false);
	if (!ft_atod(&line, &cylender->height, INT_MAX, INT_MIN))
		return (ft_putendl_fd("minirt: invalid diameter format", 2), false);
	line += skip_spaces(line);
	int a = color_parcer(line, &cylender->clr);
	if (a == -1)
		return (ft_putendl_fd("minirt: cylender invalid color format", 2), false);
	line += skip_spaces(line + a) + a;
	if (!check_for_features(line, &cylender->spec))
		return (false);
	// ft_atod(&line, &cylender->spec.intensity, INT_MAX, INT_MIN);
	// line += skip_spaces(line);
	// ft_atod(&line, &cylender->spec.shininess_factor, 1, 0);
	// line += skip_spaces(line);
	// ft_atod(&line, &cylender->spec.reflection, 1, 0);
	object = newobject(cylender, CYLENDER);
	if (!object)
		return (false);
	addobject_back(&data->objects, object);
	return (true);
}