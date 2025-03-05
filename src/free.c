/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 13:49:36 by jalqam            #+#    #+#             */
/*   Updated: 2025/03/05 18:14:53 by jalqam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void free_commands(t_cmd *cmd)
{
    t_cmd *current;
    t_cmd *next;
    int i;

    current = cmd;
    while (current)
    {
        next = current->next;
        i = 0;
        while (current->args && current->args[i])
        {
            free(current->args[i]);
            i++;
        }
        if (current->args)
            free(current->args);
        free(current);
        current = next;
    }
}