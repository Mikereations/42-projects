#include "minishell.h"

static int varlcpy(char *str, const char *env_var, int pos)
{
    int i;
    
    i = 0;
    while (env_var[i])
        str[pos++] = env_var[i++];
    return (i);
}

static void insert_var(t_expansion *ex, char *str, t_sll *env, int ret)
{
    char *env_value;
    env_value = get_var_value(str, ex->j, env, ret);
    if (env_value)  
        ex->i += varlcpy(ex->content, env_value, ex->i);
    ft_memdel(env_value);
    if (str[ex->j] == '?')
        ex->j++;
    if (!ft_isdigit(str[ex->j]) && str[ex->j - 1] != '?')
    {
        while (is_env_char(str[ex->j]))
            ex->j++;
    }else
    {
        if (str[ex->j - 1] != '?')
            ex->j++;
    }
}

char    *expansions(char *str, t_sll *env, int ret)
{
    t_expansion ex;
    int         new_arg_len;

    new_arg_len = arg_alloc_len(str, env, ret);
    ex.content = malloc(new_arg_len + 1);
    if (!ex.content)
        return (NULL);
    ex.i = 0;
    ex.j = 0;
    while (ex.i < new_arg_len && str[ex.j])
    {
        while (str[ex.j] == EXPANSION)
        {
            ex.j++;
            if ((str[ex.j] == 0 || !ft_isalnum(str[ex.j])) && str[ex.j] != '?')
                ex.content[ex.i] = '$';
            else 
                insert_var(&ex, str, env, ret);
        }
        ex.content[ex.i++] = str[ex.j++];
    }
    ex.content[ex.i] = 0;
    return (ex.content);
}