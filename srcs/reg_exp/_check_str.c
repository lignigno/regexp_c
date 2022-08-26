/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _check_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lignigno <lignign@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 00:42:33 by lignigno          #+#    #+#             */
/*   Updated: 2022/08/27 02:28:35 by lignigno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_check_regexp_encaps.h"

// static void	set_repeat(const regexp_rules_t * rule, repeat_t * repeat)
// {
// 	if (check_flag(rule->hflags, FLAG_REPEATER) == true)
// 	{
		
// 	}
// 	else
// 		repeat->to = 1;
// }

regexp_ret_code_t	check_str(const char * str, const regexp_rules_t * pars_rules)
{
	(void)str;
	(void)pars_rules;
	// size_t		i;
	// repeat_t	repeat;

	// i = 0;
	// while (str[i] != '\0' && pars_rules != NULL)
	// {
	// 	if (check_flag(pars_rules->hflags, FLAG_SINGLE_SYMBOL) == true)
	// 	{
	// 		i++;
	// 	}
	// 	if (check_flag(pars_rules->hflags, FLAG_SIMPLE) == true)
	// 	{
	// 		// while (repeat.to > 0)
	// 		// {
	// 		// 	/* code */
	// 		// }
			
	// 	}

	// 	pars_rules = pars_rules->next;
	// }

	// if (str[i] == '\0')
		return (REGEXP_OK);
	// return (REGEXP_FAIL);
}