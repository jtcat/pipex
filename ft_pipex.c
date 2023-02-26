/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 10:02:10 by joaoteix          #+#    #+#             */
/*   Updated: 2023/02/26 14:50:01 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "utils.h"
#include <stdlib.h>
#include <unistd.h>

// Close all pipes (should be called after using dup to copy necessary fds)
void	close_pipes(t_pipecon *context)
{
	int	i;

	close(context->pipes[0][0]);
	i = 1;
	while (i < (context->pipe_n - 1))
	{
		close(context->pipes[i][0]);
		close(context->pipes[i][1]);
		i++;
	}
	close(context->pipes[context->pipe_n - 1][1]);
}

// Create pipes
int	gen_pipes(t_pipecon *context, char *argv[], int argc, int append)
{
	int	i;

	context->pipe_n = (argc - 2);
	context->pipes = malloc(sizeof(int [2]) * context->pipe_n);
	context->pipes[0][0] = open(argv[1], O_RDONLY | append);
	if (context->pipes[0][0] == -1)
		ft_dprintf(2, "pipex: %s: %s\n", strerror(errno), argv[1]);
	i = 1;
	while (i < (context->pipe_n - 1))
		pipe(context->pipes[i++]);
	context->pipes[i][1] = open(argv[argc - 1], FILE_FLAG | append, ACCESS_BITS);
	if (context->pipes[i][1] == -1)
		ft_dprintf(2, "pipex: %s: %s\n", strerror(errno), argv[argc - 1]);
	return ((context->pipes[0][0] | context->pipes[i][1]) != -1);
}

int	exec_pipe_chain(t_pipecon *context, char *argv[], char *envp[])
{
	int			i;
	int			pid;
	char		**path;

	path = ft_parse_path(envp);
	i = 0;
	while (i < (context->pipe_n - 1))
	{
		context->pipe_i = i;
		pid = ft_proc_cmd(context, argv[i], path, envp);
		i++;
	}
	close_pipes(context);
	ft_free_str_arr(path);
	return (pid);
}

int	main(int argc, char *argv[], char *envp[])
{
	int			wstatus;
	int			last_pid;
	int			append_flag;
	t_pipecon	context;

	append_flag = 0;
	if (argc < 5)
		return (EXIT_FAILURE);
	if (ft_strncmp(argv[1], APPEND_ARG, sizeof(APPEND_ARG)) == 0)
	{
		if (argc < 6)
			return (EXIT_FAILURE);
		append_flag = O_APPEND;
	}
	if (!gen_pipes(&context, argv, argc, append_flag))
		return (EXIT_FAILURE);
	last_pid = exec_pipe_chain(&context, argv + 2, envp);
	free(context.pipes);
	if (last_pid < 0)
		return (EXIT_FAILURE);
	waitpid(last_pid, &wstatus, 0);
	return (WEXITSTATUS(wstatus));
}
