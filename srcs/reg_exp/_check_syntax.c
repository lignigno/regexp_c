/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _check_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lignigno <lignign@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/06 05:02:52 by lignigno          #+#    #+#             */
/*   Updated: 2022/08/07 03:35:57 by lignigno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "_check_regexp_encaps.h"

enum flag_e
{
	FLAG_ROUND_BRACKET			= 0b10000000,
	FLAG_SQUARE_BRACKET			= 0b01000000,
	FLAG_FIGURE_BRACKET			= 0b00100000,
	FLAG_BACKSLASH				= 0b00010000,
	FLAG_START_SQUARE_BRACKET	= 0b00001000,
	FLAG_START_FIGURE_BRACKET	= 0b00000100,
	FLAG_END_FIGURE_BRACKET		= 0b00000010,
	FLAG_START_BACKSLASH		= 0b00000001,
};

// _________________________________________________________________SUBFUNCTIONS

// _____________________________________________________________________SET FLAG

inline static void	set_flag(hflags_t * handler, hflags_t flag_mask)
{
	*handler |= flag_mask;
}

// ___________________________________________________________________UNSET FLAG

inline static void	unset_flag(hflags_t * handler, hflags_t flag_mask)
{
	flag_mask = ~flag_mask;
	*handler &= flag_mask;
}

// ___________________________________________________________________CHECK FLAG

inline static bool	check_flag(hflags_t handler, hflags_t flag_mask)
{
	return ((handler & flag_mask) == flag_mask);
}

// _______________________________________________________________SWITCH CHECKER

static void	switch_checker(syntax_controller_t * sctrl, char symbol)
{
	if (check_flag(sctrl->hflags, FLAG_SQUARE_BRACKET) == false &&
		check_flag(sctrl->hflags, FLAG_FIGURE_BRACKET) == false &&
		check_flag(sctrl->hflags, FLAG_BACKSLASH) == false)
	{
		switch (symbol)
		{
			case SQUARE_BRACKET:
			{
				set_flag(&sctrl->hflags, FLAG_SQUARE_BRACKET);
				break;
			}
			case FIGURE_BRACKET:
			{
				if (check_flag(sctrl->hflags, FLAG_END_FIGURE_BRACKET) == false)
					set_flag(&sctrl->hflags, FLAG_START_FIGURE_BRACKET);
				break;
			}
			case BACKSLASH:
			{
				set_flag(&sctrl->hflags, FLAG_BACKSLASH);
				break;
			}
		}
	}
}

// _________________________________________________________CHECK FIGURE BRACKET

static regexp_ret_code_t check_figure_bracket(	syntax_controller_t * sctrl,
												char symbol,
												size_t symbol_id)
{
	if (symbol_id == 0 &&
		check_flag(sctrl->hflags, FLAG_START_FIGURE_BRACKET) == false)
	{
		set_flag(&sctrl->hflags, FLAG_START_FIGURE_BRACKET);
	}
	else if (ft_strchar("0123456789", symbol) != NULL &&
			(check_flag(sctrl->hflags, FLAG_START_FIGURE_BRACKET) == true ||
			check_flag(sctrl->hflags, FLAG_END_FIGURE_BRACKET) == true))
	{
		return (REGEXP_OK);
	}
	else if (ft_strchar(",", symbol) != NULL &&
			check_flag(sctrl->hflags, FLAG_START_FIGURE_BRACKET) == true)
	{
		set_flag(&sctrl->hflags, FLAG_END_FIGURE_BRACKET);
		unset_flag(&sctrl->hflags, FLAG_START_FIGURE_BRACKET);
	}
	else if (ft_strchar("}", symbol) != NULL)
	{
		printf("point 1\n");
		return (REGEXP_ARG_WRONG_REGEXP);
	}
	else
	{
		set_flag(&sctrl->hflags, FLAG_END_FIGURE_BRACKET);
		unset_flag(&sctrl->hflags, FLAG_FIGURE_BRACKET);
		unset_flag(&sctrl->hflags, FLAG_START_FIGURE_BRACKET);
	}

	return (REGEXP_OK);
}

// ______________________________________________________________CHECK BACKSLASH

regexp_ret_code_t	check_backslash(syntax_controller_t * sctrl, char symbol)
{
	if (check_flag(sctrl->hflags, FLAG_START_BACKSLASH) == false)
	{
		set_flag(&sctrl->hflags, FLAG_START_BACKSLASH);
	}
	else if (ft_strchar("[{()|\\", symbol) == NULL)
	{
		printf("point 2\n");
		return (REGEXP_ARG_WRONG_REGEXP);
	}
	else
	{
		unset_flag(&sctrl->hflags, FLAG_BACKSLASH);
		unset_flag(&sctrl->hflags, FLAG_START_BACKSLASH);
	}

	return (REGEXP_OK);
}

// ______________________________________________________________CHECK BACKSLASH

regexp_ret_code_t	check_common(syntax_controller_t * sctrl, char symbol)
{
	if (symbol == ROUND_BRACKET)
	{
		++sctrl->num_round_bracket;
	}
	else if (symbol == BACK_ROUND_BRACKET)
	{
		if (sctrl->num_round_bracket > 0)
		{
			--sctrl->num_round_bracket;
		}
		else
		{
			printf("point 3\n");
			return (REGEXP_ARG_WRONG_REGEXP);
		}
	}

	return (REGEXP_OK);
}

// ____________________________________________________________________MAIN FUNC

regexp_ret_code_t	check_syntax(const char * regexp)
{
	regexp_ret_code_t	ret;
	syntax_controller_t	sctrl = {0};

	for (size_t i = 0; regexp[i] != '\0'; ++i)
	{
		switch_checker(&sctrl, regexp[i]);

		/* check state */
		if (check_flag(sctrl.hflags, FLAG_SQUARE_BRACKET) == true)
		{
			if (regexp[i] == ']')
				unset_flag(&sctrl.hflags, FLAG_SQUARE_BRACKET);
		}
		else if (check_flag(sctrl.hflags, FLAG_FIGURE_BRACKET) == true)
		{
			ret = check_figure_bracket(&sctrl, regexp[i], i);
			if (ret < REGEXP_OK)
			{
				return (ret);
			}
		}
		else if (check_flag(sctrl.hflags, FLAG_BACKSLASH) == true)
		{
			ret = check_backslash(&sctrl, regexp[i]);
			if (ret < REGEXP_OK)
			{
				return (ret);
			}
		}
		else
		{
			ret = check_common(&sctrl, regexp[i]);
			if (ret < REGEXP_OK)
			{
				return (ret);
			}
		}
	}

	/* check that all brackets are closed */
	if (check_flag(sctrl.hflags, FLAG_SQUARE_BRACKET) == true ||
		check_flag(sctrl.hflags, FLAG_FIGURE_BRACKET) == true ||
		sctrl.num_round_bracket != 0)
	{
		printf("point 4 %u %zu\n", sctrl.hflags, sctrl.num_round_bracket);
		return (REGEXP_ARG_WRONG_REGEXP);
	}

	return (REGEXP_OK);
}