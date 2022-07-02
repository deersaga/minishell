#include "../minishell.h"
#include <fcntl.h>

extern char **environ;
/*int	execute_commands(t_mshell *mshell, t_command *cmd)
{
	int		status;
	int		*pipefds;
	pid_t	pid;
	size_t	i;

	
	pipefds = init_pipe(mshell);
	i = 0;
	while (cmd)
	{
		setup_pipe(pipefds);
		pid = fork();
		if (pid == 0)
		{

		}
	}
}*/

int	check_builtin(t_mshell *mshell, t_command *cmd)
{
	char	*cmd_name;

	(void)mshell;
	cmd_name = get_cmd_name(cmd->token);
	//printf("command %s\n", cur->token);
	if (!ft_strcmp("cd", cmd_name) || !ft_strcmp("echo", cmd_name) || !ft_strcmp("pwd", cmd_name) \
	|| !ft_strcmp("export", cmd_name) || !ft_strcmp("unset", cmd_name) || !ft_strcmp("env", cmd_name) \
	|| !ft_strcmp("exit", cmd_name))
	{
		free(cmd_name);
		return (1);
	}
	free(cmd_name);
	return (0);
}

int	execute_a_builtin(t_mshell *mshell, t_command *cmd)
{
	char	*cmd_name;
	int		status;

	status = 1;
	cmd_name = get_cmd_name(cmd->token);
	if (!ft_strcmp(cmd_name, "cd"))
		status = ft_cd(mshell, cmd);
	else if (!ft_strcmp(cmd_name, "pwd"))
		status = ft_pwd(mshell, cmd);
	else if (!ft_strcmp(cmd_name, "echo"))
		status = ft_echo(mshell, cmd);
	else if (!ft_strcmp(cmd_name, "export"))
		status = ft_export(mshell, cmd);
	else if (!ft_strcmp(cmd_name, "unset"))
		status = ft_unset(mshell, cmd);
	else if (!ft_strcmp(cmd_name, "env"))
		status = ft_env(mshell, cmd);
	else if (!ft_strcmp(cmd_name, "exit"))
		status = ft_exit(mshell, cmd);
	free(cmd_name);
	return (status);
}

int	execute_a_add_on(t_mshell *mshell, t_command *cmd)
{
	pid_t	pid;
	char	*path;
	char	**env;
	//int		fd;

	pid = fork();
	if (pid == 0)
	{
		//signal(SIGINT, SIG_DFL);
		//signal(SIGQUIT, SIG_DFL);
		/*fd = open("out", O_RDWR | O_CREAT | O_TRUNC);
		dup2(fd, 1);
		close(fd);*/
		create_argv(mshell, cmd);
		path = get_cmd_path(mshell, cmd->argv[0]);
		//printf("path %s\n",path);
		if (path)
		{
			free(cmd->argv[0]);
			cmd->argv[0] = path;
		}
		else
			exit(127);
		//print_array(cmd->argv);
		env = make_environ(mshell);
		//print_array(env);
		execve(cmd->argv[0], cmd->argv, env);
		free_array(env);
		free_commands(mshell->commands);
		exit(1);
	}
	return (0);
}

int	execute_a_builtin_command(t_mshell *mshell, t_command *cmd)
{
	int		status;
	int		dup_stdio_fd[2];

	status = 0;
	dup_stdio_fd[0] = dup(0);
	dup_stdio_fd[1] = dup(1);
	reconnect_redir_with_stdio(mshell, cmd, 0, NULL);
	status = execute_a_builtin(mshell, cmd);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	dup2(dup_stdio_fd[0], STDIN_FILENO);
	dup2(dup_stdio_fd[1], STDOUT_FILENO);
	close(dup_stdio_fd[0]);
	close(dup_stdio_fd[1]);
	return (status);
}

int	____past__execute_a_command(t_mshell *mshell, t_command *cmd)
{
	int		status;
	//int		pipe[2];
	//int		fd[2];

	status = 0;

	
	//dup2(fd[1],1);
	if (ft_strcmp(get_first_non_delimiter_token(cmd->token)->token, "export"))
		cmd->token = expand_and_retokenize(mshell, cmd->token);
	if (check_builtin(mshell, cmd))
	{
		/*fd[0] = dup(1);
		close(1);
		fd[1] = open("out", O_RDWR | O_CREAT | O_TRUNC);*/
		status = execute_a_builtin(mshell, cmd);
		/*close(fd[1]);
		dup2(fd[0], STDOUT_FILENO);
		close(fd[0]);*/
		return (status);
	}
	else
	{
		//redir_set(mshell, cmd);
		execute_a_add_on(mshell, cmd);
		//exit(127);
	}
	while (wait(&status) >= 0);
	//printf("status %d\n", status / 256);
	mshell->exit_status = WEXITSTATUS(status);
	//printf("status %d\n", status);
	return (mshell->exit_status);
}


/*int    main(int argc, char ** argv, char **envp)
{
    int        pipefd[2];
    pid_t    child1_pid;
    pid_t    child2_pid;

    if (pipe(pipefd) == -1)
        exit(1 || printf("pipe error\n"));
    //子1
    child1_pid = fork();
    if(child1_pid==0)
    {
        char    command[] = "/bin/cat";
        char    *exargv[] = {"/bin/cat", NULL};
        
        close(pipefd[0]);
        //int input_fd = open("input.txt", O_RDWR);
        int output_fd = pipefd[1];
        //dup2(input_fd, STDIN_FILENO);
        dup2(output_fd, STDOUT_FILENO);
        //close(input_fd);
        close(output_fd);
        
        execve(command, exargv, envp);
    }else{
        //子2
        child2_pid = fork();
        if(child2_pid==0)
        {
            char    command[] = "/bin/ls";
            char    *exargv[] = {"/bin/ls", NULL,  NULL};

            close(pipefd[1]);
            int input_fd = pipefd[0];
            //int output_fd = open("output.txt", O_RDWR);
            dup2(input_fd, STDIN_FILENO);
            //dup2(output_fd, STDOUT_FILENO);
            close(input_fd);
            //close(output_fd);    
            execve(command, exargv, envp);
        }
        else
        {    
                        //おや
			close(pipefd[0]);
            close(pipefd[1]);
        }
    }
		    waitpid(child1_pid, NULL, 0);
            waitpid(child2_pid, NULL, 0);
    return 0;
}*/

