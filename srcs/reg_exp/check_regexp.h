/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_regexp.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lignigno <lignign@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/06 04:31:25 by lignigno          #+#    #+#             */
/*   Updated: 2022/08/25 11:41:00 by lignigno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHECK_REGEXP_H
# define CHECK_REGEXP_H

# include <stdint.h>

/**
 * @brief	Retcodes of regular expression checker.
 *
 * @note	A good result when the returned value is greater than
 *			or equal to REGEXP_OK.
 *			A bad result when the returned value is less than REGEXP_OK.
 */


# define REGEXP_OK					(0x80)
# define REGEXP_FAIL				(REGEXP_OK - 1)
# define REGEXP_ERR_MALLOC			(REGEXP_OK - 2)
# define REGEXP_ARG_NULL			(REGEXP_OK - 3)
# define REGEXP_ARG_WRONG_REGEXP	(REGEXP_OK - 4)

typedef uint8_t regexp_ret_code_t;

/*
	SYNTAX

	Для экранирования управляющих символов (таких как '()[{^-\') необходимо поставить перед ними двойной слеш.
	Строке "[" будет соответствовать регулярное выражение вида "\\[".

	"fr67fuy.,34F_@"	symple
	"\"					shielding
	"|"					exclusive or
	"()"				subpattern
	"[]"				single symbol
	"[^]"
	"[a-z]"
	"{1}"				repeater
	"{1,}"
	"{1,2}"
	"{,2}"

	not a bug, but a feature
	"123\{" need to fix

	preview v2

	"<>"				collection
	"?" 				{0,1}
	"*" 				{0,}
	"+" 				{1,}
*/

/**
 * @brief	Checking a string for matching a regular expression.
 *
 * @details	A string is considered valid only when it fully matches
 *			a regular expression.
 *			Not valid	: check_regexp("123    ", "[0-9]{1,3}")
 *			Valid		: check_regexp("123    ", "[0-9]{1,3}[ ]{1,}")
 *
 * @param[in]	str		The string that needs to be checked.
 * @param[in]	regexp	Regular expression for string validation.
 *
 * @return	REGEXP_OK				- The string matched the regular expression.
 *			REGEXP_FAIL				- The string does not match the regular expression.
 *			REGEXP_ERR_MALLOC		- Unexpected malloc error.
 *			REGEXP_ARG_NULL			- One of the arguments is NULL.
 *			REGEXP_ARG_WRONG_REGEXP	- A regular expression has a syntax error.
 */
regexp_ret_code_t check_regexp(const char * str, const char * regexp);

#endif // CHECK_REGEXP_H
