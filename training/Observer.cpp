/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Observer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lignigno <lignign@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 08:31:14 by lignigno          #+#    #+#             */
/*   Updated: 2022/08/19 17:58:27 by lignigno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <list>

// _____________________________________________________________________________

class Interface_Subject
{
	public :
		virtual void		update_state(const std::string & state) = 0;
		virtual std::string	get_state(void) const = 0;
};

// _____________________________________________________________________________

class Interface_Observer
{
	public :
		virtual void Event(const Interface_Subject & obj) = 0;
};

// _____________________________________________________________________________

class Observer : public Interface_Observer
{
	private :
		std::string name;

	public :
		Observer(const std::string & name) : name(name)
		{};

		void Event(const Interface_Subject & obj)
		{
			std::cout << "Obser \"" << name << "\" получил событие \"" << obj.get_state() << "\"\n";
		}

	private :
		Observer(void){};
};

// _____________________________________________________________________________

class Observer_2 : public Interface_Observer
{
	private :
		std::string name;

	public :
		Observer_2(const std::string & name) : name(name)
		{};

		void Event(const Interface_Subject & obj)
		{
			(void)obj;
			std::cout << "Obser_2 \"" << name << "\" не использует Interface_Subject & obj\n";
		}

	private :
		Observer_2(void){};
};

// _____________________________________________________________________________

class Subject : public Interface_Subject
{
	private :
		std::string state;
		std::list<Interface_Observer *> observers;

	public :

		Subject(void) : state("start_state")
		{};

		void	update_state(const std::string & state)
		{
			this->state = state;

			for (std::list<Interface_Observer *>::iterator	i = observers.begin(),
															end = observers.end();
				i != end;
				++i)
			{
				(*i)->Event(*this);
			}
		};

		void	add_observer(Interface_Observer & observer)
		{
			observers.push_back(&observer);
		}

		std::string	get_state(void) const
		{
			return (this->state);
		};

};

// _____________________________________________________________________________

int main(void)
{
	Subject s1;
	Subject s2;
	Subject s3;
	Subject s4;

	Observer o_01("01");
	Observer o_02("02");
	Observer o_03("03");
	Observer o_04("04");
	Observer o_05("05");
	Observer o_06("06");
	Observer o_07("07");
	Observer o_08("08");
	Observer o_09("09");
	Observer o_10("10");
	Observer o_11("11");
	Observer o_12("12");
	Observer o_13("13");

	Observer_2 obus_2("obobos");

	s1.add_observer(o_11);
	s1.add_observer(o_03);
	s1.add_observer(o_09);
	s1.add_observer(o_07);
	s1.add_observer(o_06);
	s1.add_observer(o_01);
	s1.add_observer(o_13);

	s2.add_observer(o_05);
	s2.add_observer(o_06);
	s2.add_observer(o_02);
	s2.add_observer(o_04);
	s2.add_observer(o_07);
	s2.add_observer(o_10);
	s2.add_observer(o_11);

	s3.add_observer(o_12);
	s3.add_observer(o_11);
	s3.add_observer(o_10);
	s3.add_observer(o_01);
	s3.add_observer(o_03);
	s3.add_observer(o_07);
	s3.add_observer(o_09);

	s4.add_observer(o_06);
	s4.add_observer(o_09);
	s4.add_observer(o_01);
	s4.add_observer(o_03);
	s4.add_observer(o_13);
	s4.add_observer(o_12);
	s4.add_observer(o_07);
	s4.add_observer(obus_2);


	std::cout << "s1\n";
	s1.update_state("biba in");

	std::cout << "s2\n";
	s2.update_state("boba out");

	std::cout << "s3\n";
	s3.update_state("soda");

	std::cout << "s4\n";
	s4.update_state("tenno skum");

	return (0);
}
