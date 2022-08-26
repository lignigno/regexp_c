/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lignigno <lignign@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 21:44:57 by lignigno          #+#    #+#             */
/*   Updated: 2022/08/25 15:35:06 by lignigno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

// __________________________________________________________________ROUNDING UP

inline static size_t	rounding_up(size_t num, size_t base)
{
	size_t	result;

	result = num / base;
	result += num % base != 0;
	result *= base;

	return (result);
}

// ______________________________________________________________________FT JOIN

bool	ft_join(char ** dst, const char * str, size_t str_size)
{
	char *	result;
	size_t	i;
	size_t	mem_size;
	size_t	dst_size;

	if (dst == NULL || str == NULL)
		return (false);

	if (*dst != NULL)
	{
		dst_size = ft_strlen(*dst) + 1;
		mem_size = rounding_up(dst_size, MULTIPLICITY);
	}
	else
	{
		dst_size = 1;
		mem_size = 0;
	}

	if (dst_size + str_size > mem_size)
	{
		mem_size = mem_size + rounding_up(str_size, MULTIPLICITY);
		result = (char *)malloc(mem_size);
		if (result == NULL)
		{
			return (false);
		}

		i = 0;
		if (*dst != NULL)
		{
			while ((*dst)[i] != '\0')
			{
				result[i] = (*dst)[i];
				++i;
			}
			free(*dst);
		}
	}
	else
	{
		result = *dst;
		i = dst_size - 1;
	}

	for (size_t j = 0; j < str_size && str[j] != '\0'; ++i, ++j)
	{
		result[i] = str[j];
	}
	result[i] = '\0';
	*dst = result;

	return (true);
}

// ________________________________________________________________SHRINK TO FIT

bool	shrink_to_fit(char ** str)
{
	size_t	i;
	char *	new_str;

	if (str == NULL)
		return (false);
	if (*str == NULL)
		return (true);

	new_str = (char *)malloc(ft_strlen(*str) + 1);
	if (new_str == NULL)
		return (false);

	i = 0;
	while ((*str)[i] != '\0')
	{
		new_str[i] = (*str)[i];
		++i;
	}
	new_str[i] = '\0';

	free(*str);
	*str = new_str;

	return (true);
}
