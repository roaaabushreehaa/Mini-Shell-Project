/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 14:52:16 by jalqam            #+#    #+#             */
/*   Updated: 2025/03/06 14:56:11 by jalqam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int unset_command(t_cmd *cmd, t_env **env)
{
    int i;
    t_env *node;
    t_env *prev;

    i = 1;
    while(cmd->args[i])
    {
        node = *env;
        prev = NULL;
        while(node)
        {
            if(!ft_strcmp(node->key, cmd->args[i]))
            {
                if(prev)
                    prev->next = node->next;
                else
                    *env = node->next;
                free_env_node(node);
                break;
            }
            prev = node;
            node = node->next;
        }
        i++;
    }
    return (0);
}
