#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void)
{
	int	pipefd[2];
	pid_t	cpid;

	char *const	args[] = {"wc", "-l", NULL};
	pipe(pipefd);
	cpid = fork();
	if (cpid == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		execve("/bin/wc", args, NULL);
	} else {
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		printf("ola tudo bem\nvamos la trabalhar\nsiga!!!\n");
		close(STDOUT_FILENO);
		wait(NULL);
	}

	return (EXIT_FAILURE);
}
