/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labia-fe <labia-fe@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 20:05:05 by labia-fe          #+#    #+#             */
/*   Updated: 2025/07/30 20:18:01 by labia-fe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

//	COLOR DEFINITIONS

# define RED "\001\033[0;31m\002"
# define GREEN "\001\033[0;32m\002"
# define YELLOW "\001\033[0;33m\002"
# define BLUE "\001\033[0;34m\002"
# define MAGENTA "\001\033[0;35m\002"
# define CYAN "\001\033[0;36m\002"

# define B_RED "\001\033[1;31m\002"
# define B_GREEN "\001\033[1;32m\002"
# define B_YELLOW "\001\033[1;33m\002"
# define B_BLUE "\001\033[1;34m\002"
# define B_MAGENTA "\001\033[1;35m\002"
# define B_CYAN "\001\033[1;36m\002"

# define RESET "\001\033[0m\002"

# define ERROR_1 "USAGE:\n(number of philosophers)\n"
# define ERROR_2 "(time to die)\n(time to eat)\n"
# define ERROR_3 "(time to sleep)\n(number of meals) OPTIONAL\n"

typedef struct s_data	t_data;

typedef struct s_philo
{
	short				id;
	pthread_t			thread;
	pthread_mutex_t		*fork_l;
	pthread_mutex_t		*fork_r;
	pthread_mutex_t		meal_check;
	long				last_meal;
	short				times_eaten;
	t_data				*data;
}						t_philo;

typedef struct s_data
{
	int					philo_n;
	long				death_t;
	long				eat_t;
	long				sleep_t;
	int					meals_n;

	t_philo				**philo_list;
	pthread_mutex_t		*forks;
	pthread_mutex_t		print;
	pthread_mutex_t		sim_check;

	long				start_time;
	bool				sim_end;
}						t_data;

typedef enum e_action
{
	TAKE_FORK,
	THINK,
	EAT,
	SLEEP,
	DIE
}						t_action;

void					*philo_loop(void *arg);
void					wait_sim_end(t_data *data);
int						start_simulation(t_data *data);
void					join_philosophers(t_data *data);

long					get_time(long start);
void					print_action(t_philo *philo, t_action action);

int						check_args(char **argv);
void					check_on_philos(t_data *data);

int						ft_isdigit(char c);
int						ft_atoi(const char *c);

void					free_data(t_data *data);

#endif