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
											size_t * i)
{
	(void)regexp;
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

	/* end logic */
	*current_rule = rule;
	set_flag(&rule->hflags, FLAG_SUBPATTERN);

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
	if (check_flag(current_rule->hflags, FLAG_SUBPATTERN) == true)
	{
		do
		{
			current_rule = current_rule->previous;
		}
		while (	current_rule != NULL &&
				check_flag(current_rule->hflags, FLAG_SUBPATTERN) == false);
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
		unset_flag(&rule->hflags, FLAG_BACKSLASH);

		if (check_flag(rule->hflags, FLAG_BACKSLASH) == false)
		{
			if (regexp[*i] == BACKSLASH)
			{
				set_flag(&rule->hflags, FLAG_BACKSLASH);
			}
			else if (regexp[*i] == DASH)
			{
				for (size_t j = 0; j < 2; ++j)
				{
					if (ft_join(&rule->range, &regexp[*i], 1) == false)
					{
						free(rule->str);
						free(rule->range);
						free(rule);
						return (REGEXP_ERR_MALLOC);
					}
					++(*i);
				}
				++rule->num_ranges;
			}
			else if (ft_strchar(rule->str, regexp[*i]) == NULL)
			{
				if (ft_join(&rule->str, &regexp[*i], 1) == false)
				{
					free(rule->str);
					free(rule->range);
					free(rule);
					return (REGEXP_ERR_MALLOC);
				}
			}
		}

		++(*i);

		/* skip only backslash and remember his */
		if (regexp[*i] == BACKSLASH)
		{
			set_flag(&rule->hflags, FLAG_BACKSLASH);
			++(*i);
			continue ;
		}

		if (regexp[*i + 1] == DASH &&
			regexp[*i + 2] != BACK_SQUARE_BRACKET)
		{
			if (ft_join(&rule->range, &regexp[*i], 1) == false)
			{
				free(rule->str);
				free(rule->range);
				free(rule);
				return (REGEXP_ERR_MALLOC);
			}
		}
		else if (regexp[*i + 1])
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

	result = NULL;
	last = NULL;
	for (size_t i = 0; regexp[i] != '\0';)
	{
		tmp = NULL;
		if (regexp[i] == ROUND_BRACKET || regexp[i] == BACK_ROUND_BRACKET)
		{
			ret = parse_subpattern(regexp, &tmp, &i);
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

	*parse_rules = result;

	return (REGEXP_OK);
}
