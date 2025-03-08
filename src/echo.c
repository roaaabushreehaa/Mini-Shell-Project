/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabu-shr <rabu-shr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 14:35:05 by jalqam            #+#    #+#             */
/*   Updated: 2025/03/06 16:12:00 by rabu-shr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int valid_echo_arg(char *arg)
{
    int j;
    j = 1;
    if (arg[0] != '-')
        return (0);
    if (arg[j] != 'n')
        return (0);
    while (arg[j] == 'n')
        j++;
    while (arg[j] == 'e' || arg[j] == 'E' || arg[j] == 'n')
        j++;
    return (arg[j] == '\0');
}

void echo_command(t_cmd *cmds)
{
    int i;
    int no_newline;
    i = 1;
    no_newline = 0;

    while (cmds->args[i] && cmds->args[i][0] == '-' && valid_echo_arg(cmds->args[i]))
    {
        no_newline = 1;
        i++;
    }
    while (cmds->args[i])
    {
        printf("%s", cmds->args[i]);
        if (cmds->args[i + 1] && !(cmds->args[i][0] == '\0'))
        {
            int curr_len = ft_strlen(cmds->args[i]);
            char last_char = cmds->args[i][curr_len - 1];
            char first_next = cmds->args[i + 1][0];
            if (!(ft_strchr("'\"", last_char) && first_next != '"' && first_next != '\''))
                printf(" ");
        }
        i++;
    }
    if (!no_newline)
        printf("\n");
}