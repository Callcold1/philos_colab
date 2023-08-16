/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_func.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmooney <kmooney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 23:22:38 by kmooney           #+#    #+#             */
/*   Updated: 2023/08/16 22:53:51 by kmooney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	will_he_wakeup(t_philo *philo, int status)
{
	if (status == 0 && philo->t_die <= philo->t_sleep + philo->t_eat)
	{
		usleep (philo->t_die * 1000 - philo->t_eat * 1000);
		philo->current = get_time() - philo->start;
		//printf("%lu philo %d slept before dying : %lu\n", philo->current, philo->id, philo->t_die - philo->t_eat);
		philo->dead = 1;
	}
	if (status == 1 && philo->t_die <= philo->t_eat)
	{
		usleep (philo->t_eat * 1000);
		philo->dead = 1;
	}
	return ;
}

void *ft_routine(void *philo_void)
{
	t_philo *philo;
	int i = 0;
	
	philo = (t_philo *)philo_void;
	while (philo->dead == 0)
	{
		philo->hungry = 1;
		// will_he_wakeup(t_philo *philo)
		if (ft_eat(philo) == 1)
			break;
		else
		{
			print_out(philo, "is sleeping");
			// printf("%lu Philo %d is sleeping\n",philo->current, philo->id);
			will_he_wakeup(philo, 0);
			if (philo->dead != 1)
			{
				usleep (philo->t_sleep * 1000);
				print_out(philo, "is thinking");
				// printf("%lu Philo %d is thinking\n",philo->current, philo->id);
			}
			else
				break;
		}
	}
	return (NULL);
}

int	print_out(t_philo *philo, char *msg)
{
	if (philo->dead != 1)
	{	
		pthread_mutex_lock(*philo->dead_mutex);
		philo->current = get_time() - philo->start;							     							//first will have a fork free on the other side. Maybe only take the other if the already have one. If even numbers put the right fork
		printf("%lu %d %s\n", philo->current, philo->id, msg);
		pthread_mutex_unlock(*philo->dead_mutex);
		return (0);
	}
	else
		return (1);
}

void	odd_eat(t_philo *philo)
{
	if (pthread_mutex_lock(philo->fork_right) == 0)  							
	{	   	
		philo->current = get_time() - philo->start;
       	print_out(philo, "has taken a fork");
		pthread_mutex_lock(philo->fork_left);
		print_out(philo, "has taken a fork");
		print_out(philo, "is eating");
		// printf("%lu Philo %d has taken a fork 2\n",philo->current, philo->id);
		// printf("%lu Philo %d is eating\n",philo->current, philo->id);
		will_he_wakeup(philo, 1);
		if (philo->dead == 1)
		{
			pthread_mutex_unlock(philo->fork_left);
			pthread_mutex_unlock(philo->fork_right);			
			return ;
		}
		usleep(philo->t_eat * 1000);
		philo->current = get_time() - philo->start;
		philo->reset = philo->current;
		// print_out(philo, "has finished eating");
		// printf("%lu Philo %d has finished eating\n",philo->current, philo->id);
		philo->hungry = 0;
		pthread_mutex_unlock(philo->fork_right);
		pthread_mutex_unlock(philo->fork_left);
	}
}

//maybe worth checking if the philosopher is dead before trying to grab any forks

void	even_eat(t_philo *philo)
{                                               
	if (pthread_mutex_lock(philo->fork_left) == 0)
	{
		philo->current = get_time() - philo->start;							     							//first will have a fork free on the other side. Maybe only take the other if the already have one. If even numbers put the right fork
       	print_out(philo, "has taken a fork");
		pthread_mutex_lock(philo->fork_right);
		print_out(philo, "has taken a fork");
		print_out(philo, "is eating");
		will_he_wakeup(philo, 1);
		if (philo->dead == 1)
		{
			pthread_mutex_unlock(philo->fork_left);
			pthread_mutex_unlock(philo->fork_right);			
			return ;
		}
		philo->current = get_time() - philo->start;
		philo->reset = philo->current;
		usleep(philo->t_eat * 1000);
		philo->hungry = 0;
		pthread_mutex_unlock(philo->fork_left);
		pthread_mutex_unlock(philo->fork_right);
	}
}


// void	double_fork_method(t_philo *philo)
// {                                                   
// 	if ((pthread_mutex_lock(philo->fork_left) == 0) && (pthread_mutex_lock(philo->fork_right) == 0))
// 	{
// 		philo->current = get_time() - philo->start;							     							//first will have a fork free on the other side. Maybe only take the other if the already have one. If even numbers put the right fork
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

int	ft_eat(t_philo *philo)
{
	pthread_mutex_t	*dead_mutex;

	philo->current = get_time() - philo->start;
	while (philo->dead == 0 && philo->hungry == 1)			//need to add this to struct - once fed they should exit the function and return to routine
	{
		if (philo->id % 2 == 0)
			even_eat(philo);
		else if (philo->id % 2 == 1)                        
			odd_eat(philo);
		if (philo->dead == 1)
			return (1);
		if (philo->hungry == 0)
			return (0);
		ft_die_verify(philo);
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
// 	while (philo->dead == 0 && philo->hungry == 1)			//need to add this to struct - once fed they should exit the function and return to routine
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

void ft_die_verify(t_philo *philo)
{
	philo->elapsed = get_time() - philo->start - philo->reset;
	philo->current = get_time() - philo->start;
	printf("philo %d Verify: %lu\n", philo->id, philo->elapsed);
	printf("philo %d ft_die_verify says philo->elapsed is %lu\n", philo->id, philo->elapsed);
	printf("philo %d ft_die_verify says philo->t_die is %lu\n", philo->id, philo->t_die);
	printf("philo %d ft_die_verify says philo->reset is %lu\n", philo->id, philo->reset);
	if(philo->elapsed > philo->t_die)
	{
		// printf("ft_die_verify says philo->elapsed is %lu\n", philo->elapsed);
		// printf("ft_die_verify says philo->t_die is %lu\n", philo->t_die);
		
		// print_out(philo, "is dead");
		philo->dead = 1;
	}
	return ;
}