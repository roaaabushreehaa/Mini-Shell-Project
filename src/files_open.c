/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_open.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabu-shr <rabu-shr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:47:08 by rabu-shr          #+#    #+#             */
/*   Updated: 2025/03/13 14:54:42 by rabu-shr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void print_file_counts(t_files *files)
{
    if (!files)
        return;
    printf("Number of input files: %s\n", files->last_file_in);
    printf("Number of output files: %s\n", files->last_file_out);
}
void execute_in_files(t_token *temp , t_files *files)
{
    char *new_file;

    if (!temp || !temp->value || !files)
        return;
    files->count_infiles++;
    if (check_validtion(temp->value, 0) < 0)
    {
        printf("THE FILE DOES NOT EXIST\n");
        files->valid_in = 0;
        return;
    }
    new_file = ft_strdup(temp->value);
    if (!new_file)
        return;
    if (files->last_file_in)
        free(files->last_file_in);
    files->last_file_in = new_file;
}

void execute_out_files(t_token *temp , t_files *files)
{
    int fd;

    fd = check_validtion(temp->value, 1);
    if (fd < 0)
    {    
        printf("Cannot create/open output file: %s\n", temp->value);
        files->valid_out = 0;
        return;
    }
    close(fd); 
    files->count_outfiles++;
        
    if (files->last_file_out)
        free(files->last_file_out);
    files->last_file_out = ft_strdup(temp->value);
}

t_files *init_files(t_token *token)
{
    t_files *files;
    t_token *temp;

    if (!token)
        return (NULL);
    files = malloc(sizeof(t_files));
    if (!files)
        return (NULL);
    files->count_infiles = 0;
    files->count_outfiles = 0;
    files->last_file_in = NULL;
    files->last_file_out = NULL;
    files->valid_in = 1;
    files->valid_out = 1;
    temp = token;
    
    while (temp)
    {
        if ((temp->type == REDIRECT_IN || temp->type == INFILE) && temp->next)
        {
            temp = temp->next;
            execute_in_files(temp, files);
        }
        else if ((temp->type == REDIRECT_OUT || temp->type == OUTFILE) && temp->next)
        {
            temp = temp->next;
            execute_out_files(temp, files);
        }
        temp = temp->next;
    }
    return (files);
}

int check_validtion(char *file,int which)
{
	int	fd;
	fd = 0;
	if (which == 0)
	{
		fd = open(file, O_RDONLY);
	}
	else if (which == 1)
	{
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	if (fd == -1)
	{
		return (-1);
	}
	return (fd);
}
