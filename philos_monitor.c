/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_monitor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 12:22:08 by kmooney           #+#    #+#             */
/*   Updated: 2023/08/17 18:57:02 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	ft_kill_all(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philo)
	{
		data->philo[i]->dead = 1;
		i++;
	}
	return ;
}

void	will_he_wakeup(t_philo *philo, int status)
{
	if (status == 0 && philo->t_die <= philo->t_sleep + philo->t_eat)
	{
		usleep (philo->t_die * 1000 - philo->t_eat * 1000);
		philo->current = get_time() - philo->start;
		printf("%lu philo %d slept before dying : %lu\n", philo->current, philo->id, philo->t_die - philo->t_eat);
		philo->dead = 1;
	}
	if (status == 1 && philo->t_die <= philo->t_eat)
	{
		usleep (philo->t_eat * 1000);
		
		philo->dead = 1;
	}
	return ;
}

void ft_die_verify(t_philo *philo)
{
	//philo->elapsed = get_time() - philo->start - philo->reset;
	
	// printf("philo %d Verify: %lu\n", philo->id, philo->elapsed);
	// printf("philo %d ft_die_verify says philo->elapsed is %lu\n", philo->id, philo->elapsed);
	// printf("philo %d ft_die_verify says philo->t_die is %lu\n", philo->id, philo->t_die);
	// printf("philo %d ft_die_verify says philo->reset is %lu\n", philo->id, philo->reset);
	philo->current = get_time() - philo->start;
	//if (get_time() - philo->start > philo->time_to_die)
	if ((get_time() - philo->start) > philo->death_after_eating)
	{
		// printf("ft_die_verify says philo->elapsed is %lu\n", philo->elapsed);
		// printf("ft_die_verify says philo->t_die is %lu\n", philo->t_die);
		// print_out(philo, "is dead");
		philo->dead = 1;
	}
	return ;
}

void	ft_monitor(t_data *data)
{
	int	i;
	
	i = 0;
	usleep(data->time_to_die * (1000 - 1));
	while (1)
	{
		i = 0;
		while (i < data->num_philo)
		{
			ft_die_verify(data->philo[i]);
			if (data->philo[i]->dead != 1)
				i++;
			else
			{
				ft_kill_all(data);
				data->philo[i]->current = get_time() - data->philo[i]->start;
				printf("%lu %d died\n", data->philo[i]->current, data->philo[i]->id);
				return ;
			}
		}
	}
	return ;
}
