/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabu-shr <rabu-shr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:31:38 by jalqam            #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *getpwd(char *prompt)
{
	char *pwd;
	
	pwd = getcwd(NULL, 0);
	if(!pwd)
	{
		ft_printf("Error: getcwd\n");
		return (NULL);
	}
	prompt = ft_strjoin("🎀minishell:",pwd);
	prompt = ft_strjoin(prompt, "$ ");
	free(pwd);
	return (prompt);
}

int main(int argc, char *argv[], char *envp[])
{
	char	*readline_shell;
	t_token	*tokens;
	t_cmd	*cmd;
	t_env   *env;
	char *prompt;

	//cmd = NULL;
	prompt = NULL;
	(void)argv;
	(void)argc;
	env = init_envp(envp);
    	if (!env)
        	return (1); 
	while (1)
	{
		prompt = getpwd(prompt);
		readline_shell = readline(prompt);
		if (!readline_shell)
		{
			ft_printf("exit\n");
			break ;
		}
		if (*readline_shell)
			add_history(readline_shell);
		if (right_command_check(readline_shell))
			continue ;
		tokens = tokenize(readline_shell);
		get_built_in_type(tokens);
		//define_word(tokens);
		cmd = separator(tokens);
		if(!cmd)
			continue;
		execute_commands(cmd, env,tokens);
			return (1);
		// print_tokens(tokens);
		// print_commands(cmd);
		free_tokens(tokens);
		free_commands(cmd);
		free(readline_shell);
	}
	return (0);
}
