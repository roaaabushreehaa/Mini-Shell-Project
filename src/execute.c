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


// void	execute(char *cmd, char **envp)
// {
// 	char	**s_cmd;
// 	char	*path;

// 	s_cmd = ft_split(cmd, ' ');
// 	if(!s_cmd || !s_cmd[0])
// 	{
// 		if (s_cmd)
// 			free(s_cmd);
// 		exit(1);
// 	}
// 	path = get_path(s_cmd[0], envp);
// 	if (!path)
// 		ft_exit(path, s_cmd, 0);
// 	if (cmd == 0 || cmd[0] == ' ')
// 		ft_exit(path, s_cmd, 2);
// 	if (cmd[0] == '/')
// 	{
// 		if (!access(cmd, X_OK))
// 			execve(cmd, s_cmd, envp);
// 		ft_exit(path, s_cmd, 3);
// 	}
// 	if (execve(path, s_cmd, envp) == -1)
// 		ft_exit(path, s_cmd, 1);
// 	ft_exit(path, s_cmd, 3);
// }

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