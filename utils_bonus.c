/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 10:40:38 by joaoteix          #+#    #+#             */
/*   Updated: 2023/03/18 19:22:50 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Free allocd string array
void	ft_free_str_arr(char **arr)
{
	char	**iter;

	iter = arr;
	while (*(iter))
		free(*(iter++));
	free(arr);
}

// Read PATH env var and return a string array of all entries
char	**ft_parse_path(char *envp[])
{
	while (envp && !ft_strnstr(*envp, "PATH", 4))
		envp++;
	if (!envp)
	{
		ft_putstr_fd("pipex: PATH not defined\n", 2);
		exit(EXIT_FAILURE);
	}
	return (ft_split(ft_strchr(*envp, '=') + 1, ':'));
}

// Determine command path string
char	*ft_get_cmd_path(char *cmd, char **path)
{
	char	*fullpath;

	cmd = ft_strjoin("/", cmd);
	while (*path)
	{
		fullpath = ft_strjoin(*(path++), cmd);
		if (!access(fullpath, X_OK))
		{
			free(cmd);
			return (fullpath);
		}
		free(fullpath);
	}	
	free(cmd);
	return (NULL);
}

// pipes: contains the previous (read) and next (write) pipes
int	ft_exec_cmd(t_pipecon *context, char *cmd_path, \
		char **argv, char *const envp[])
{
	pid_t	c_pid;

	c_pid = fork();
	if (c_pid == -1)
	{
		perror("fork");
		return (-1);
	}
	else if (c_pid == 0)
	{
		dup2(context->pipes[context->pipe_i][0], STDIN_FILENO);
		dup2(context->pipes[context->pipe_i + 1][1], STDOUT_FILENO);
		close_pipes(context);
		execve(cmd_path, argv, envp);
		perror("execve");
		return (-1);
	}
	else
		return (c_pid);
}

// Process each command: determine its path and execute it
int	ft_proc_cmd(t_pipecon *context, char *cmd_str, \
		char **path, char *const envp[])
{
	char	**cmd_tokens;
	char	*cmd_path;
	int		pid;

	pid = -1;
	cmd_tokens = ft_split(cmd_str, ' ');
	cmd_path = NULL;
	if (ft_strchr("./", *(cmd_tokens[0])))
		cmd_path = ft_strdup(cmd_tokens[0]);
	else
		cmd_path = ft_get_cmd_path(cmd_tokens[0], path);
	if (!cmd_path)
		ft_dprintf(2, "pipex: Command not found: %s\n", cmd_tokens[0]);
	else if (access(cmd_path, X_OK))
		ft_dprintf(2, "pipex: Permission denied : %s\n", cmd_tokens[0]);
	else
		pid = ft_exec_cmd(context, cmd_path, cmd_tokens, envp);
	free(cmd_path);
	ft_free_str_arr(cmd_tokens);
	return (pid);
}
