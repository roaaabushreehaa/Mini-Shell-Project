/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabu-shr <rabu-shr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:31:38 by jalqam            #+#    #+#             */
/*   Updated: 2025/02/27 18:55:47 by rabu-shr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char *argv[])
{
	char	*readline_shell;
	t_token	*tokens;
	t_cmd	*cmd;

	cmd = NULL;
	(void)argv;
	(void)argc;
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
		define_word(tokens);
		cmd = separator(tokens);
		print_tokens(tokens);
		print_commands(cmd);
		free_tokens(tokens);
		free(readline_shell);
	}
	return (0);
}
