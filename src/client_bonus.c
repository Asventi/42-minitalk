/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pjarnac <pjarnac@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:13:36 by pjarnac           #+#    #+#             */
/*   Updated: 2025/01/23 15:13:36 by pjarnac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "libft.h"

void	handler(int sig)
{
	if (sig == SIGUSR2)
	{
		ft_printf("Data has been received and processed by the server.\n");
		exit(EXIT_SUCCESS);
	}
}

void	send_len(pid_t pid, size_t len)
{
	int8_t	bits;

	bits = 31;
	while (bits >= 0)
	{
		usleep(1);
		if (!(len & 0x1 << bits))
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		bits--;
		pause();
	}
}

void	send_mess(pid_t pid, char *msg)
{
	int8_t	bits;

	while (*msg)
	{
		bits = 7;
		while (bits >= 0)
		{
			usleep(1);
			if (!(*msg & 0x1 << bits))
				kill(pid, SIGUSR1);
			else
				kill(pid, SIGUSR2);
			bits--;
			pause();
		}
		msg++;
	}
}

int	main(int c, char **args)
{
	struct sigaction	action;
	pid_t				pid;

	if (c != 3)
		return (ft_printf("Invalid number of arguments,"
				"usage: ./client serverPid\n"));
	pid = ft_atoi(args[1]);
	ft_bzero(&action, sizeof (action));
	action.sa_handler = handler;
	if (sigaction(SIGUSR1, &action, NULL) == -1)
		return (-1);
	if (sigaction(SIGUSR2, &action, NULL) == -1)
		return (-1);
	if (pid == 0 || kill(pid, 0) == -1)
		return (ft_printf("Invalid PID\n"));
	send_len(pid, ft_strlen(args[2]));
	send_mess(pid, args[2]);
	pause();
	return (0);
}
