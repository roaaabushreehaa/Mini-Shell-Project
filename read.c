/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:31:38 by jalqam            #+#    #+#             */
/*   Updated: 2025/03/05 13:58:51 by jalqam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char *argv[], char *envp[])
{
    char    *readline_shell;
    t_token *tokens;
    t_cmd   *cmd;
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
            break;
        }
        if (*readline_shell)
            add_history(readline_shell);
        if (right_command_check(readline_shell))
        {
            free(readline_shell);
            continue;
        }
        tokens = tokenize(readline_shell);
        define_word(tokens);
        cmd = separator(tokens);
        // print_tokens(tokens);
        // print_commands(cmd);
        execute_commands(cmd, env);
        free_tokens(tokens);
        free_commands(cmd);
        free(readline_shell);
    }
    free_env_list(env);
    return (0);
}
