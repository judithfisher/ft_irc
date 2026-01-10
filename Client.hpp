/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfischer <jfischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:45:13 by jfischer          #+#    #+#             */
/*   Updated: 2026/01/10 17:47:05 by jfischer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

class Client
{
	public:	
		Client();
		Client(const Client &other);
		Client &operator=(const Client &other);
		~Client();

	private:
		int client_fd;
		

};

#endif