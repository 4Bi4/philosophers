/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labia-fe <labia-fe@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 12:05:55 by labia-fe          #+#    #+#             */
/*   Updated: 2025/07/11 21:55:15 by labia-fe         ###   ########.fr       */
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
	int i;

	i = 0;
	while (i < data->philo_n)
	{
		long time_since_last_meal = get_time(data->start_time) - data->philo_list[i]->last_meal;
		if (time_since_last_meal > data->death_t)
		{
			print_action(data->philo_list[i], DIE);
			pthread_mutex_lock(&data->print);
			data->sim_end = true;
			pthread_mutex_unlock(&data->print);
			return (true);
		}
		i++;
	}
	return (false);
}

bool	check_meals_done(t_data *data)
{
	int i;
	bool all_satisfied;

	if (data->meals_n <= 0)
		return (false);
	i = 0;
	all_satisfied = true;
	while (i < data->philo_n)
	{
		if (data->philo_list[i]->times_eaten < data->meals_n)
		{
			all_satisfied = false;
			break;
		}
		i++;
	}
	if (all_satisfied)
		data->sim_end = true;
	return (all_satisfied);
}

void	check_on_philos(t_data *data)
{
	while (!data->sim_end)
	{
		if (check_deaths(data))
			break ;
		if (check_meals_done(data))
		{
			printf(B_GREEN "[SIMULATION COMPLETE]\n" RESET);	
			break ;
		}
		usleep(500);
	}
}
