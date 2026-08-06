/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Uno.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:44:13 by nico              #+#    #+#             */
/*   Updated: 2026/08/05 10:54:21 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../includes/colors.hpp"
#include "Game.hpp"
#include <iostream>

class Uno: public Game
{
	private:

	public:
		// == Constuctor & destructor ==
		Uno();
		~Uno();
};
