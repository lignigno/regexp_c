/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _flag_funcs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lignigno <lignign@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 22:10:31 by lignigno          #+#    #+#             */
/*   Updated: 2022/08/25 09:21:04 by lignigno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_check_regexp_encaps.h"

// _____________________________________________________________________SET FLAG

void	set_flag(hflags_t * holder, hflags_t flag_mask)
{
	*holder |= flag_mask;
}

// ___________________________________________________________________UNSET FLAG

void	unset_flag(hflags_t * holder, hflags_t flag_mask)
{
	flag_mask = ~flag_mask;
	*holder &= flag_mask;
}

// ___________________________________________________________________CHECK FLAG

bool	check_flag(hflags_t holder, hflags_t flag_mask)
{
	return ((holder & flag_mask) == flag_mask);
}
