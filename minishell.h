/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:36:10 by jalqam            #+#    #+#             */
/*   Updated: 2025/03/05 17:00:49 by jalqam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include"libft/libft.h"

typedef enum s_enum
{
	WORD, //0
	PIPE, //1
	REDIRECT_IN, //2 //<
	REDIRECT_OUT, //3 //>
	APPEND, //4 // >>
	HEREDOC, //5
	DQUOTE, //6
	SQUOTE, //7
	COMMAND, //8
	INFILE,//9
	OUTFILE,//10
	ARGS//11
}	t_enum;

typedef struct s_token
{
	char *value;
	t_enum type;
    struct s_token *next;
}t_token;

typedef struct s_cmd
{
	char **args;
	char *value;
	int command_num;
	char *type;
	t_token *tokens;
	struct s_cmd *next;
}t_cmd;

typedef struct s_split
{
	size_t	i;
	size_t	j;
	int		in_quotes;
	char	quote_char;
	char	*new_word;
	char	**word_splitted;
}	t_split;

typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
} t_env;

t_token *new_token(char *value) ;
void add_token(t_token **head,char *value);
t_token *tokenize(char *input);
void print_tokens(t_token *head);
void free_tokens(t_token *head);
void free_tokens_split(char **token_split);
int right_command_check(char * command);
char **our_split(char *word);
int	ft_strcmp(const char *s1, const char *s2);
char **split_with_quotes(char *str);
void	split_quotes(char c, int *in_quotes, char *quote_char);
void	split_chars(char *word, char *new_word, size_t *i, size_t *j,
		int in_quotes);
void	init_split_vars(t_split *s, char *word);
void	free_result(char **result, size_t j);
void	skip_spaces(char *str, size_t *i);
void	handle_quotes(char *str, size_t *i, int *in_quotes, char *quote_char);
char	*extract_word(char *str, size_t *i);
void text_error(int which);
int count_pip(t_token *token);
t_cmd	*command(t_token **token, int cmd_num);
t_cmd *separator(t_token *token);
void print_commands(t_cmd *cmd);
void get_built_in_type(t_token *token);
void define_word (t_token *token);
void define_word (t_token *token);
int execute_commands(t_cmd *cmd, t_env *env);
void echo_command(t_cmd *cmds);
int valid_echo_arg(char *arg);
void exit_command(t_cmd *cmds);
void env_print(t_env *env);
t_env *create_env_node(char *env_var);
void	free_env_list(t_env *env);
t_env	*init_envp(char **env_list);
void free_commands(t_cmd *cmd);
int export_command(t_cmd *cmd, t_env **env);
int	print_export_command(t_env *env);
int is_valid_export(char *arg);
#endif