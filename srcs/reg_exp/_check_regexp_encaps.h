/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _check_regexp_encaps.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lignigno <lignign@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/06 05:04:47 by lignigno          #+#    #+#             */
/*   Updated: 2022/08/07 02:20:15 by lignigno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CHECK_REGEXP_ENCAPS_H
# define _CHECK_REGEXP_ENCAPS_H

# include <stdlib.h>
# include <stdbool.h>
# include "utils.h"
# include "check_regexp.h"

# define ROUND_BRACKET		'('
# define BACK_ROUND_BRACKET	')'
# define SQUARE_BRACKET		'['
# define FIGURE_BRACKET		'{'
# define BACKSLASH			'\\'

typedef uint8_t hflags_t;

/**
 * @brief	A structure that stores the necessary records
 *			of the verification process.
 *
 */
typedef struct 
{
	hflags_t	hflags;
	size_t		num_round_bracket;
}	syntax_controller_t;

/**
 * @brief Regular expression syntax checker.
 *
 * @param[in]	regexp	A string with a regular expression.
 *
 * @return	REGEXP_OK	- The regular expression is correct.
 *			REGEXP_FAIL	- The regular expression has a syntax error.
 */
regexp_ret_code_t	check_syntax(const char * regexp);

#endif // _CHECK_REGEXP_ENCAPS_H