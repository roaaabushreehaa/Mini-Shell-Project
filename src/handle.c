/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabu-shr <rabu-shr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 19:00:20 by jalqam            #+#    #+#             */
/*   Updated: 2025/03/08 14:35:25 by rabu-shr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	right_command_check(char *command)
{
	int	i;
	int	flag_echo;

	flag_echo = 0;
	i = 0;
	while (command[i])
	{
		if (command[i] == '"')
			flag_echo = 1;
		if (command[i] == '>' && command[i + 1] == '\0')
		{
			text_error(0);
			return (1);
		}
		else if ((!flag_echo && command[i] == 'e' && command[i + 1] == 'c'
					&& command[i + 2] == 'h' && command[i + 3] == 'o')
				&& command[i + 4] != ' ' && command[i + 5])
		{
			text_error(1);
			return (1);
		}
		i++;
	}
	return (0);
}

void	text_error(int which)
{
	if (which == 0)
	{
		ft_printf("syntax error near unexpected token `newline'\n");
		return ;
	}
	else if (which == 1)
	{
		ft_printf("command not found\n");
		return ;
	}
}

int check_quotes_num(t_token *token)
{
    t_token *temp = token;
    while (temp)
    {
        int i = 0;
        int in_double = 0;
        int in_single = 0;
        
        while (temp->value && temp->value[i])
        {
            if (temp->value[i] == '"')
            {
                if (!in_single)
                    in_double = !in_double;
            }
            else if (temp->value[i] == '\'')
            {
                if (!in_double)
                    in_single = !in_single;
            }
            i++;
        }
        
        if ((in_single && !in_double) || (!in_single && in_double))
            return (1);
        temp = temp->next;
    }
    return (0);
}
