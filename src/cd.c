/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:20:55 by jalqam            #+#    #+#             */
/*   Updated: 2025/03/06 15:21:08 by jalqam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int cd_command(t_cmd *cmds, t_env *env)
{
    char *path;
    t_env *home;

    if (!cmds->args[1] || cmds->args[1][0] == '~')
    {
        home = find_env_node(env, "HOME");
        if (!home)
        {
            ft_printf("minishell: cd: HOME not set\n");
            return (1);
        }
        path = home->value;
        if (chdir(path) == -1)
        {
            perror("minishell: cd");
            return (1);
        }
    }
    else if (cmds->args[2])
    {
        ft_printf("minishell: cd: too many arguments\n");
        return (1);
    }
    else if (chdir(cmds->args[1]) == -1)
    {
        perror("minishell: cd");
        return (1);
    }
    return (0);
}
