/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _check_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lignigno <lignign@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/06 05:02:52 by lignigno          #+#    #+#             */
/*   Updated: 2022/08/26 23:20:39 by lignigno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <stdio.h>
#include "_check_regexp_encaps.h"

// _________________________________________________________________SUBFUNCTIONS

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
				if (check_flag(sctrl->hflags, FLAG_FIGURE_BRACKET_CHECKED) == false)
					set_flag(&sctrl->hflags, FLAG_FIGURE_BRACKET);
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

static regexp_ret_code_t	check_square_bracket(syntax_controller_t * sctrl, char symbol)
{
	if (check_flag(sctrl->hflags, FLAG_BACKSLASH_START) == true &&
		check_flag(sctrl->hflags, FLAG_DASH) == false)
	{
		unset_flag(&sctrl->hflags, FLAG_BACKSLASH_START);
	}
	else if (symbol == BACKSLASH)
	{
		set_flag(&sctrl->hflags, FLAG_BACKSLASH_START);
	}
	else if (ft_strchar("^]", symbol) != NULL && sctrl->char_counter < 2)
	{
		sctrl->char_counter = (symbol == BACK_SQUARE_BRACKET) ? 1 : 0;
	}
	else if (symbol == BACK_SQUARE_BRACKET &&
			check_flag(sctrl->hflags, FLAG_BACKSLASH_START) == false)
	{
		if (sctrl->char_counter < 2)
		{
			return (REGEXP_ARG_WRONG_REGEXP);
		}
		unset_flag(&sctrl->hflags, FLAG_DASH);
		unset_flag(&sctrl->hflags, FLAG_SQUARE_BRACKET);
		sctrl->char_counter = 0;
	}
	else if (check_flag(sctrl->hflags, FLAG_DASH) == true)
	{
		if (sctrl->previous_symbol > symbol)
		{
			return (REGEXP_ARG_WRONG_REGEXP);
		}
		if (sctrl->previous_symbol == '\0')
			unset_flag(&sctrl->hflags, FLAG_DASH);
		else
			sctrl->previous_symbol = '\0';
		unset_flag(&sctrl->hflags, FLAG_BACKSLASH_START);
	}
	else if (symbol == DASH && sctrl->char_counter > 1)
	{
		set_flag(&sctrl->hflags, FLAG_DASH);
	}

	if (check_flag(sctrl->hflags, FLAG_DASH) == false)
		sctrl->previous_symbol = symbol;
	++sctrl->char_counter;

	return (REGEXP_OK);
}

// _________________________________________________________CHECK FIGURE BRACKET

static regexp_ret_code_t	check_figure_bracket(syntax_controller_t * sctrl,
												char symbol,
												size_t symbol_id)
{
	if (symbol_id == 0 && symbol == FIGURE_BRACKET)
	{
		return (REGEXP_OK);
	}
	else if (ft_strchar("0123456789", symbol) != NULL)
	{
		set_flag(&sctrl->hflags, FLAG_FIGURE_BRACKET_DIGIT);
	}
	else if (symbol == COMMA &&
			check_flag(sctrl->hflags, FLAG_FIGURE_BRACKET_COMMA) == false)
	{
		set_flag(&sctrl->hflags, FLAG_FIGURE_BRACKET_COMMA);
	}
	else if (symbol == BACKSLASH &&
			check_flag(sctrl->hflags, FLAG_FIGURE_BRACKET_DIGIT) == true)
	{
		return (REGEXP_ARG_WRONG_REGEXP);
	}
	else
	{
		set_flag(&sctrl->hflags, FLAG_FIGURE_BRACKET_CHECKED);
		unset_flag(&sctrl->hflags, FLAG_FIGURE_BRACKET);
		unset_flag(&sctrl->hflags, FLAG_FIGURE_BRACKET_DIGIT);
		unset_flag(&sctrl->hflags, FLAG_FIGURE_BRACKET_COMMA);
	}

	return (REGEXP_OK);
}

// ______________________________________________________________CHECK BACKSLASH

static regexp_ret_code_t	check_backslash(syntax_controller_t * sctrl, char symbol)
{
	if (check_flag(sctrl->hflags, FLAG_BACKSLASH_START) == false)
	{
		set_flag(&sctrl->hflags, FLAG_BACKSLASH_START);
	}
	else if (ft_strchar("[{()|\\", symbol) == NULL)
	{
		return (REGEXP_ARG_WRONG_REGEXP);
	}
	else
	{
		unset_flag(&sctrl->hflags, FLAG_BACKSLASH);
		unset_flag(&sctrl->hflags, FLAG_BACKSLASH_START);
	}

	return (REGEXP_OK);
}

// ______________________________________________________________CHECK BACKSLASH

static regexp_ret_code_t	check_common(syntax_controller_t * sctrl, char symbol)
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
			ret = check_square_bracket(&sctrl, regexp[i]);
			if (ret < REGEXP_OK)
			{
				return (ret);
			}
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
		check_flag(sctrl.hflags, BACKSLASH) == true ||
		sctrl.num_round_bracket != 0)
	{
		return (REGEXP_ARG_WRONG_REGEXP);
	}

	return (REGEXP_OK);
}