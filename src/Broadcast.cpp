/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Broadcast.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afons <afons@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 15:07:48 by afons             #+#    #+#             */
/*   Updated: 2026/07/20 15:34:47 by afons            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/User.hpp"
#include "core/Server.hpp"
#include <sys/socket.h>
#include <cstdio>

void Server::broadcast(const User *user, std::string message) {
	std::string messageError = user->getUsername() + message;
    if (send(user->getFd(), messageError.c_str(), messageError.size(), 0) == -1)
        std::perror("Send crashed.");
}
