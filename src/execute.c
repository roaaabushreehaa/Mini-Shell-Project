/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:43:07 by rabu-shr          #+#    #+#             */
/*   Updated: 2025/03/15 15:55:24 by rabu-shr         ###   ########.fr       */
/*   Created: 2025/02/24 18:18:13 by rabu-shr          #+#    #+#             */
/*   Updated: 2025/03/13 16:32:01 by jalqam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int num_pip(t_token *token)
{
    int pipe_count = 0;
    t_token *temp = token;

    if (!temp)
        return (0);
        
    while (temp)
    {
        if (temp->value && ft_strcmp(temp->value, "|") == 0)
        {
            pipe_count++;
        }
        temp = temp->next;
    }
    return (pipe_count);
}