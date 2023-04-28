/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 10:37:08 by joaoteix          #+#    #+#             */
/*   Updated: 2023/04/28 15:50:02 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"
# include <stdio.h>
# include <errno.h>
# include <string.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <unistd.h>

# define ACCESS_BITS 420
# define FILE_FLAG	577
# define APPEND_ARG "here_doc"
# define HEREDOC_PROMPT "pipe heredoc>"

typedef struct s_pipecon
{
	int	pipe_i;
	int	pipe_n;
	int	(*pipes)[2];
}	t_pipecon;

void	ft_free_str_arr(char **arr);

char	**ft_parse_path(char *envp[]);

char	*ft_get_cmd_path(char *cmd, char **path);

int		ft_exec_cmd(t_pipecon *context, char *cmd_path, \
		char **argv, char *const envp[]);

int		ft_proc_cmd(t_pipecon *context, char *cmd_str, \
		char **path, char *const envp[]);

void	close_pipes(t_pipecon *context);

void	read_here_doc(int pipe_in, char *terminator);

#endif
