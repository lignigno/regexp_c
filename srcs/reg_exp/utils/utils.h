/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lignigno <lignign@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/06 20:06:28 by lignigno          #+#    #+#             */
/*   Updated: 2022/08/24 03:13:01 by lignigno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stdlib.h>
# include <stdbool.h>

/**
 * @brief	Used in ft_join to allocate memory with a margin.
 */
# define MULTIPLICITY	10

/**
 * @brief	Find length of string.
 * 
 * @param[in]	str	The string whose size you need to find out.
 *
 * @return	Size of string.
 */
size_t	ft_strlen(const char * str);

/**
 * @brief	Search for a character in a string.
 * 
 * @param[in]	str	The string in which the search will be performed.
 * @param[in]	c	The searched symbol.
 *
 * @return	The address where the symbol was found, otherwise NULL.
 */
char *	ft_strchar(const char * str, char c);

/**
 * @brief	asdf
 * 
 * @details	asdf
 *
 * @param[out]	dst			asdf
 * @param[in]	str			asdf
 * @param[in]	str_size	asdf
 *
 * @return	true	- The operation was successful.
 *			false	- Malloc failed or wrong argument.
 */
bool	ft_join(char ** dst, const char * str, size_t str_size);

/**
 * @brief	Reducing the size of memory used.
 *
 * @note	It is applied after using the ft_join function to reduce
 *			the memory size in case there is unused memory left.
 *
 * @param[out]	str	The address of the string to compress and the return value.
 *
 * @return	true	- The operation was successful.
 *			false	- Malloc failed or wrong argument.
 */
bool	shrink_to_fit(char ** str);

#endif // UTILS_H
