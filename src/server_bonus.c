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
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdio.h>

#include "libft.h"

static void	end_session(pid_t pid, uint32_t *len, uint32_t *bit_r,
	uint8_t **data)
{
	uint32_t	i;

	i = 0;
	while (i < *len)
	{
		write(1, &(*data)[i], 1);
		i++;
	}
	write(1, "\n", 1);
	*len = 0;
	*bit_r = 0;
	free(*data);
	*data = 0;
	usleep(1);
	kill(pid, SIGUSR2);
}

static void	create_data(uint32_t len, uint8_t **data)
{
	*data = malloc(len);
	if (!*data)
		exit(EXIT_FAILURE);
	ft_bzero(*data, len);
}

static void	handler(int sig, siginfo_t *info, void *p)
{
	static uint32_t	len;
	static uint32_t	bit_r;
	static uint8_t	*data;

	(void)p;
	bit_r++;
	if (bit_r <= 32)
	{
		len |= (sig == SIGUSR2);
		if (bit_r < 32)
			len <<= 1;
	}
	if (bit_r == 33)
		create_data(len, &data);
	if (data && bit_r <= len * 8 + 32)
	{
		data[(bit_r - 33) / 8] |= (sig == SIGUSR2);
		if ((bit_r - 32) % 8 != 0)
			data[(bit_r - 33) / 8] <<= 1;
	}
	usleep(1);
	kill(info->si_pid, SIGUSR1);
	if (bit_r == len * 8 + 32)
		end_session(info->si_pid, &len, &bit_r, &data);
}

int	main(void)
{
	struct sigaction	action;

	ft_bzero(&action, sizeof (action));
	action.sa_sigaction = handler;
	action.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &action, NULL) == -1)
		return (-1);
	if (sigaction(SIGUSR2, &action, NULL) == -1)
		return (-1);
	ft_printf("Server PID: %d\n", getpid());
	while (1)
	{
		pause();
	}
}
