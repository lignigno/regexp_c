/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_regexp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lignigno <lignign@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/06 04:30:26 by lignigno          #+#    #+#             */
/*   Updated: 2022/08/07 02:28:12 by lignigno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_check_regexp_encaps.h"

regexp_ret_code_t check_regexp(const char * str, const char * regexp)
{
	regexp_ret_code_t	ret;

	if (str == NULL || regexp == NULL)
	{
		return (REGEXP_ARG_NULL);
	}

	ret = check_syntax(regexp);
	if (ret < REGEXP_OK)
	{
		return (ret);
	}

	/* main logic */

	return (REGEXP_OK);
}
