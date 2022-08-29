/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _parse_regexp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lignigno <lignign@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 18:32:08 by lignigno          #+#    #+#             */
/*   Updated: 2022/08/24 10:16:44 by lignigno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "_check_regexp_encaps.h"

// _________________________________________________________________SUBFUNCTIONS

// __________________________________________________________________IS REPEATER

/* Maybe it can be make as a common part to check the repeater. */

static bool	is_repeater(const char * regexp)
{
	bool	flag_num;
	bool	flag_comma;

	if (*regexp != FIGURE_BRACKET)
	{
		return (false);
	}

	flag_num = false;
	flag_comma = false;
	for (size_t i = 1; regexp[i] != '\0'; ++i)
	{
		if ('0' <= regexp[i] && regexp[i] <= '9')
		{
			flag_num = true;
		}
		else if (regexp[i] == COMMA)
		{
			if (flag_comma == true)
				return (false);
			flag_comma = true;
		}
		else if (regexp[i] == BACK_FIGURE_BRACKET)
		{
			if (regexp[i - 1] != FIGURE_BRACKET && flag_num == true)
				return (true);
			return (false);
		}
		else
			return (false);
	}

	/* The symbols have run out and the repeater has not closed. */
	return (false);
}

// ______________________________________________________________PARS SUBPATTERN

static regexp_ret_code_t	parse_subpattern(const char * regexp,
											regexp_rules_t ** current_rule,
											regexp_rules_t * last,
											size_t * i)
{
	size_t				counter;
	regexp_rules_t *	rule;
	regexp_rules_t *	tmp_rule;

	/* create rule */
	rule = (regexp_rules_t *)malloc(sizeof(regexp_rules_t));
	if (rule == NULL)
	{
		return (REGEXP_ERR_MALLOC);
	}
	*rule = (regexp_rules_t){0};

	/* parsing */
	if (regexp[*i] == ROUND_BRACKET)
		set_flag(&rule->hflags, FLAG_SUBPATTERN_BEGIN);
	else
		set_flag(&rule->hflags, FLAG_SUBPATTERN_END);
	++(*i);

	/* end logic */
	*current_rule = rule;
	if (check_flag(rule->hflags, FLAG_SUBPATTERN_END) == true)
	{
		tmp_rule = last;
		counter = 0;
		while (	counter > 0 ||
				check_flag(tmp_rule->hflags, FLAG_SUBPATTERN_BEGIN) == false)
		{
			// printf("\n");
			if (check_flag(tmp_rule->hflags, FLAG_SUBPATTERN_BEGIN) == true)
				--counter;
			else if (check_flag(tmp_rule->hflags, FLAG_SUBPATTERN_END) == true)
				++counter;
			tmp_rule = tmp_rule->previous;
		}
		rule->connect = tmp_rule;
		tmp_rule->connect = rule;
	}

	return (REGEXP_OK);
}

// _____________________________________________________________PARS ALTERNATIVE

#include <stdio.h>

