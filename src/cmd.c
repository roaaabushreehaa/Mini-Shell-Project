/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabu-shr <rabu-shr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 17:59:17 by rabu-shr          #+#    #+#             */
/*   Updated: 2025/03/15 15:32:30 by rabu-shr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
    cmd->cmd_count=0;
	cmd->type = NULL;
	cmd->next = NULL;
    cmd->pid=0;
    cmd->prev=-1;
	return (cmd);
}
t_cmd	*command(t_token **token, int cmd_num)
{
    t_cmd	*new_cmd;
    int		count_cmd;
    char	*processed_value;

    count_cmd = 0;
    new_cmd = init_cmd();
    if (!new_cmd)
        return (NULL);
    
    while (*token && ft_strcmp((*token)->value, "|") != 0)
    {
        processed_value = NULL;
        if ((*token)->value)
        {
            if (count_cmd == 0)
            {
                expander_main(*token);
                processed_value = ft_strdup((*token)->value);
                
            }
            else
            {
                if (ft_strchr((*token)->value, '\'') && ft_strchr((*token)->value, '"'))
                    processed_value = handle_mixed_quotes(*token);
                else if (ft_strchr((*token)->value, '\'') || ft_strchr((*token)->value, '"'))
                    processed_value = ft_strdup((*token)->value);
                else
                    processed_value = ft_strdup((*token)->value);
            }
            if (!processed_value)
            {
                free_cmd(new_cmd);
                return (NULL);
            }
            new_cmd->args[count_cmd++] = processed_value;
        }
        *token = (*token)->next;
    }
    
    new_cmd->args[count_cmd] = NULL;
    new_cmd->command_num = cmd_num;
    if (*token && ft_strcmp((*token)->value, "|") == 0)
        *token = (*token)->next;
    return (new_cmd);
}

t_cmd *separator(t_token *token)
{
    t_cmd *cmd_list;
    t_cmd *cmd_node;
    t_cmd *new_cmd;
    int cmd_num;
    int total_cmds;

    if (check_quotes_num(token))
    {
        ft_printf("Error: Unmatched quotes\n");
        return (NULL);
    }
    cmd_node = NULL;
    cmd_list = NULL;
    cmd_num = 1;
    total_cmds = 0;
    while (token)
    {
        if (token->type != PIPE)
        {
            new_cmd = command(&token, cmd_num);
            if (!new_cmd)
                return (NULL);
            if (!cmd_list)
                cmd_list = new_cmd;
            else
                cmd_node->next = new_cmd;
            cmd_node = new_cmd;
            cmd_num++;
            total_cmds++;
        }
        else
            token = token->next;
    }
    
    // Set total command count for each node
    cmd_node = cmd_list;
    while (cmd_node)
    {
        cmd_node->cmd_count = total_cmds;
        cmd_node = cmd_node->next;
    }
    
    return (cmd_list);
}


void print_commands(t_cmd *cmd)
{
    t_cmd *temp = cmd;
    
    if (!temp)
        return;
        
    while (temp)
    {
        printf("Command %d:\n", temp->command_num);
        if (temp->args)
        {
            for (int i = 0; temp->args[i]; i++)
                printf("arg[%d]:%s\n", i, temp->args[i]);
        }
        printf("count: %d\n", temp->cmd_count);
        printf("\n");
        temp = temp->next;
    }
}

void	free_cmd(t_cmd *cmd)
{
    int i;

    if (cmd)
    {
        if (cmd->args)
        {
            for (i = 0; cmd->args[i]; i++)
                free(cmd->args[i]);
            free(cmd->args);
        }
        free(cmd);
    }
}

void	free_cmd_list(t_cmd *cmd_list)
{
    t_cmd *tmp;

    while (cmd_list)
    {
        tmp = cmd_list;
        cmd_list = cmd_list->next;
        free_cmd(tmp);
    }
}

