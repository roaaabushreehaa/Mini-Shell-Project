/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabu-shr <rabu-shr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 17:59:17 by rabu-shr          #+#    #+#             */
/*   Updated: 2025/02/24 11:58:27 by rabu-shr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = malloc(sizeof(char *) * 100);
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	cmd->command_num = 0;
	cmd->type = NULL;
	cmd->next = NULL;
	return (cmd);
}

t_cmd	*command(t_token **token, int cmd_num)
{
	t_cmd	*new_cmd;
	int		count_cmd;

	count_cmd = 0;
	new_cmd = init_cmd();
	if (!new_cmd)
		return (NULL);
	while (*token && ft_strcmp((*token)->value, "|") != 0)
	{
		new_cmd->args[count_cmd++] = ft_strdup((*token)->value);
		*token = (*token)->next;
	}
	new_cmd->args[count_cmd] = NULL;
	new_cmd->command_num = cmd_num;
	if (*token && ft_strcmp((*token)->value, "|") == 0)
		*token = (*token)->next;
	return (new_cmd);
}

t_cmd	*separator(t_token *token)
{
	t_cmd	*cmd_list;
	t_cmd	*cmd_node;
	t_cmd	*new_cmd;
	int		cmd_num;

	cmd_node = NULL;
	cmd_list = NULL;
	cmd_num = 1;
	while (token)
	{
		new_cmd = command(&token, cmd_num++);
		if (!new_cmd)
			return (NULL);
		if (!cmd_list)
			cmd_list = new_cmd;
		else
			cmd_node->next = new_cmd;
		cmd_node = new_cmd;
	}
	return (cmd_list);
}

void	print_commands(t_cmd *cmd)
{
	while (cmd)
	{
		printf("Command %d: ", cmd->command_num);
		for (int i = 0; cmd->args[i]; i++)
			printf("%s ", cmd->args[i]);
		printf("\n");
		cmd = cmd->next;
	}
}
