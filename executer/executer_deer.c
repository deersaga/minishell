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
	t_token	*cur;

	(void)mshell;
	cur = skip_delimiter_token(cmd->token);
	//printf("command %s\n", cur->token);
	if (!ft_strcmp("cd", cur->token) || !ft_strcmp("echo", cur->token) || !ft_strcmp("pwd", cur->token) \
	|| !ft_strcmp("export", cur->token) || !ft_strcmp("unset", cur->token) || !ft_strcmp("env", cur->token) \
	|| !ft_strcmp("exit", cur->token))
		return (1);
	return (0);
}

int	execute_a_builtin(t_mshell *mshell, t_command *cmd)
{
	char	*command;

	command = (skip_delimiter_token(cmd->token))->token;
	if (!ft_strcmp(command, "cd"))
		return (ft_cd(mshell, cmd));
	else if (!ft_strcmp(command, "pwd"))
		return (ft_pwd(mshell, cmd));
	else if (!ft_strcmp(command, "echo"))
		return (ft_echo(mshell, cmd));
	else if (!ft_strcmp(command, "export"))
		return (ft_export(mshell, cmd));
	else if (!ft_strcmp(command, "unset"))
		return (ft_unset(mshell, cmd));
	else if (!ft_strcmp(command, "env"))
		return (ft_env(mshell, cmd));
	else if (!ft_strcmp(command, "exit"))
		return (ft_exit(mshell, cmd));
	return (1);
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
	}
	return (0);
}

int	execute_a_command(t_mshell *mshell, t_command *cmd)
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

