/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _check_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lignigno <lignign@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 00:42:33 by lignigno          #+#    #+#             */
/*   Updated: 2022/08/30 01:07:22 by lignigno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "_check_regexp_encaps.h"

// _____________________________________________________________________________

static void	set_repeater(regexp_rules_t * rule)
{
	if (check_flag(rule->hflags, FLAG_REPEATER) == true)
	{
		/* {from,} */
		if (check_flag(rule->hflags, FLAG_FROM) == true &&
			check_flag(rule->hflags, FLAG_TO) == false)
		{
			rule->repeat.to = 1;
		}
		/* {from} */
		else if (check_flag(rule->hflags, FLAG_FROM | FLAG_TO) == false &&
				check_flag(rule->hflags, FLAG_TO) == false)
		{
			rule->repeat.to = rule->repeat.from;
		}
	}
	else
	{
		rule->repeat.from = 1;
		rule->repeat.to = 1;
	}
}

// _____________________________________________________________________________

static bool	check_single_symbol(const char * str, const regexp_rules_t * rule, size_t * i)
{
	size_t	num_ranges;
	bool	ret;

	ret = !check_flag(rule->hflags, FLAG_REVERSE);
	if (rule->range != NULL)
	{
		num_ranges = rule->num_ranges * 2;
		for (size_t j = 0; j < num_ranges; j += 2)
		{
			if (rule->range[j] <= str[*i] && str[*i] <= rule->range[j + 1])
			{
				if (ret == true)
				{
					++(*i);
				}
				return (ret);
			}
		}
	}
	if (ft_strchar(rule->str, str[*i]) != NULL)
	{
		if (ret == true)
		{
			++(*i);
		}
		return (ret);
	}

	if (ret == false)
	{
		++(*i);
	}
	return (!ret);
}

// _____________________________________________________________________________

static bool	check_simple(const char * str, const regexp_rules_t * rule, size_t * i)
{
	size_t	j;

	j = 0;
	while (str[*i] != '\0' && rule->str[j] != '\0')
	{
		if (str[*i] != rule->str[j])
		{
			break ;
		}
		++(*i);
		++j;
	}

	return (rule->str[j] == '\0');
}

// _____________________________________________________________________________

regexp_ret_code_t	check_str(const char * str, regexp_rules_t * parse_rules)
{
	size_t				i;
	regexp_rules_t *	tmp_rule;
	regexp_rules_t *	next_step;
	size_t				nesting_alternatives;

	i = 0;
	nesting_alternatives = 0;
	do
	{
		printf("befor {%c}\n", str[i]);
		if (parse_rules == NULL)
		{
			break ;
		}

		if (check_flag(parse_rules->hflags, FLAG_SUBPATTERN_END) == true)
		{
			printf("FLAG_SUBPATTERN_END\n");
			tmp_rule = parse_rules;
			parse_rules = parse_rules->connect;
			parse_rules->repeat.from -= (parse_rules->repeat.from > 0);
			if (check_flag(parse_rules->hflags, FLAG_REPEATER) == false ||
				check_flag(parse_rules->hflags, FLAG_FROM) == false ||
				check_flag(parse_rules->hflags, FLAG_TO) == true)
			{
				--parse_rules->repeat.to;
			}
			parse_rules = tmp_rule;
			/* rollback */
			parse_rules = parse_rules->connect;
		}
		if (check_flag(parse_rules->hflags, FLAG_SUBPATTERN_BEGIN) == true)
		{
			printf("FLAG_SUBPATTERN_BEGIN\n");
			if (check_flag(parse_rules->hflags, FLAG_ALTERNATIVE) == true)
			{
				remember_alternative();
			}
			printf("%zu %zu\n", parse_rules->repeat.from, parse_rules->repeat.to);
			if (parse_rules->repeat.from == parse_rules->mem_repeat.from &&
				parse_rules->repeat.to == parse_rules->mem_repeat.to)
			{
				set_repeater(parse_rules);
			}
			if (parse_rules->repeat.to <= 0 || str[i] == '\0')
			{
				if (parse_rules->repeat.from > 0)
				{
					if (nesting_alternatives > 0)
					{
						call_alternative();
					}
					else
						return (REGEXP_FAIL);
				}
				else
				{
					parse_rules->repeat = parse_rules->mem_repeat;
					parse_rules = parse_rules->connect;
				}
			}
		}
		else if (check_flag(parse_rules->hflags, FLAG_SINGLE_SYMBOL) == true)
		{
			printf("FLAG_SINGLE_SYMBOL\n");
			set_repeater(parse_rules);
			while (parse_rules->repeat.to > 0 && str[i] != '\0')
			{
				if (check_single_symbol(str, parse_rules, &i) == false)
				{
					break ;
				}
				parse_rules->repeat.from -= (parse_rules->repeat.from > 0);
				if (check_flag(parse_rules->hflags, FLAG_REPEATER) == false ||
					check_flag(parse_rules->hflags, FLAG_FROM) == false ||
					check_flag(parse_rules->hflags, FLAG_TO) == true)
				{
					--parse_rules->repeat.to;
				}
			}
			if (parse_rules->repeat.from > 0)
			{
				return (REGEXP_FAIL);
			}
			parse_rules->repeat = parse_rules->mem_repeat;
		}
		else if (check_flag(parse_rules->hflags, FLAG_SIMPLE) == true)
		{
			printf("FLAG_SIMPLE\n");
			set_repeater(parse_rules);
			while (parse_rules->repeat.to > 0 && str[i] != '\0')
			{
				if (check_simple(str, parse_rules, &i) == false)
				{
					break ;
				}
				parse_rules->repeat.from -= (parse_rules->repeat.from > 0);
				if (check_flag(parse_rules->hflags, FLAG_REPEATER) == false ||
					check_flag(parse_rules->hflags, FLAG_FROM) == false ||
					check_flag(parse_rules->hflags, FLAG_TO) == true)
				{
					--parse_rules->repeat.to;
				}
			}
			if (parse_rules->repeat.from > 0)
			{
				return (REGEXP_FAIL);
			}
			parse_rules->repeat = parse_rules->mem_repeat;
		}

		printf("after {%c}\n\n", str[i]);

		parse_rules = parse_rules->next;
	}
	while (parse_rules != NULL);

	if (str[i] != '\0' || parse_rules != NULL)
	{
		return (REGEXP_FAIL);
	}
	return (REGEXP_OK);
}
