/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _check_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lignigno <lignign@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 00:42:33 by lignigno          #+#    #+#             */
/*   Updated: 2022/08/27 23:22:54 by lignigno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "_check_regexp_encaps.h"

// _____________________________________________________________________________

static void	set_repeater(const regexp_rules_t * rule, repeat_t * repeater)
{
	if (check_flag(rule->hflags, FLAG_REPEATER) == true)
	{
		/* {from,to} */
		/* {,to} */
		if (check_flag(rule->hflags, FLAG_FROM | FLAG_TO) == true ||
			check_flag(rule->hflags, FLAG_TO) == true)
		{
			*repeater = rule->repeat;
		}
		/* {from,} */
		else if (check_flag(rule->hflags, FLAG_FROM) == true)
		{
			repeater->from = rule->repeat.from;
			repeater->to = 1;
		}
		/* {from} */
		else
		{
			repeater->from = rule->repeat.from;
			repeater->to = rule->repeat.from;
		}
	}
	else
	{
		repeater->from = 1;
		repeater->to = 1;
	}
}

// _____________________________________________________________________________

static bool	check_single_symbol(const char * str, const regexp_rules_t * rule, size_t * i)
{
	size_t	num_ranges;
	bool	good;

	good = !check_flag(rule->hflags, FLAG_REVERSE);
	if (rule->range != NULL)
	{
		num_ranges = rule->num_ranges * 2;
		for (size_t j = 0; j < num_ranges; j += 2)
		{
			if (rule->range[j] <= str[*i] && str[*i] <= rule->range[j + 1])
			{
				++(*i);
				return (good);
			}
		}
	}
	if (ft_strchar(rule->str, str[*i]) != NULL)
	{
		++(*i);
		return (good);
	}

	++(*i);
	return (!good);
}

// _____________________________________________________________________________

static bool	check_simple(const char * str, const regexp_rules_t * rule, size_t * i)
{
	size_t	j;

	j = 0;
	while (str[*i] != '\0' && rule->str[j] != '\0')
	{
		if (str[*i] != rule->str[j])
			break ;
		++(*i);
		++j;
	}

	return (rule->str[j] == '\0');
}

// _____________________________________________________________________________

regexp_ret_code_t	check_str(const char * str, const regexp_rules_t * pars_rules)
{
	size_t		i;
	repeat_t	repeater;

	i = 0;
	do
	{
		printf("befor {%c}\n", str[i]);
		if (pars_rules == NULL)
			break ;

		if (check_flag(pars_rules->hflags, FLAG_SINGLE_SYMBOL) == true)
		{
			set_repeater(pars_rules, &repeater);
			while (repeater.to > 0 && str[i] != '\0')
			{
				if (check_single_symbol(str, pars_rules, &i) == false)
				{
					break ;
				}
				repeater.from -= (repeater.from > 0);
				if (check_flag(pars_rules->hflags, FLAG_TO) == true ||
					check_flag(pars_rules->hflags, FLAG_REPEATER) == false)
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
				if (check_simple(str, pars_rules, &i) == false)
				{
					break ;
				}
				repeater.from -= (repeater.from > 0);
				if (check_flag(pars_rules->hflags, FLAG_TO) == true ||
					check_flag(pars_rules->hflags, FLAG_REPEATER) == false)
					--repeater.to;
			}
			if (repeater.from > 0)
				return (REGEXP_FAIL);
		}

		printf("after {%c}\n", str[i]);

		pars_rules = pars_rules->next;
	}
	while (str[i] != '\0' && pars_rules != NULL);

	if (str[i] != '\0' || pars_rules != NULL)
		return (REGEXP_FAIL);
	return (REGEXP_OK);
}
