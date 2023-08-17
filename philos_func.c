/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_func.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 23:22:38 by kmooney           #+#    #+#             */
/*   Updated: 2023/08/17 19:47:24 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void *ft_routine(void *philo_void)
{
	t_philo *philo;
	int i = 0;
	
	philo = (t_philo *)philo_void;
	philo->death_after_eating = philo->start + philo->t_die;
	while (philo->dead == 0)
	{
		philo->hungry = 1;
		if (ft_eat(philo) == 1)
			break;
		else
		{
			print_out(philo, "is sleeping");
			//will_he_wakeup(philo, 0);
			if (philo->dead != 1)
			{
				usleep (philo->t_sleep * 1000);
				print_out(philo, "is thinking");
			}
			else
				break;
		}
	}
	return (NULL);
}

void	odd_eat(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->fork_right->mutex) == 0)
	{	   	
       	print_out(philo, "has taken a fork");
		pthread_mutex_lock(&philo->fork_left->mutex);
		print_out(philo, "has taken a fork");
		print_out(philo, "is eating");
		philo->current = get_time() - philo->start;
		philo->death_after_eating = philo->current + philo->t_die;
		//will_he_wakeup(philo, 1);
		//if (philo->dead == 1)
		//{
		//	pthread_mutex_unlock(philo->fork_left);
		//	pthread_mutex_unlock(philo->fork_right);
		//	return ;
		//}
		//philo->current = get_time() - philo->start;
		//philo->reset = philo->current;
		usleep(philo->t_eat * 1000);
		philo->hungry = 0;
		pthread_mutex_unlock(&philo->fork_right->mutex);
		pthread_mutex_unlock(&philo->fork_left->mutex);
	}
}

void	even_eat(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->fork_left->mutex) == 0)
	{     		
       	print_out(philo, "has taken a fork");
		pthread_mutex_lock(&philo->fork_right->mutex);
		print_out(philo, "has taken a fork");
		print_out(philo, "is eating");
		philo->current = get_time() - philo->start;
		philo->death_after_eating = philo->current + philo->t_die;
		//will_he_wakeup(philo, 1);
		//if (philo->dead == 1)
		//{
		//	pthread_mutex_unlock(philo->fork_left);
		//	pthread_mutex_unlock(philo->fork_right);
		//	return ;
		//}
		//philo->current = get_time() - philo->start;
		//philo->reset = philo->current;
		usleep(philo->t_eat * 1000);
		philo->hungry = 0;
		pthread_mutex_unlock(&philo->fork_left->mutex);
		pthread_mutex_unlock(&philo->fork_right->mutex);
	}
}

int	ft_eat(t_philo *philo)
{
	philo->current = get_time() - philo->start;
	while (philo->dead == 0 && philo->hungry == 1)  //&& philo->hungry == 1
	{
		if (philo->alone == 1)
		{
			print_out(philo, "has taken a fork");
			usleep(philo->t_eat * 1000);
			philo->dead = 1;
		}	
		else if (philo->id % 2 == 0)
			even_eat(philo);
		else if (philo->id % 2 == 1)               
			odd_eat(philo);
	}
	if (philo->dead == 1)
		return (1);
	else 
		return (0);
}

// int	ft_eat(t_philo *philo)
// {
// 	pthread_mutex_t	*dead_mutex;

// 	philo->current = get_time() - philo->start;
// 	while (philo->dead == 0 && philo->hungry == 1)
// 	{
// 		double_fork_method(philo);
// 		if (philo->dead == 1)
// 			return (1);
// 		ft_die_verify(philo);
// 		if (philo->hungry == 0)
// 			return (0);

// 	}
// 	if (philo->dead == 1)
// 		return (1);
// 	else 
// 		return (0);
// }

// void	double_fork_method(t_philo *philo)
// {                                                   
// 	if ((pthread_mutex_lock(philo->fork_left) == 0) && (pthread_mutex_lock(philo->fork_right) == 0))
// 	{
// 		philo->current = get_time() - philo->start;							     						
//        	print_out(philo, "has taken a fork");
// 		print_out(philo, "has taken a fork");
// 		print_out(philo, "is eating");
// 		will_he_wakeup(philo, 1);
// 		if (philo->dead == 1)
// 		{
// 			pthread_mutex_unlock(philo->fork_left);
// 			pthread_mutex_unlock(philo->fork_right);			
// 			return ;
// 		}
// 		philo->current = get_time() - philo->start;
// 		philo->reset = philo->current;
// 		usleep(philo->t_eat * 1000);
// 		philo->hungry = 0;
// 		pthread_mutex_unlock(philo->fork_left);
// 		pthread_mutex_unlock(philo->fork_right);
// 	}
// 	else
// 	{
// 		pthread_mutex_unlock(philo->fork_left);
// 		pthread_mutex_unlock(philo->fork_right);
// 	}
// }