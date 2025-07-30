/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labia-fe <labia-fe@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 11:16:43 by labia-fe          #+#    #+#             */
/*   Updated: 2025/07/30 20:21:03 by labia-fe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_isdigit(char c)
{
	if (c <= '9' && c >= '0')
		return (1);
	return (0);
}

int	ft_atoi(const char *c)
{
	int	a;
	int	i;
	int	sign;

	i = 0;
	a = 0;
	sign = 1;
	while ((c[i] >= 9 && c[i] <= 13) || c[i] == ' ')
	{
		i++;
	}
	if (c[i] == '-' || c[i] == '+')
	{
		if (c[i] == '-')
		{
			sign = -sign;
		}
		i++;
	}
	while (c[i] >= '0' && c[i] <= '9')
	{
		a = a * 10 + (c[i] - '0');
		i++;
	}
	return (a * sign);
}

//	Frees all the allocated memory for the program
void	free_data(t_data *data)
{
	int	i;

	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->sim_check);
	if (data->forks)
	{
		i = -1;
		while (++i < data->philo_n)
			pthread_mutex_destroy(&data->forks[i]);
		free(data->forks);
		data->forks = NULL;
	}
	if (data->philo_list)
	{
		i = -1;
		while (data->philo_list[++i])
		{
			pthread_mutex_destroy(&data->philo_list[i]->meal_check);
			free(data->philo_list[i]);
		}
		free(data->philo_list);
		data->philo_list = NULL;
	}
}

//	Tells the time in ms since the start of the simulation
//	RETURN: (long int) time if ok, -1 if error
long	get_time(long start)
{
	struct timeval	current;
	long			time;

	if (gettimeofday(&current, NULL) != 0)
		return (perror("gettimeofday"), -1);
	time = (current.tv_sec) * 1000 + (current.tv_usec) / 1000;
	return (time - start);
}

void	print_action(t_philo *philo, t_action action)
{
	long	time;
	bool	sim_state;

	pthread_mutex_lock(&philo->data->print);
	pthread_mutex_lock(&philo->data->sim_check);
	sim_state = philo->data->sim_end;
	pthread_mutex_unlock(&philo->data->sim_check);
	time = get_time(philo->data->start_time);
	if (time < 0)
	{
		pthread_mutex_unlock(&philo->data->print);
		return ;
	}
	if (!sim_state)
	{
		if (action == TAKE_FORK)
			printf(CYAN "%ld %d has taken a fork\n" RESET, time, philo->id);
		else if (action == THINK)
			printf(YELLOW "%ld %d is thinking\n" RESET, time, philo->id);
		else if (action == EAT)
			printf(MAGENTA "%ld %d is eating\n" RESET, time, philo->id);
		else if (action == SLEEP)
			printf(BLUE "%ld %d is sleeping\n" RESET, time, philo->id);
	}
	pthread_mutex_unlock(&philo->data->print);
}
