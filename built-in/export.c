/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 13:39:19 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/13 15:50:24 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	print_export(t_envList *env)
{
	t_envList	*cur;
	char		*val;

	(void)val;
	cur = env;
	while (cur->key)
	{
		printf("declare -x %s", cur->key);
		if (cur->val)
			printf("=\"%s\"", cur->val);
		printf("\n");
		cur = cur->next;
	}
}

static int	is_valid(char *s)
{
	char	*eq_pos;
	char	*plus_pos;
	char	*cur;

	eq_pos = ft_strchr(s, '=');
	if (!eq_pos)
		eq_pos = ft_strchr(s, '\0');
	if (*eq_pos != '\0' && *(eq_pos - 1) == '+')
		plus_pos = eq_pos - 1;
	else
		plus_pos = eq_pos;
	cur = s;
	if (!(ft_isalpha(*cur) || *cur == '_') || cur == plus_pos)
		return (0);
	while (cur != plus_pos)
	{
		if (!(ft_isalnum(*cur) || *cur == '_'))
			return (0);
		cur++;
	}
	return (1);
}

static void	create_export_argv(t_mshell *mshell, t_command *cmd)
{
	size_t	len;
	t_token	*cur;

	create_argv(mshell, cmd);
	len = 0;
	while (cmd->argv[len])
		len++;
	free_array(cmd->argv);
	cmd->argv = ft_calloc(len + 1, sizeof(char *));
	len = 0;
	cur = cmd->token;
	while (1)
	{
		cur = skip_delimiter_token(cur);
		if (cur && cur->type == T_END)
			break ;
		cmd->argv[len++] = concat_expanded_tokens(mshell, cur);
		cur = skip_by_next_delimiter_token(cur);
	}
	cmd->argv[len] = NULL;
}

static void	print_errmsg(char *cmd)
{
	ft_putstr_fd("export: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
}

int	ft_export(t_mshell *mshell, t_command *cmd)
{
	char		*key_val[2];
	size_t		i;
	int			status;

	create_export_argv(mshell, cmd);
	sort_env(mshell->env);
	if (cmd->argc == 1)
		print_export(mshell->env);
	i = 0;
	status = 0;
	while (cmd->argv[++i])
	{		
		if (!is_valid(cmd->argv[i]))
		{
			print_errmsg(cmd->argv[i]);
			status = EXIT_FAILURE;
			continue ;
		}
		get_key_val(cmd->argv[i], key_val);
		append_lefthand_side(mshell, cmd, i, key_val);
		register_or_update_env(mshell, key_val[0], key_val[1]);
		free(key_val[0]);
		free(key_val[1]);
	}
	return (status);
}

/*		if (*(ft_strchr(cmd->argv[i], '=') - 1) == '+')
		{
			tmp = key_val[1];
			key_val[1] = ft_strjoin(get_env(mshell, key_val[0]), key_val[1]);
			free(tmp);
		}
*/