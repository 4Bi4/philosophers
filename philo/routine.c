/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labia-fe <labia-fe@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:49:36 by labia-fe          #+#    #+#             */
/*   Updated: 2025/08/04 20:22:56 by labia-fe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	eat(t_philo *philo, pthread_mutex_t *first_fork,
		pthread_mutex_t *second_fork)
{
	pthread_mutex_lock(&philo->meal_check);
	if (philo->data->meals_n > 0 && philo->times_eaten >= philo->data->meals_n)
	{
		pthread_mutex_unlock(&philo->meal_check);
		pthread_mutex_unlock(second_fork);
		pthread_mutex_unlock(first_fork);
		return (1);
	}
	philo->last_meal = get_time(philo->data->start_time);
	pthread_mutex_unlock(&philo->meal_check);
	print_action(philo, EAT);
	usleep(philo->data->eat_t * 1000);
	pthread_mutex_lock(&philo->meal_check);
	philo->times_eaten++;
	pthread_mutex_unlock(&philo->meal_check);
	pthread_mutex_unlock(second_fork);
	pthread_mutex_unlock(first_fork);
	return (0);
}

static int	take_forks_and_eat(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	first_fork = philo->fork_l;
	second_fork = philo->fork_r;
	pthread_mutex_lock(first_fork);
	print_action(philo, TAKE_FORK);
	pthread_mutex_lock(second_fork);
	print_action(philo, TAKE_FORK);
	return (eat(philo, first_fork, second_fork));
}

static int	routine(t_philo *philo)
{
	bool	sim_state;

	pthread_mutex_lock(&philo->data->sim_check);
	sim_state = philo->data->sim_end;
	pthread_mutex_unlock(&philo->data->sim_check);
	if (sim_state)
		return (1);
	print_action(philo, THINK);
	if (take_forks_and_eat(philo) != 0)
		return (1);
	pthread_mutex_lock(&philo->data->sim_check);
	sim_state = philo->data->sim_end;
	pthread_mutex_unlock(&philo->data->sim_check);
	if (sim_state)
		return (1);
	print_action(philo, SLEEP);
	usleep(philo->data->sleep_t * 1000);
	return (0);
}

void	*philo_loop(void *arg)
{
	t_philo	*philo;
	bool	sim_state;

	philo = (t_philo *)arg;
	if (philo->data->philo_n < 2)
	{
		pthread_mutex_lock(philo->fork_l);
		print_action(philo, TAKE_FORK);
		while (true)
		{
			pthread_mutex_lock(&philo->data->sim_check);
			sim_state = philo->data->sim_end;
			pthread_mutex_unlock(&philo->data->sim_check);
			if (sim_state)
				break ;
			usleep(500);
		}
		pthread_mutex_unlock(philo->fork_l);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		usleep(philo->data->eat_t / 2 * 1000);
	while (routine(philo) == 0)
		;
	return (NULL);
}

int	start_simulation(t_data *data)
{
	int	i;

	data->sim_end = false;
	data->start_time = get_time(0);
	i = 0;
	while (i < data->philo_n)
	{
		if (pthread_create(&data->philo_list[i]->thread, NULL, philo_loop,
				data->philo_list[i]) != 0)
			return (perror("Error creating philosopher thread"), 1);
		i++;
	}
	return (0);
}
