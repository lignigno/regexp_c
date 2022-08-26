/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tets_polygone.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lignigno <lignign@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 18:03:28 by lignigno          #+#    #+#             */
/*   Updated: 2022/08/24 03:11:08 by lignigno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BLUE_COLOR	"\033[38;2;0;255;251m"
#define RED_COLOR	"\033[38;2;250;0;0m"
#define GREEN_COLOR	"\033[38;2;0;200;0m"
#define UNSET_COLOR	"\033[39m"

# define MULTIPLICITY	1

size_t	ft_strlen(const char * str)
{
	size_t	i;

	if (str == NULL)
		return (0);
	i = 0;
	while (str[i] != '\0')
		++i;
	return (i);
}

inline static size_t	rounding_up(size_t num, size_t base)
{
	size_t	result;

	result = num / base;
	result += num % base != 0;
	result *= base;

	return (result);
}

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


int main(int argc, char const *argv[])
{
	/* print input argv */
	printf("%s", BLUE_COLOR);
	for (size_t i = 0; argv[i] != NULL; ++i)
	{
		printf("argv[%5zu] : \"%s\"\n", i, argv[i]);
	}
	printf("%s\n", UNSET_COLOR);

	/* tests*/

	if (argc < 2)
	{
		printf("%sneed string to copy%s\n", RED_COLOR, UNSET_COLOR);
		return (0);
	}

	char *	dst = NULL;

	for (size_t i = 0; argv[1][i] != '\0'; ++i)
	{
		if (ft_join(&dst, &argv[1][i], 1) == false)
		{
			printf("%shuinya sluchaetsa %s\n", BLUE_COLOR, UNSET_COLOR);
			return (0);
		}
		printf("dst : {%s}\n", dst);
	}

	while (1);

	return (0);
}
