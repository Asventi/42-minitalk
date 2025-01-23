/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pjarnac <pjarnac@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:13:43 by pjarnac           #+#    #+#             */
/*   Updated: 2025/01/23 15:13:43 by pjarnac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>

#include "libft.h"

int	g_ack;

void	handler(int sig)
{
	if (!g_ack)
	{
		g_ack = 1;
		return ;
	}
}

int	main(void)
{
	struct sigaction	action;

	ft_bzero(&action, sizeof (action));
	action.sa_handler = handler;
	if (sigaction(SIGUSR1, &action, NULL) == -1)
		return (-1);
	if (sigaction(SIGUSR2, &action, NULL) == -1)
		return (-1);
	g_ack = 0;
	ft_printf("Server PID: %d\n", getpid());
	while (1)
	{
		pause();
	}
}