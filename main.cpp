#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <err.h>

int pipefd[2];

void onexit(int signal)
{
	// send stop message to server
	if (write(pipefd[1], "stop\r\n", 6) < 0) {
		err(1, "Failed to send stop signal to child");
	}
}

int main(int argc, char **argv)
{
	pid_t pid;
	int exit_code;
	signal(SIGINT, onexit);
	signal(SIGTERM, onexit);

	// create pipe to allow communication with server
	if (pipe(pipefd) < 0) {
		err(1, "Failed to open IPC");
		return 1;
	}
	// make child process
	pid = fork();
	if (pid == -1) {
		// fork failed
		err(1, "Failed to fork new process");
		return 1;
	} else if (pid == 0) {
		// inside child process
		// create new process group to prevent passing signals
		setpgid(0, 0);
		// connect input of child process
		dup2(pipefd[0], STDIN_FILENO);
		// start child process
		execve(argv[1], argv+1, environ);
		err(1, "Starting process failed");
		return 1;
	} else {
		// inside parent process
		wait(&exit_code);
		return exit_code;
	}
}
