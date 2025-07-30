/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labia-fe <labia-fe@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 12:05:55 by labia-fe          #+#    #+#             */
/*   Updated: 2025/07/22 15:19:16 by labia-fe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_args_2(char **argv)
{
	size_t	i;

	i = 1;
	if (ft_atoi(argv[1]) > 200)
		return (1);
	while (argv[i])
	{
		if (ft_atoi(argv[i]) <= 0)
			return (1);
		i++;
	}
	return (0);
}

int	check_args(char **argv)
{
	size_t	i;
	size_t	j;

	j = 1;
	while (argv[j])
	{
		i = 0;
		while (argv[j][i])
		{
			if (!ft_isdigit(argv[j][i]))
				return (1);
			i++;
		}
		j++;
	}
	return (check_args_2(argv));
}

bool	check_deaths(t_data *data)
{
	int		i;
	long	last_meal;

	i = 0;
	while (i < data->philo_n)
	{
		pthread_mutex_lock(&data->philo_list[i]->meal_check);
		last_meal = get_time(data->start_time) - data->philo_list[i]->last_meal;
		pthread_mutex_unlock(&data->philo_list[i]->meal_check);
		if (last_meal > data->death_t)
		{
			pthread_mutex_lock(&data->sim_check);
			data->sim_end = true;
			pthread_mutex_unlock(&data->sim_check);
			pthread_mutex_lock(&data->print);
			printf(B_RED "%ld %d died\n" RESET, get_time(data->start_time),
				data->philo_list[i]->id);
			pthread_mutex_unlock(&data->print);
			return (true);
		}
		i++;
	}
	return (false);
}

bool	check_meals_done(t_data *data)
{
	int		i;
	bool	all_satisfied;

	if (data->meals_n <= 0)
		return (false);
	i = -1;
	all_satisfied = true;
	while (++i < data->philo_n)
	{
		pthread_mutex_lock(&data->philo_list[i]->meal_check);
		if (data->philo_list[i]->times_eaten < data->meals_n)
		{
			pthread_mutex_unlock(&data->philo_list[i]->meal_check);
			all_satisfied = false;
			break ;
		}
		pthread_mutex_unlock(&data->philo_list[i]->meal_check);
	}
	if (all_satisfied)
	{
		pthread_mutex_lock(&data->sim_check);
		data->sim_end = true;
		pthread_mutex_unlock(&data->sim_check);
	}
	return (all_satisfied);
}

void	check_on_philos(t_data *data)
{
	bool	sim_state;

	pthread_mutex_lock(&data->sim_check);
	sim_state = !data->sim_end;
	pthread_mutex_unlock(&data->sim_check);
	while (sim_state)
	{
		if (check_deaths(data))
			break ;
		if (check_meals_done(data))
		{
			pthread_mutex_lock(&data->print);
			printf(B_GREEN "[SIMULATION COMPLETE]\n" RESET);
			pthread_mutex_unlock(&data->print);
			break ;
		}
		usleep(500);
		pthread_mutex_lock(&data->sim_check);
		sim_state = !data->sim_end;
		pthread_mutex_unlock(&data->sim_check);
	}
}
