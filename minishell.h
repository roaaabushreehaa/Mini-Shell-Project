/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:36:10 by jalqam            #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>  // Add this line
#include <readline/readline.h>
#include <readline/history.h>
#include"libft/libft.h"

extern int g_exit_status;  // Add this line to track exit status globally

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
	int count_pip;
    struct s_token *next;
}t_token;

typedef struct s_cmd
{
	char **args;
	char *value;
	pid_t	pid;
	int prev;
	int command_num;
	int cmd_count;
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

typedef struct s_quote_state {
    char *result;
    char *value;
    int i;
    int inside_double;
    int inside_single;
} t_quote_state;

typedef struct s_files
{
	int count_infiles;
	int count_outfiles;
	char *last_file_in;
	char *last_file_out;
	int valid_in;
	int valid_out;
}t_files;

typedef struct s_excute
{
	char *path;
	
}t_excute;

typedef struct s_command
{
    char *cmd;
    struct s_command *next;
} t_command;

typedef struct s_pip
{
	pid_t	*pid;
	int old_fd;
	char *cmd;
	char *path;
}t_pip;
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
int execute_commands(t_cmd *cmd, t_env *env,t_token *tokens);
void echo_command(t_cmd *cmds);
void exit_command(t_cmd *cmds);
void env_print(t_env *env);
t_env *create_env_node(char *env_var);
void	free_env_list(t_env *env);
t_env	*init_envp(char **env_list);
void free_commands(t_cmd *cmd);
int export_command(t_cmd *cmd, t_env **env);
int print_export_command(t_env *env);
int expander_main(t_token *token);
char *handle_mixed_quotes(t_token *token);
void	free_cmd(t_cmd *cmd);
int check_quotes_num(t_token *token);
char *handle_onequote_expander(t_token *token);
char *handle_twoquotes(t_token *token);
t_env *find_env_node(t_env *env, char *key);
void	update_env_value(t_env *node, char *var);
void	append_node(t_env **env, t_env **last, t_env *new_node);
void free_env_node(t_env *node);
int unset_command(t_cmd *cmd, t_env **env);
int cd_command(t_cmd *cmds, t_env *env);
int ft_isnumeric(char *str);
t_files *init_files(t_token *token);
int check_validtion(char *file,int which);
void	execute(char *cmd, char **envp);
char	*get_path(char *cmd, char **envp);
char	*my_env(const char *key, char **envp);
int num_pip(t_token *token);
void	ft_exit(char *path, char **s_cmd, int which);
void	error_handel(void);
void	ft_free(char **s_cmd);
void	free_function(char **s_cmd, char **paths);
int get_cmd_execution(t_cmd *cmd, t_env *env, t_files *files);
int init_fork(t_cmd *cmd,t_files *files);
char *handle_dollar_expander(t_token *token, t_env *env);
#endif
