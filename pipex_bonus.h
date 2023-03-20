/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoteix <joaoteix@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 22:40:55 by joaoteix          #+#    #+#             */
/*   Updated: 2023/03/20 23:20:38 by joaoteix         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

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
# define FILE_APPEND 1089
# define FILE_TRUNC	577
# define APPEND_ARG "here_doc"
# define HEREDOC_PROMPT "pipe heredoc>"

typedef struct s_pipecon
{
	int	append;
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
