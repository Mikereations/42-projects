#include "minishell.h"

// This function checks if the current character in the line is a seperator
// that functions to link commands. 
int is_sep(char *line, int i)
{
    if (i > 0 && line[i - 1] == '\\' && ft_strrchr("<>|;", line[i]))
        return (0);
    else if (ft_strrchr("<>|;", line[i]) && quotes(line, i) == 0)
        return (1);
    return (0);
}

// This one will recieve the token at the seperator location
// it makes sure its one of those, > >> < ; |
int is_last_valid(t_dll *token)
{
    t_dll *prev;
    
    if (!token || is_type(token, cmd) || is_type(token, arg))
    {
        prev = prev_sep(token, no);
        if (!prev || is_type(prev, end) || is_type(prev, pipx))
            return (1);
        return (0);
    }
    else
        return (0);
    
}

// Need revisiting, specially the part with >> < >
int ignore_sep(char *line, int i)
{
    if (line[i] && line[i] == '\\' && line[i+1] && line[i + 1] == ';')
        return (1);
    else if (line[i] && line[i] == '\\' && line[i+1] && line[i+1] == '|')
        return (1);
    else if (line[i] && line[i] == '\\' && line[i + 1] && line[i + 1] == '<')
        return (1);
    else if (line[i] && line[i] == '\\' && line[i+1] && line[i+1] == '>'
        && line[i+2] && line[i+2] == '>')
        return (1);
    return (0);
}

int quotes(char *line, int index)
{
    int i;
    int open;

    i = 0;
    open = 0;
    while (line[i] && i != index)
    {
        if (i > 0 && line[i - 1] == '\\')
            ;
        else if (open == 0 && line[i] == '\"')
            open = 1;
        else if (open == 0 && line[i] == '\'')
            open = 2;
        else if (open == 1 && line[i] == '\"')
            open = 0;
        else if (open == 2 && line[i] == '\'')
            open = 0;
        i++;
    }
    return (open);
}

// This function checks whether the line given is valid or not.
int check_line(t_prog *prog, t_dll *token)
{
    // Goes through the line element by element.
    while (token)
    {
        // if it is a command, followed by another command, or by nothing we 
        // give a syntax error.
        if (is_types(token, "TAI") &&
        (!token->next || is_types(token->next, "TAIPE")))
        {
            ft_putstr_fd("bash : syntax error near unexpected token`", err);
            if (token->next)
                ft_putstr_fd(token->next->value, err);
            else 
                ft_putstr_fd("newline", err);
            write(1, "'\n", 2);
            prog->ret = 258;
            return (0);
        }
        if (is_types(token, "PE") && 
        (!token->prev || !token->next || is_types(token->prev, "TAIPE")))
        {
            ft_putstr_fd("bash : syntax error near unexpected token`", err);
            ft_putstr_fd(token->value, err);
            write(1, "'\n", 2);
            prog->ret = 258;
            return (0);
        }
        token = token->next;
    }
    return (1);
}