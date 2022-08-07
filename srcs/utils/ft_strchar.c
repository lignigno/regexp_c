/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lignigno <lignign@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/06 20:08:37 by lignigno          #+#    #+#             */
/*   Updated: 2022/08/07 03:18:14 by lignigno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

char *ft_strchar(const char * str, char c)
{
	if (str == NULL)
		return NULL;
	while (str[0] != '\0')
	{
		if (str[0] == c)
			return ((char *)str);
		++str;
	}

	return (NULL);
}
