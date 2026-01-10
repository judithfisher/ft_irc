/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:05:44 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/09 18:45:44 by jfischer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector> 				//-> for vector
#include <sys/socket.h> 		//-> for socket()
#include <sys/types.h> 			//-> for socket()
#include <netinet/in.h> 		//-> for sockaddr_in
#include <fcntl.h> 				//-> for fcntl()
#include <unistd.h> 			//-> for close()
#include <arpa/inet.h> 			//-> for inet_ntoa()
#include <poll.h> 				//-> for poll()
#include <csignal> 				//-> for signal()


/* 
sa_family_t: defines the type of address (AF_INET for IPv4), unsigned short
in_port_t: defines the port number, unsigned short
in_addr: structure that contains a single field, s_addr, which is the IP address in network byte order (uint32_t s_addr);
*/

struct sockaddr_in 
{
    sa_family_t sin_family;   	// AF_INET
    in_port_t   sin_port;     	// Which port?
    struct in_addr sin_addr;  	// Which IP?
};