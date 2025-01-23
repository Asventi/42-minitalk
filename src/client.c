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
#include <unistd.h>

#include "libft.h"

void	handler(int sig)
{

}

int	main(int c, char **args)
{
	struct sigaction	action;
	pid_t				pid;

	if (c != 2)
		return (ft_printf("Invalid number of arguments, usage: ./client serverPid\n"));
	pid = ft_atoi(args[1]);
	ft_bzero(&action, sizeof (action));
	action.sa_handler = handler;
	if (sigaction(SIGUSR1, &action, NULL) == -1)
		return (-1);
	if (sigaction(SIGUSR2, &action, NULL) == -1)
		return (-1);
	if (pid == 0 || kill(pid, 0) == -1)
		return (ft_printf("Invalid PID\n"));
	while (1)
	{
		pause();
	}
	return (0);
}