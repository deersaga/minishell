/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaou <kaou@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 13:39:19 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/01 17:50:14 by kaou             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_export(t_envList *env)
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
	char	*cur;

	eq_pos = ft_strchr(s, '=');
	if (!eq_pos)
		eq_pos = ft_strchr(s, '\0');
	cur = s;
	while (cur != eq_pos)
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

int	ft_export(t_mshell *mshell, t_command *cmd)
{
	char		*key_val[2];
	size_t		i;

	printf("called ft_export\n");
	print_tokens(cmd->token);
	create_export_argv(mshell, cmd);
	if (cmd->argc == 1)
	{
		sort_env(mshell->env);
		print_export(mshell->env);
	}
	i = 0;
	while (cmd->argv[++i])
	{		
		if (!is_valid(cmd->argv[i]))
		{
			ft_putstr_fd("export: not a valid identifier\n", 2);
			continue ;
		}
		get_key_val(cmd->argv[i], key_val);
		register_or_update_env(mshell, key_val[0], key_val[1]);
		free(key_val[0]);
		free(key_val[1]);
	}
	return (0);
}