static regexp_ret_code_t	parse_alternative(	regexp_rules_t ** result,
												regexp_rules_t ** last,
												size_t * i,
												bool * flag_alternative)
{
	regexp_ret_code_t	ret;
	size_t				counter;
	regexp_rules_t *	rule;
	regexp_rules_t *	tmp_rule;
	size_t				j;

	*flag_alternative = true;
	++(*i);

	if (*result == NULL)
	{
		printf("p 1\n");
		j = 0;
		ret = parse_subpattern("(", result, NULL, &j);
		if (ret < REGEXP_OK)
		{
			return (ret);
		}
		*last = *result;
		set_flag(&(*result)->hflags, FLAG_ALTERNATIVE);
		printf("pointer %p (*result)->hflags %u\n", *result, (*result)->hflags);
	}
	else
	{
		printf("p 2\n");
		tmp_rule = *last;
		counter = 0;
		while (	tmp_rule->previous != NULL &&
				(check_flag(tmp_rule->hflags, FLAG_SUBPATTERN_BEGIN) == false || counter > 0))
		{
			if (check_flag(tmp_rule->hflags, FLAG_SUBPATTERN_BEGIN) == true)
				--counter;
			else if (check_flag(tmp_rule->hflags, FLAG_SUBPATTERN_END) == true)
				++counter;
			tmp_rule = tmp_rule->previous;
		}

		if (check_flag(tmp_rule->hflags, FLAG_ALTERNATIVE) == false)
		{
			j = 0;
			ret = parse_subpattern("(", &rule, NULL, &j);
			if (ret < REGEXP_OK)
			{
				return (ret);
			}
			set_flag(&rule->hflags, FLAG_ALTERNATIVE);

			if (check_flag(tmp_rule->hflags, FLAG_SUBPATTERN_BEGIN) == true &&
				counter <= 0)
			{
				rule->next = tmp_rule->next;
				rule->previous = tmp_rule;
				tmp_rule->next = rule;
				if (rule->next != NULL)
					((regexp_rules_t *)rule->next)->previous = rule;
				else
					*last = rule;
			}
			else
			{
				*result = rule;
				rule->next = tmp_rule;
				tmp_rule->previous = rule;
			}
		}
	}

	for (j = 0; j < 2;)
	{
		ret = parse_subpattern(")(", &rule, *last, &j);
		if (ret < REGEXP_OK)
		{
			return (ret);
		}
		rule->previous = *last;
		(*last)->next = rule;
		*last = rule;
		set_flag(&rule->hflags, FLAG_ALTERNATIVE);
	}
	

	// printf("p 3\n");

	// j = 0;
	// ret = parse_subpattern(")", &rule, *last, &j);
	// if (ret < REGEXP_OK)
	// {
	// 	return (ret);
	// }
	// rule->previous = *last;
	// (*last)->next = rule;
	// *last = rule;
	// set_flag(&rule->hflags, FLAG_ALTERNATIVE);

	// printf("pointer %p rule->hflags %u\n", rule, rule->hflags);

	// printf("p 4\n");

	// j = 0;
	// ret = parse_subpattern("(", &rule, NULL, &j);
	// if (ret < REGEXP_OK)
	// {
	// 	return (ret);
	// }
	// rule->previous = *last;
	// (*last)->next = rule;
	// *last = rule;
	// set_flag(&rule->hflags, FLAG_ALTERNATIVE);

	// printf("p 5\n");
	// printf("pointer %p rule->hflags %u\n", rule, rule->hflags);

	return (REGEXP_OK);
}

// ____________________________________________________________CLOSE ALTERNATIVE

static regexp_ret_code_t	close_alternative(regexp_rules_t ** last)
{
	regexp_ret_code_t	ret;
	regexp_rules_t *	tmp_rule;
	size_t				counter;
	size_t				j;

	printf("p 6\n");
	j = 0;
	ret = parse_subpattern(")", (regexp_rules_t **)(&(*last)->next), *last, &j);
	if (ret < REGEXP_OK)
	{
		return (ret);
	}
	((regexp_rules_t *)(*last)->next)->previous = *last;
	*last = (*last)->next;

	printf("p 7\n");
	tmp_rule = (*last)->previous;
	counter = 0;
	while (check_flag(tmp_rule->hflags, FLAG_SUBPATTERN_BEGIN) == false || counter > 0)
	{
		printf("%p %zu\n", tmp_rule, counter);
		if (check_flag(tmp_rule->hflags, FLAG_SUBPATTERN_BEGIN) == true)
			--counter;
		else if (check_flag(tmp_rule->hflags, FLAG_SUBPATTERN_END) == true)
			++counter;
		tmp_rule = tmp_rule->previous;
	}
	printf("%p %zu\n", tmp_rule, counter);
	unset_flag(&tmp_rule->hflags, FLAG_ALTERNATIVE);

	return (REGEXP_OK);
}

// __________________________________________________________________PARS SIMPLE

