/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabu-shr <rabu-shr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:43:07 by rabu-shr          #+#    #+#             */
/*   Updated: 2025/03/15 15:55:24 by rabu-shr         ###   ########.fr       */
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