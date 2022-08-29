/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_regexp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lignigno <lignign@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/06 04:30:26 by lignigno          #+#    #+#             */
/*   Updated: 2022/08/29 23:17:57 by lignigno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "_check_regexp_encaps.h"

// [FOR TESTS] [FOR TESTS] [FOR TESTS] [FOR TESTS] [FOR TESTS] [FOR TESTS] [FOR TESTS]

#define WIDTH_COLUMN 10

static char *	flag_names[] = {
	"FLAG_SQUARE_BRACKET",
	"FLAG_FIGURE_BRACKET",
	"FLAG_FIGURE_BRACKET_DIGIT",
	"FLAG_FIGURE_BRACKET_COMMA",
	"FLAG_FIGURE_BRACKET_CHECKED",
	"FLAG_BACKSLASH",
	"FLAG_BACKSLASH_START",
	"FLAG_SUBPATTERN_BEGIN",
	"FLAG_SUBPATTERN_END",
	"FLAG_ALTERNATIVE",
	"FLAG_SINGLE_SYMBOL",
	"FLAG_REVERSE",
	"FLAG_REPEATER",
	"FLAG_SIMPLE",
	"FLAG_FROM",
	"FLAG_TO",
	"FLAG_DASH",
};


void print_rules(const regexp_rules_t * rules)
{
	while (rules != NULL)
	{
		printf("__________________________________\n");

		printf("current %p\n", rules);

		printf("%-*s : %u\n", WIDTH_COLUMN, "hflags", rules->hflags);
		for (size_t i = 0; i < 17 /* num of flags */ ; i++)
		{
			if (check_flag(rules->hflags, FLAG_SQUARE_BRACKET >> i))
				printf("\t- %s\n", flag_names[i]);
		}

		printf("%-*s : {%s}\n", WIDTH_COLUMN, "str", rules->str);

		printf("%-*s : {%s}\n", WIDTH_COLUMN, "range", rules->range);

		printf("%-*s : %zu\n", WIDTH_COLUMN, "num_ranges", rules->num_ranges);

		printf("%-*s :\n", WIDTH_COLUMN, "repeat");
		printf("\t- from : %zu\n", rules->repeat.from);
		printf("\t- to   : %zu\n", rules->repeat.to);

		printf("%-*s :\n", WIDTH_COLUMN, "mem_repeat");
		printf("\t- from : %zu\n", rules->mem_repeat.from);
		printf("\t- to   : %zu\n", rules->mem_repeat.to);
	
		printf("%-*s : %p\n", WIDTH_COLUMN, "connect", rules->connect);
		
		printf("%-*s : %p\n", WIDTH_COLUMN, "previous", rules->previous);

		printf("%-*s : %p\n", WIDTH_COLUMN, "next", rules->next);

		rules = rules->next;
	}
	printf("__________________________________\n");
}

// [FOR TESTS] [FOR TESTS] [FOR TESTS] [FOR TESTS] [FOR TESTS] [FOR TESTS] [FOR TESTS]

regexp_ret_code_t check_regexp(const char * str, const char * regexp)
{
	regexp_ret_code_t	ret;
	regexp_rules_t *	parse_rules;

	if (str == NULL || regexp == NULL)
	{
		return (REGEXP_ARG_NULL);
	}

	printf("point 1\n");
	ret = check_syntax(regexp);
	if (ret < REGEXP_OK)
	{
		return (ret);
	}

	printf("point 2\n");
	ret = parse_regexp(regexp, &parse_rules);
	if (ret < REGEXP_OK)
	{
		return (ret);
	}

	print_rules(parse_rules);

	printf("point 3\n");
	ret = check_str(str, parse_rules);
	if (ret < REGEXP_OK)
	{
		return (ret);
	}

	return (ret);
}
