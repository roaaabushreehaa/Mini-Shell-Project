/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabu-shr <rabu-shr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:18:13 by rabu-shr          #+#    #+#             */
/*   Updated: 2025/03/15 15:55:07 by rabu-shr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_builtin(char *cmd)
{
    return (!ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "echo") ||
            !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "env") ||
            !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset") ||
            !ft_strcmp(cmd, "cd"));
}

int execute_commands(t_cmd *cmd, t_env *env,t_token *tokens)
{
    t_cmd *cmds;
    int status = 0;
    char **env_array;
    env_array = NULL;  
    t_files *files;
    cmds = cmd;
    files =init_files(tokens);
    while (cmds)
    {
        if (!cmds->args[0])
        {
            cmds = cmds->next;
            continue;
        }

        if (is_builtin(cmds->args[0]))
        {
            // Handle builtin commands
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
            else if(!ft_strcmp(cmd->args[0],"unset"))
                unset_command(cmds, &env);
            else if(!ft_strcmp(cmd->args[0],"cd"))
                cd_command(cmds, env);
            // else 
            //     get_cmd_execution(cmd,env,files);
        }
        cmds = cmds->next;
    }
    return (status);
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


