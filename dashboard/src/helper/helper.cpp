/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 13:53:18 by nico              #+#    #+#             */
/*   Updated: 2026/08/27 14:19:55 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Dashboard.hpp"

std::string convertIntToString(int num) {
	std::ostringstream oss;
	oss << num;
	return (oss.str());
}

std::string timestamp() {
	std::time_t t = std::time(NULL);
	char buf[32];
	std::strftime(buf, sizeof(buf), "%H:%M:%S", std::localtime(&t));
	return (std::string(buf));
}

