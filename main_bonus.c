/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 10:02:10 by joaoteix          #+#    #+#             */
/*   Updated: 2023/03/20 23:21:08 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex_bonus.h"
#include <stdlib.h>
#include <unistd.h>

// Close all pipes (should be called after using dup to copy necessary fds)
void	close_pipes(t_pipecon *context)
{
	int	i;

	if (context->append)
		close(context->pipes[0][1]);
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
int	gen_pipes(t_pipecon *context, char *argv[], int argc)
{
	int	i;

	context->pipes = malloc(sizeof(int [2]) * context->pipe_n);
	if (context->append)
	{
		pipe(context->pipes[0]);
		read_here_doc(context->pipes[0][1], argv[2]);
	}
	else
		context->pipes[0][0] = open(argv[1], O_RDONLY);
	if (context->pipes[0][0] == -1)
	{
		ft_dprintf(2, "pipex: %s: %s\n", strerror(errno), argv[1]);
		context->pipes[0][0] = open("/dev/null", O_RDONLY);
	}
	i = 1;
	while (i < (context->pipe_n - 1))
		pipe(context->pipes[i++]);
	if (context->append)
		context->pipes[i][1] = open(argv[argc - 1], FILE_APPEND, ACCESS_BITS);
	else
		context->pipes[i][1] = open(argv[argc - 1], FILE_TRUNC, ACCESS_BITS);
	if (context->pipes[i][1] == -1)
		ft_dprintf(2, "pipex: %s: %s\n", strerror(errno), argv[argc - 1]);
	return (context->pipes[i][1] != -1);
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
		pid = ft_proc_cmd(context, argv[i + 2 + context->append], path, envp);
		i++;
	}
	ft_free_str_arr(path);
	return (pid);
}

void	read_here_doc(int pipe_in, char *terminator)
{
	char	*line;
	int		size;

	size = ft_strlen(terminator);
	ft_putstr_fd(HEREDOC_PROMPT, STDOUT_FILENO);
	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		if (ft_strncmp(line, terminator, size) == 0 && line[size] == '\n')
		{
			free(line);
			return ;
		}
		ft_putstr_fd(line, pipe_in);
		free(line);
		ft_putstr_fd(HEREDOC_PROMPT, STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	int			wstatus;
	int			last_pid;
	t_pipecon	context;

	if (argc < 5)
		return (EXIT_FAILURE);
	context.append = ft_strncmp(argv[1], APPEND_ARG,
			ft_strlen(APPEND_ARG)) == 0;
	if (context.append && argc < 6)
		return (EXIT_FAILURE);
	context.pipe_n = (argc - 2 - context.append);
	if (!gen_pipes(&context, argv, argc))
	{
		free(context.pipes);
		return (EXIT_FAILURE);
	}
	last_pid = exec_pipe_chain(&context, argv, envp);
	close_pipes(&context);
	free(context.pipes);
	if (last_pid < 0)
		return (127);
	waitpid(last_pid, &wstatus, 0);
	return (WEXITSTATUS(wstatus));
}