static regexp_ret_code_t	parse_simple(	const char * regexp,
											regexp_rules_t ** current_rule,
											size_t * i)
{
	regexp_rules_t *	rule;
	bool				flag_is_first;

	/* create rule */
	rule = (regexp_rules_t *)malloc(sizeof(regexp_rules_t));
	if (rule == NULL)
	{
		return (REGEXP_ERR_MALLOC);
	}
	*rule = (regexp_rules_t){0};
	flag_is_first = true;

	/* parsing */
	while (regexp[*i] != '\0')
	{
		if (check_flag(rule->hflags, FLAG_BACKSLASH) == false)
		{
			if (regexp[*i] == BACKSLASH)
			{
				set_flag(&rule->hflags, FLAG_BACKSLASH);
			}
			else if (regexp[*i] == ROUND_BRACKET ||
					regexp[*i] == BACK_ROUND_BRACKET ||
					regexp[*i] == SQUARE_BRACKET ||
					regexp[*i] == ALTERNATIVE ||
					is_repeater(&regexp[*i + 1]) == true)
			{
				if (flag_is_first == true)
				{
					if (ft_join(&rule->str, &regexp[*i], 1) == false)
					{
						free(rule->str);
						free(rule);
						return (REGEXP_ERR_MALLOC);
					}
					++(*i);
				}
				break ;
			}
			else
			{
				if (ft_join(&rule->str, &regexp[*i], 1) == false)
				{
					free(rule->str);
					free(rule);
					return (REGEXP_ERR_MALLOC);
				}
			}
		}
		else
		{
			if (ft_join(&rule->str, &regexp[*i], 1) == false)
			{
				free(rule->str);
				free(rule);
				return (REGEXP_ERR_MALLOC);
			}
			unset_flag(&rule->hflags, FLAG_BACKSLASH);
		}

		flag_is_first = false;
		++(*i);
	}

	/* end logic */
	if (shrink_to_fit(&rule->str) == false)
	{
		free(rule->str);
		free(rule);
		return (REGEXP_ERR_MALLOC);
	}
	*current_rule = rule;
	set_flag(&rule->hflags, FLAG_SIMPLE);

	return (REGEXP_OK);
}

// ________________________________________________________________PARS REPEATER

static regexp_ret_code_t	parse_repeater(	const char * regexp,
											regexp_rules_t * current_rule,
											size_t * i)
{
	size_t				counter;

	if (check_flag(current_rule->hflags, FLAG_SUBPATTERN_END) == true)
	{
		counter = 1;
		while (counter > 0)
		{
			current_rule = current_rule->previous;
			if (check_flag(current_rule->hflags, FLAG_SUBPATTERN_BEGIN) == true)
				--counter;
			else if (check_flag(current_rule->hflags, FLAG_SUBPATTERN_END) == true)
				++counter;
		}
	}

	/* parsing */
	++(*i);
	if (regexp[*i] != COMMA)
	{
		/* atoll */
		current_rule->repeat.from = atoll(&regexp[*i]);
		set_flag(&current_rule->hflags, FLAG_FROM);
	}
	while (regexp[*i] != COMMA && regexp[*i] != BACK_FIGURE_BRACKET)
		++(*i);
	if (regexp[*i] == BACK_FIGURE_BRACKET)
	{
		unset_flag(&current_rule->hflags, FLAG_FROM);
	}
	else
	{
		++(*i);
		if (regexp[*i] != BACK_FIGURE_BRACKET)
		{
			/* atoll */
			current_rule->repeat.to = atoll(&regexp[*i]);
			set_flag(&current_rule->hflags, FLAG_TO);
		}
		while (regexp[*i] != BACK_FIGURE_BRACKET)
			++(*i);
	}

	/* end logic */
	current_rule->mem_repeat = current_rule->repeat;
	set_flag(&current_rule->hflags, FLAG_REPEATER);
	++(*i);

	return (REGEXP_OK);
}

// ___________________________________________________________PARS SINGLE SYMBOL

/* need later optimized */

