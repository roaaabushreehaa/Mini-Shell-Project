/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabu-shr <rabu-shr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:31:38 by jalqam            #+#    #+#             */
/*   Updated: 2025/03/15 15:38:08 by rabu-shr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int main(int argc, char *argv[], char *envp[])
{
	char	*readline_shell;
	t_token	*tokens;
	t_cmd	*cmd;
	t_env   *env;
	(void)argv;
	(void)argc;
	env = init_envp(envp);
    	if (!env)
        	return (1); 
	while (1)
	{
		readline_shell = readline("🎀 minishell> ");
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
		// print_tokens(tokens);
		// print_commands(cmd);
		free_tokens(tokens);
		free_commands(cmd);
		free(readline_shell);
	}
	return (0);
}
