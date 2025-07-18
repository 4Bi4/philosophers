/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labia-fe <labia-fe@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 20:46:41 by labia-fe          #+#    #+#             */
/*   Updated: 2025/07/18 16:48:44 by labia-fe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_philo_data(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_n)
	{
		data->philo_list[i]->id = i + 1;
		data->philo_list[i]->fork_l = &data->forks[i];
		data->philo_list[i]->fork_r = &data->forks[(i + 1) % data->philo_n];
		data->philo_list[i]->last_meal = 0;
		data->philo_list[i]->times_eaten = 0;
		data->philo_list[i]->thread = 0;
		data->philo_list[i]->data = data;
		i++;
	}
	return (0);
}

int	init_philos(t_data *data)
{
	int	i;

	data->philo_list = malloc(sizeof(t_philo *) * (data->philo_n + 1));
	if (!data->philo_list)
		return (free(data->forks), 1);
	i = 0;
	while (i < data->philo_n)
	{
		data->philo_list[i] = malloc(sizeof(t_philo));
		if (!data->philo_list[i])
		{
			while (i > 0)
			{
				i--;
				free(data->philo_list[i]);
			}
			free(data->philo_list);
			return (free(data->forks), 1);
		}
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	data->philo_list[i] = NULL;
	return (init_philo_data(data));
}

int	init_data(char **argv, t_data *data)
{
	data->philo_n = ft_atoi(argv[1]);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->philo_n);
	if (!data->forks)
		return (1);
	data->sim_end = false;
	data->death_t = ft_atoi(argv[2]);
	data->eat_t = ft_atoi(argv[3]);
	data->sleep_t = ft_atoi(argv[4]);
	data->meals_n = -1;
	if (argv[5])
		data->meals_n = ft_atoi(argv[5]);
	data->philo_list = NULL;
	pthread_mutex_init(&data->print, NULL);
	return (init_philos(data));
}

int	main(int argc, char **argv)
{
	t_data	data;

	printf("Sorpresa! %lu\n", sizeof(short));
	if (argc != 5 && argc != 6)
		return (printf("%s%s%s", ERROR_1, ERROR_2, ERROR_3), 2);
	if (check_args(argv) != 0)
		return (printf(B_RED "[ERROR] bad input (invalid values)\n" RESET), 2);
	if (init_data(argv, &data) != 0)
		return (printf(B_RED "[ERROR] internal error\n" RESET),
			free_data(&data), 1);
	if (start_simulation(&data) != 0)
		return (printf(B_RED "[ERROR] failed to start simulation :(\n" RESET),
			free_data(&data), 1);
	check_on_philos(&data);
	wait_sim_end(&data);
	return (free_data(&data), 0);
}
