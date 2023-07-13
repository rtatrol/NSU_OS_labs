#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>

int glob = 0;

int main()
{
	int loc = 1;
	printf("parent: loc: val = %d, addr = %p     glob: val = %d, addr = %p\n", loc, &loc, glob, &glob);
	printf("parent: pid: %d\n", getpid());
	int pid = fork();
	if (pid == -1)
	{
		printf("failed");
		return 0;
	}
	else
	{
		if (pid == 0)
		{
			int childPid = getpid();
			printf("child: childpid: %d\n", childPid);
			int parentPid = getppid();
			printf("child: parent of pid=%d is pid=%d\n", childPid, parentPid);
			printf("child: loc: val = %d, addr = %p     glob: val = %d, addr = %p\n", loc, &loc, glob, &glob);
			loc = 3;
			glob = 2;
			printf("child: newloc: val = %d     newglob: val = %d\n", loc, glob);
			sleep(10);
			exit(5);
		}
		else
		{
			sleep(20);
			printf("parent: newloc: val = %d     newglob: val = %d\n", loc, glob);

			int status;
			
			wait(&status);

			if (WIFEXITED(status))
			{
				printf("parent: exited, status=%d\n", WEXITSTATUS(status));
			}
			else if (WIFSIGNALED(status))
			{
				printf("parent: killed by signal %d\n", WTERMSIG(status));
			}

			int es = WEXITSTATUS(status);
			printf("parent: status = %d\n", es);
		}
	}
}
