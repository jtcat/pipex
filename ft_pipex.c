/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 20:46:48 by joaoteix          #+#    #+#             */
/*   Updated: 2022/12/29 09:14:15 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "utils.h"

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

char	*ft_get_cmd_path(char *cmd, char **path)
{
	char	*fullpath;

	cmd = ft_strjoin("/", cmd);
	fullpath = NULL;
	while (*path)
	{
		fullpath = ft_strjoin(*(path++), cmd);
		if (!access(fullpath, X_OK))
			break ;
		free(fullpath);
		fullpath = NULL;
	}	
	free(cmd);
	return (fullpath);
}

void	ft_free_str_arr(char **arr)
{
	char	**iter;

	iter = arr;
	while (*(iter))
		free(*(iter++));
	free(arr);
}

void	ft_exec_cmd(char *cmd_path, char **argv, char *const envp[])
{
	pid_t	c_pid;

	c_pid = fork();
	if (c_pid == -1)
		perror("fork");
	if (c_pid == 0)
	{
		execve(cmd_path, argv, envp);
		perror("execve");
	}
	else
		wait(&c_pid);
}

void	ft_proc_cmd(char *cmd_str, char **path, char *const envp[])
{
	char	**argv;
	char	*cmd_path;

	argv = ft_split(cmd_str, ' ');
	cmd_path = ft_get_cmd_path(argv[0], path);
	if (!cmd_path)
		ft_dprintf(2, "pipex: Command not found: %s\n", argv[0]);
	else
	{
		ft_exec_cmd(cmd_path, argv, envp);
		free(cmd_path);
	}
	ft_free_str_arr(argv);
}

int	main(int argc, char *argv[], char *envp[])
{
	char	**path;
	int		pipefd[2];
	int		fd;
	int		i;

	if (argc < 5)
		return (EXIT_FAILURE);
	path = ft_parse_path(envp);
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		ft_dprintf(2, "pipex: %s: %s\n", strerror(errno), argv[1]);
	pipe(pipefd);
	dup2(fd, STDIN_FILENO);
	dup2(pipefd[1], STDOUT_FILENO);
	ft_proc_cmd(argv[2], path, envp);
	close(fd);
	dup2(pipefd[0], STDIN_FILENO);
	i = 3;
	while (i < (argc - 2))
		ft_proc_cmd(argv[i++], path, envp);
	close(pipefd[1]);
	fd = open(argv[argc - 1], O_CREAT | O_WRONLY, O_FILE_MODE);
	if (fd == -1)
		ft_dprintf(2, "pipex: %s: %s\n", strerror(errno), argv[argc - 1]);
	dup2(fd, STDOUT_FILENO);
	ft_proc_cmd(argv[argc - 2], path, envp);
	close(pipefd[0]);
	close(fd);
	ft_free_str_arr(path);
	return (EXIT_SUCCESS);
}
