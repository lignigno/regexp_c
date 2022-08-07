/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lignigno <lignign@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/07 02:28:23 by lignigno          #+#    #+#             */
/*   Updated: 2022/08/07 05:01:12 by lignigno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "check_regexp.h"

#define BLUE_COLOR	"\033[38;2;0;255;251m"
#define RED_COLOR	"\033[38;2;250;0;0m"
#define GREEN_COLOR	"\033[38;2;0;200;0m"
#define UNSET_COLOR	"\033[39m"

int main(int argc, char const *argv[])
{
	if (argc < 3)
	{
		printf("%sneed \"str\" and \"regexp\"%s\n", RED_COLOR, UNSET_COLOR);
		return (0);
	}
	
	printf("\n%sneed \"%s\" and \"%s\"%s\n\n", BLUE_COLOR, argv[1], argv[2], UNSET_COLOR);

	if (check_regexp(argv[1], argv[2]) < REGEXP_OK)
		printf("%sNOT VALID%s\n", RED_COLOR, UNSET_COLOR);
	else
		printf("%sVALID%s\n", GREEN_COLOR, UNSET_COLOR);

	return (0);
}
