#ifndef TOOLS_H
#define TOOLS_H
#include "structs.h"
#include <stdbool.h>

int			skip_spaces(char *str);
void		get_integer(char **line, int *integer);
void		addobject_front(t_objects **lst, t_objects *new);
bool    	ft_atod(char **ascii, double *result, int marge_max, int marge_min);
t_objects	*newobject(void *object, unsigned char type);
void	    clearobjs(t_objects **lst);
void	hooks_settings(t_data *data);
#endif