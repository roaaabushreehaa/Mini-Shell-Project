/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:18:13 by rabu-shr          #+#    #+#             */
/*   Updated: 2025/03/05 14:13:43 by jalqam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int execute_commands(t_cmd *cmd, t_env *env)
{
    t_cmd *cmds;
    int status = 0;

    cmds = cmd;
    while (cmds)
    {
        // if (!cmds->args[0])
        // {
        //     cmds = cmds->next;
        //     continue;
        // }
        if (!ft_strcmp(cmds->args[0], "pwd"))
        {
            char *word = getcwd(NULL, 0);
            if (word)
            {
                printf("%s\n", word);
                free(word);
            }
            else
                status = 1;
        }
        else if (!ft_strcmp(cmds->args[0], "echo"))
        {
            echo_command(cmds);
            status = 1;
        }
        else if (!ft_strcmp(cmds->args[0], "exit"))
            exit_command(cmd);
        else if (!ft_strcmp(cmds->args[0], "env"))
            env_print(env);
        else if(!ft_strcmp(cmd->args[0],"export"))
            export_command(cmds, &env);
        //     status = execute_external_command(cmds, env);
        cmds = cmds->next;
    }
    return status;
}

void env_print(t_env *env)
{
    if (!env)
        return;
    while (env)
    {
        printf("%s=%s\n", env->key, env->value);
        env = env->next;
    }
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
        if (cmds->args[i + 1])
            printf(" ");
        i++;
    }
    if (!no_newline)
        printf("\n");
}

int valid_echo_arg(char *arg)
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

void exit_command(t_cmd *cmds)
{
    int exit_code;

    exit_code = 0;
    if (cmds->args[1])
    {
        exit_code = ft_atoi(cmds->args[1]); 
        // printf("%d",exit_code);
        if (!ft_isnumeric(cmds->args[1])) 
        {
            printf("exit\n");
            fprintf(stderr, "minishell: exit: %s: numeric argument required\n", cmds->args[1]);
            exit(255); 
        }
    }
    printf("exit\n");
    exit(exit_code);
}