static regexp_ret_code_t	parse_single_symbol(	const char * regexp,
												regexp_rules_t ** current_rule,
												size_t * i)
{
	regexp_rules_t *	rule;

	/* create rule */
	rule = (regexp_rules_t *)malloc(sizeof(regexp_rules_t));
	if (rule == NULL)
	{
		return (REGEXP_ERR_MALLOC);
	}
	*rule = (regexp_rules_t){0};

	/* parsing */
	++(*i);
	if (regexp[*i] == '^')
	{
		set_flag(&rule->hflags, FLAG_REVERSE);
		++(*i);
	}
	while (	regexp[*i] != BACK_SQUARE_BRACKET ||
			check_flag(rule->hflags, FLAG_BACKSLASH) == true)
	{
		/* skip only backslash and remember his */
		if (regexp[*i] == BACKSLASH)
		{
			set_flag(&rule->hflags, FLAG_BACKSLASH);
			++(*i);
			continue ;
		}
		else if (check_flag(rule->hflags, FLAG_BACKSLASH) == true)
		{
			unset_flag(&rule->hflags, FLAG_BACKSLASH);
		}

		/* get range if the next symbol dash and the following symbol BACK_SQUARE_BRACKET */
		if (regexp[*i + 1] == DASH &&
			regexp[*i + 2] != BACK_SQUARE_BRACKET)
		{
			for (size_t j = 0; j < 4; ++j)
			{
				if (j == 0 || (j == 2 && regexp[*i] != BACKSLASH) || j == 3)
				{
					if (ft_join(&rule->range, &regexp[*i], 1) == false)
					{
						free(rule->str);
						free(rule->range);
						free(rule);
						return (REGEXP_ERR_MALLOC);
					}
					if (j > 1)
						break ;
				}
				++(*i);
			}
			++rule->num_ranges;
		}
		else
		{
			if (ft_join(&rule->str, &regexp[*i], 1) == false)
			{
				free(rule->str);
				free(rule->range);
				free(rule);
				return (REGEXP_ERR_MALLOC);
			}
		}
		++(*i);
	}

	/* end logic */
	*current_rule = rule;
	if (shrink_to_fit(&rule->str) == false)
	{
		free(rule->str);
		free(rule);
		return (REGEXP_ERR_MALLOC);
	}
	if (shrink_to_fit(&rule->range) == false)
	{
		free(rule->str);
		free(rule);
		return (REGEXP_ERR_MALLOC);
	}
	set_flag(&rule->hflags, FLAG_SINGLE_SYMBOL);
	++(*i);

	return (REGEXP_OK);
}

// ____________________________________________________________________MAIN FUNC

regexp_ret_code_t	parse_regexp(const char * regexp,
								regexp_rules_t ** parse_rules)
{
	regexp_ret_code_t	ret;
	regexp_rules_t *	result;
	regexp_rules_t *	tmp;
	regexp_rules_t *	last;
	size_t				counter_subpattern;
	bool				flag_alternative;

	result = NULL;
	last = NULL;
	counter_subpattern = 0;
	flag_alternative = false;
	for (size_t i = 0; regexp[i] != '\0';)
	{
		tmp = NULL;
		if (regexp[i] == ROUND_BRACKET || regexp[i] == BACK_ROUND_BRACKET)
		{
			/* if exist and BACK_ROUND_BRACKET */
			if (flag_alternative == true)
			{
				if (counter_subpattern > 0 && regexp[i] == BACK_ROUND_BRACKET)
				{
					--counter_subpattern;
				}
				else if (regexp[i] == BACK_ROUND_BRACKET)
				{
					close_alternative(&last);
				}
				else
				{
					++counter_subpattern;
				}
			}

			ret = parse_subpattern(regexp, &tmp, last, &i);
			if (ret < REGEXP_OK)
			{
				return (ret);
			}
		}
		else if (regexp[i] == ALTERNATIVE)
		{
			ret = parse_alternative(&result, &last, &i, &flag_alternative);
			if (ret < REGEXP_OK)
			{
				return (ret);
			}
		}
		else if (regexp[i] == SQUARE_BRACKET)
		{
			ret = parse_single_symbol(regexp, &tmp, &i);
			if (ret < REGEXP_OK)
			{
				return (ret);
			}
		}
		else if (regexp[i] == FIGURE_BRACKET &&
				is_repeater(&regexp[i]) == true)
		{
			ret = parse_repeater(regexp, last, &i);
			if (ret < REGEXP_OK)
			{
				return (ret);
			}
		}
		else
		{
			ret = parse_simple(regexp, &tmp, &i);
			if (ret < REGEXP_OK)
			{
				return (ret);
			}
		}

		if (tmp != NULL)
		{
			if (result == NULL)
				result = tmp;
			if (last == NULL)
				last = tmp;
			else
			{
				last->next = tmp;
				tmp = last;
				last = last->next;
				last->previous = tmp;
			}
		}
	}

	if (flag_alternative == true)
	{
		close_alternative(&last);
	}

	*parse_rules = result;

	return (REGEXP_OK);
}
