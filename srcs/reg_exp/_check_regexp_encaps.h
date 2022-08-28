/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _check_regexp_encaps.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lignigno <lignign@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/06 05:04:47 by lignigno          #+#    #+#             */
/*   Updated: 2022/08/28 07:45:51 by lignigno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CHECK_REGEXP_ENCAPS_H
# define _CHECK_REGEXP_ENCAPS_H

# include <stdlib.h>
# include <stdbool.h>
# include "utils.h"
# include "check_regexp.h"

# define ROUND_BRACKET			'('
# define BACK_ROUND_BRACKET		')'
# define SQUARE_BRACKET			'['
# define BACK_SQUARE_BRACKET	']'
# define FIGURE_BRACKET			'{'
# define BACK_FIGURE_BRACKET	'}'
# define DASH					'-'
# define COMMA					','
# define BACKSLASH				'\\'

/**
 * @brief	Flags to work.
 */
enum flag_e
{
	FLAG_SQUARE_BRACKET			= 0b1000000000000000, /*  */
	FLAG_FIGURE_BRACKET			= 0b0100000000000000, /*  */
	FLAG_FIGURE_BRACKET_DIGIT	= 0b0010000000000000, /*  */
	FLAG_FIGURE_BRACKET_COMMA	= 0b0001000000000000, /*  */
	FLAG_FIGURE_BRACKET_CHECKED	= 0b0000100000000000, /*  */
	FLAG_BACKSLASH				= 0b0000010000000000, /*  */
	FLAG_BACKSLASH_START		= 0b0000001000000000, /*  */
	FLAG_SUBPATTERN_BEGIN		= 0b0000000100000000, /*  */
	FLAG_SUBPATTERN_END			= 0b0000000010000000, /*  */
	FLAG_SINGLE_SYMBOL			= 0b0000000001000000, /* rule [] */
	FLAG_REVERSE				= 0b0000000000100000, /* reverse rule [] when '^' was set */
	FLAG_REPEATER				= 0b0000000000010000, /* rule {} */
	FLAG_SIMPLE					= 0b0000000000001000, /* rule some symbols */
	FLAG_FROM					= 0b0000000000000100, /*  */
	FLAG_TO						= 0b0000000000000010, /*  */
	FLAG_DASH					= 0b0000000000000001, /*  */
};

/* Type of flag holder */
typedef uint16_t hflags_t;

/**
 * @brief	A structure that stores the necessary records
 *			of the verification process in check_syntax function.
 */
typedef struct 
{
	hflags_t	hflags;
	char		previous_symbol;
	size_t		char_counter;
	size_t		num_round_bracket;
}	syntax_controller_t;

/**
 * @brief	Structure for storing the number of repetitions.
 */
typedef struct
{
	size_t	from;
	size_t	to;
}	repeat_t;

/**
 * @brief	A structure that stores the received regular expression rules
 *			for checking the string.
 */
typedef struct
{
	hflags_t	hflags;
	char *		str;
	char *		range;
	size_t		num_ranges;
	repeat_t	repeat;
	repeat_t	mem_repeat;
	void *		connect;
	void *		previous;
	void *		next;
}	regexp_rules_t;

/**
 * @brief	Regular expression syntax checker.
 *
 * @param[in]	regexp	String with a regular expression.
 *
 * @return	REGEXP_OK	- The regular expression is correct.
 *			REGEXP_FAIL	- The regular expression has a syntax error.
 */
regexp_ret_code_t	check_syntax(const char * regexp);

/**
 * @brief	Parsing a regular expression string
 *
 * @param[in]	regexp		String with regular expression.
 * @param[out]	pars_rules	Resulting rules for parsing.
 *
 * @return	REGEXP_OK			- The regular expression is parsed.
 *			REGEXP_ERR_MALLOC	- Unexpected malloc error.
 */
regexp_ret_code_t	parse_regexp(const char * regexp, regexp_rules_t ** pars_rules);

/**
 * @brief	Checking string according to the received regular expression rules.
 *
 * @param[in]	str			Sring to check.
 * @param[in]	pars_rules	Rules of parsing.
 *
 * @return	REGEXP_OK	- The string matched the regular expression.
 *			REGEXP_FAIL	- The string does not match the regular expression.
 */
regexp_ret_code_t	check_str(const char * str, regexp_rules_t * pars_rules);

/**
 * @brief	Add the flag to flag holder.
 * 
 * @param[in]	holder		Flag holder.
 * @param[in]	flag_mask	Bitmask of flags.
 */
void	set_flag(hflags_t * holder, hflags_t flag_mask);

/**
 * @brief	Unset the flag form flag holder.
 *
 * @param[in]	holder		Flag holder.
 * @param[in]	flag_mask	Bitmask of flags.
 */
void	unset_flag(hflags_t * holder, hflags_t flag_mask);

/**
 * @brief	Checking for multiple flags at the same time.
 *
 * @param[in]	holder		Flag holder.
 * @param[in]	flag_mask	Bitmask of flags.
 *
 * @return	true	- The bitmask of the flags is present in holder.
 *			false	- The bitmask of the flags is not present in the holder.
 */
bool	check_flag(hflags_t holder, hflags_t flag_mask);

#endif // _CHECK_REGEXP_ENCAPS_H