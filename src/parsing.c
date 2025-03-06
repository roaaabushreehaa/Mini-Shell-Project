/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 18:15:26 by jalqam            #+#    #+#             */
/*   Updated: 2025/03/06 12:20:32 by jalqam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void define_word(t_token *token)
{
    t_token *temp;
    int flag;

    temp = token;
    flag = 1;
    while (temp)
    {
        if (temp->type == WORD)
        {
            if (flag == 1 || flag == PIPE) 
                temp->type = COMMAND;
            else if (flag == REDIRECT_IN)
                temp->type = INFILE;
            else if (flag == REDIRECT_OUT || flag == APPEND)
                temp->type = OUTFILE;
            else
                temp->type = ARGS;
            flag = 0;
        }
        else if (temp->type == PIPE || temp->type == REDIRECT_IN ||
                 temp->type == REDIRECT_OUT || temp->type == APPEND)
            	flag = temp->type; 
        temp = temp->next;
    }
}
