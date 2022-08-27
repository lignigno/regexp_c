/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _check_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lignigno <lignign@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 00:42:33 by lignigno          #+#    #+#             */
/*   Updated: 2022/08/27 03:23:17 by lignigno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_check_regexp_encaps.h"

static void	set_repeater(const regexp_rules_t * rule, repeat_t * repeat)
{
	if (check_flag(rule->hflags, FLAG_REPEATER) == true)
	{
		/* {from,to} */
		if (check_flag(rule->hflags, FLAG_FROM | FLAG_TO) == true)
		{}
		/* {from,} */
		else if (check_flag(rule->hflags, FLAG_FROM) == true)
		{}
		/* {,to} */
		else if (check_flag(rule->hflags, FLAG_TO) == true)
		{}
		/* {from} */
		else
		{}
	}
	else
		repeat->to = 1;
}

regexp_ret_code_t	check_str(const char * str, const regexp_rules_t * pars_rules)
{
	size_t		i;
	repeat_t	repeater;

	i = 0;
	while (str[i] != '\0' && pars_rules != NULL)
	{
		if (check_flag(pars_rules->hflags, FLAG_SINGLE_SYMBOL) == true)
		{
			set_repeater(pars_rules, &repeater);
			while (repeater.to > 0 && str[i] != '\0')
			{
				if (check_single_symbol(str, i) == false)
				{
					break ;
				}
				repeater.from -= (repeater.from > 0);
				--repeater.to;
			}
			if (repeater.from > 0)
				return (REGEXP_FAIL);
		}
		if (check_flag(pars_rules->hflags, FLAG_SIMPLE) == true)
		{
			set_repeater(pars_rules, &repeater);
			while (repeater.to > 0 && str[i] != '\0')
			{
				if (check_simple(str, i) == false)
				{
					break ;
				}
				repeater.from -= (repeater.from > 0);
				--repeater.to;
			}
			if (repeater.from > 0)
				return (REGEXP_FAIL);
		}

		pars_rules = pars_rules->next;
	}

	if (str[i] == '\0')
		return (REGEXP_OK);
	return (REGEXP_FAIL);
}