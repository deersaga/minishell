#include "../minishell.h"

void	print_redir(t_command *cmd)
{
	t_redir *cur;

	cur = cmd->redir_in;
	printf("redir_in\n");
	while (cur)
	{
		printf("file %s fd %d type %d\n", cur->file, cur->fd, cur->type);
		cur = cur->next;
	}
	cur = cmd->redir_out;
	printf("redir_out\n");
	while (cur)
	{
		printf("file %s fd %d type %d\n", cur->file, cur->fd, cur->type);
		cur = cur->next;
	}
}

void	print_commands(t_mshell *mshell)
{
	t_command	*cur;

	cur = mshell->commands;
	while (cur->token)
	{
		printf("----------------------------\n");
		print_tokens(cur->token);
		print_redir(cur);
		cur = cur->next;
	}
}

void	free_redir(t_redir *redir)
{
	t_redir *tmp;

	while(redir)
	{
		tmp = redir->next;
		free(redir->file);
		free(redir);
		redir = tmp;
	}
	free(redir);
}

void	free_command(t_command *cmd)
{
	free_redir(cmd->redir_in);
	free_redir(cmd->redir_out);
	free_all_token(cmd->token);
	free(cmd);
}

void	free_commands(t_command *cmd)
{
	t_command *tmp;

	while (cmd)
	{
		tmp = cmd->next;
		free_command(cmd);
		cmd = tmp;
	}
	free(cmd);
}

t_token	*terminate_command(t_mshell *mshell, t_token *cur, t_command **cmd)
{
	t_token	*next;

	(void)mshell;
	next = cur->next;
	cur->type = T_END;
	free(cur->token);
	cur->token = NULL;
	cur->next = NULL;
	(*cmd)->next = ft_calloc(1, sizeof(t_command));
	*cmd = (*cmd)->next;
	mshell->num_commands += 1;
	return (next);
}

t_redir	*get_redir_last(t_command *cmd, e_type_token type)
{
	t_redir *last;

	if (type == T_REDIR_IN || type == T_HEREDOC)
	{
		if (!cmd->redir_in)
			cmd->redir_in = ft_calloc(1, sizeof(t_redir));
		last = cmd->redir_in;
	}
	else
	{
		if (!cmd->redir_out)
			cmd->redir_out = ft_calloc(1, sizeof(t_redir));
		last = cmd->redir_out;
	}
	while (last->next)
		last = last->next;
	return (last);
}

int	get_fd(t_token *op)
{
	if (op->fd != 0)
		return (op->fd);
	else if (op->type == T_REDIR_IN)
		return (STDIN_FILENO);
	else if (op->type == T_REDIR_OUT)
		return (STDOUT_FILENO);
	else if (op->type == T_APPEND)
		return (STDOUT_FILENO);
	else if (op->type == T_HEREDOC)
		return (STDIN_FILENO);
	return (-1);
}

void	new_redir(t_command *cmd, t_token *op, e_type_token type)
{
	t_redir *last;
	t_token	*file;

	file = op->next;
	last = get_redir_last(cmd, type);
	last->type = type;
	last->file = ft_strdup(file->token);
	last->fd = get_fd(op);
	last->next = ft_calloc(1, sizeof(t_redir));
}

void	add_redir_info(t_command *cmd, t_token *cur)
{
	if (cur->type == T_REDIR_IN)
		new_redir(cmd, cur, cur->type);
	else if (cur->type == T_REDIR_OUT)
		new_redir(cmd, cur, cur->type);
	else if (cur->type == T_APPEND)
		new_redir(cmd, cur, cur->type);
	else if (cur->type == T_HEREDOC)
		new_redir(cmd, cur, cur->type);
}

void	delete_redir_token(t_command *cmd, t_token *pre, t_token *cur)
{
	t_token	*file;

	file = cur->next;
	delete_one_token(&(cmd->token), pre, cur, cur->next);
	delete_one_token(&(cmd->token), pre, file, file->next);
}

int	parser(t_mshell *mshell, char *cmdline)
{
	t_token		*head;
	t_token		*pre;
	t_token		*cur;
	t_command	*cmd;

	printf("here\n");
	head = tokenizer(mshell, cmdline);
	if (check_syntax(head))
	{
		free_all_token(head);
		ft_putstr_fd("syntax error\n", 2);
		return (1);
	}
	pre = NULL;
	cur = head;
	mshell->commands = ft_calloc(1, sizeof(t_command));
	cmd = mshell->commands;
	cmd->token = head;
	while(cur->token)
	{
		if (cur->type == T_PIPE)
		{
			cur = terminate_command(mshell, cur, &cmd);
			cmd->token = cur;
			continue ;
		}
		if (is_redirect_token(cur->type))
		{
			add_redir_info(cmd, cur);
			delete_redir_token(cmd, pre, cur);
			cur = cur->next->next;
			continue ;
		}
		pre = cur;
		cur = cur->next;
		
	}
	cur = terminate_command(mshell, cur, &cmd);
	//print_commands(mshell);
	//print_redir(mshell->commands);
	return (0);
}

