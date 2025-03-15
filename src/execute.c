/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabu-shr <rabu-shr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:43:07 by rabu-shr          #+#    #+#             */
/*   Updated: 2025/03/15 16:58:18 by rabu-shr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
        
    char *expanded;
    int i = 0;
    while (cmds->args[i])
    {
        expanded = handle_dollar_expander(&(t_token){.value = cmds->args[i]}, env);
        free(cmds->args[i]);
        cmds->args[i] = expanded;
        i++;
    }
        echo_command(cmds);
            status = 1;
        }
        else if (!ft_strcmp(cmds->args[0], "exit"))
            exit_command(cmd);
        else if (!ft_strcmp(cmds->args[0], "env"))
            env_print(env);
        else if(!ft_strcmp(cmd->args[0],"export"))
            export_command(cmds, &env);
        else if(!ft_strcmp(cmd->args[0],"unset"))
            unset_command(cmds, &env);
        else if(!ft_strcmp(cmd->args[0],"cd"))
            cd_command(cmds, env);
        // else
        //     status = execute_external_command(cmds, env);
        cmds = cmds->next;
    }
    return (status);
}
//execute_external_command

void env_print(t_env *env)
{
    if (!env)
        return;
    while (env)
    {
        if (env->value)
            printf("%s=%s\n", env->key, env->value);
        env = env->next;
    }
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