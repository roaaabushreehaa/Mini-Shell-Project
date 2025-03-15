/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabu-shr <rabu-shr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:14:40 by rabu-shr          #+#    #+#             */
/*   Updated: 2025/03/15 16:26:00 by rabu-shr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// char	*get_path(char *cmd, char **envp)
// {
// 	int		i;
// 	char	*exec;
// 	char	**paths;
// 	char	*path_part;
// 	char	**s_cmd;

// 	i = -1;
// 	paths = ft_split(my_env("PATH", envp), ':');
// 	if (!paths)
// 		return (NULL);
// 	s_cmd = ft_split(cmd, ' ');
// 	while (paths[++i])
// 	{
// 		path_part = ft_strjoin(paths[i], "/");
// 		exec = ft_strjoin(path_part, s_cmd[0]);
// 		free(path_part);
// 		if (access(exec, F_OK | X_OK) == 0)
// 		{
// 			free_function(s_cmd, paths);
// 			return (exec);
// 		}
// 		free(exec);
// 	}
// 	free_function(s_cmd, paths);
// 	return (NULL);
// }

// char	*my_env(const char *key, char **envp)
// {
// 	int		i;
// 	int		j;
// 	char	*sub;

// 	i = 0;
// 	while (envp[i])
// 	{
// 		j = 0;
// 		while (envp[i][j] && envp[i][j] != '=')
// 			j++;
// 		sub = ft_substr(envp[i], 0, j);
// 		if (ft_strncmp(sub, key, ft_strlen(key)) == 0)
// 		{
// 			free(sub);
// 			return (envp[i] + j + 1);
// 		}
// 		free(sub);
// 		i++;
// 	}
// 	return (NULL);
// }

// void	free_function(char **s_cmd, char **paths)
// {
// 	ft_free(s_cmd);
// 	ft_free(paths);
// }

// int get_cmd_execution(t_cmd *cmd,char **env,t_files *files)
// {
// 	int		fd[2];
// 	t_cmd *temp;
// 	int i;
// 	i=0;
// 	temp=cmd;
// 	while (i < cmd->cmd_count - 1)
// 	{
// 		if (i < cmd->cmd_count - 1)
// 		{
// 			if (pipe(fd) == -1)
// 			{
// 				perror("Pipe error");
// 				return (1);
// 			}
// 			init_fork(temp,files);
// 		}
		
// 	}
	
	
// }


// int init_fork(t_cmd *cmd,t_files *files)
// {

// cmd->pid=fork();

// if (cmd->pid == 0 && cmd->pid != -1)
// {
// 	child_process(cmd,files);
// }

// }

