/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delegation.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lignigno <lignign@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 06:38:18 by lignigno          #+#    #+#             */
/*   Updated: 2022/08/19 16:15:01 by lignigno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

// _____________________________________________________________________________

struct Phone
{
	virtual void call(void) = 0;
};

// _____________________________________________________________________________

struct Phone_from_1920 : public Phone
{
	void call(void)
	{
		std::cout << "get to the phone booth and call\n";
	}
};

// _____________________________________________________________________________

struct Phone_from_2020 : public Phone
{
	void call(void)
	{
		std::cout << "get out your phone and call\n";
	}
};

// _____________________________________________________________________________

class Human : public Phone
{
	Phone * phone;

	Human(void) {};

	public :

		Human(int current_year)
		{
			if (current_year < 1990)
				phone = new Phone_from_1920();
			else
				phone = new Phone_from_2020();
		}
	
		void call(void)
		{
			phone->call();
		}
};

// _________________________________________________________________________MAIN

int main(void)
{
	Human mr_Bin(2001);

	mr_Bin.call();

	return (0);
}

