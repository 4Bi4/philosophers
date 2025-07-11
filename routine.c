/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labia-fe <labia-fe@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:49:36 by labia-fe          #+#    #+#             */
/*   Updated: 2025/07/11 21:48:07 by labia-fe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	routine(t_philo *philo)
{
	print_action(philo, THINK);
	pthread_mutex_lock(philo->fork_L);
	print_action(philo, TAKE_FORK);
	pthread_mutex_lock(philo->fork_R);
	print_action(philo, TAKE_FORK);
	philo->last_meal = get_time(philo->data->start_time);
	if (philo->data->sim_end && philo->data->meals_n > 0 && philo->times_eaten >= philo->data->meals_n)
	{
		pthread_mutex_unlock(philo->fork_L);
		pthread_mutex_unlock(philo->fork_R);
		return (1);
	}
	print_action(philo, EAT);
	usleep(philo->data->eat_t * 1000);
	philo->times_eaten++;
	pthread_mutex_unlock(philo->fork_L);
	pthread_mutex_unlock(philo->fork_R);
	if (philo->data->sim_end)
		return (1);
	print_action(philo, SLEEP);
	usleep(philo->data->sleep_t * 1000);
	return (0);
}

void	*philo_loop(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->philo_n < 2)
	{
		pthread_mutex_lock(philo->fork_L);
		print_action(philo, TAKE_FORK);
		while (!philo->data->sim_end)
			usleep(1000);
		pthread_mutex_unlock(philo->fork_L);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		usleep(philo->data->eat_t * 1000 / 2);
	while (!philo->data->sim_end)
	{
		if (routine(philo) != 0)
			break;
	}
	return (NULL);
}

int	start_simulation(t_data *data)
{
	int	i;
	
	data->start_time = get_time(0);
	if (data->start_time < 0)
		return (perror("gettimeofday"), 1);
	data->sim_end = false;
	i = 0;
	while (i < data->philo_n)
	{
		if (pthread_create(&data->philo_list[i]->thread, NULL, philo_loop, data->philo_list[i]) != 0)
			return (perror("Error creating philosopher thread"), 1);
		i++;
	}
	return (0);
}

void	wait_sim_end(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_n)
	{
		pthread_join(data->philo_list[i]->thread, NULL);
		i++;
	}
}

void	join_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_n)
	{
		pthread_join(data->philo_list[i]->thread, NULL);
		i++;
	}
}
