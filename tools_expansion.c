#include "minishell.h"

int ret_size(int ret)
{
    char *tmp;
    int ret_len;

    tmp = ft_itoa(ret);
    ret_len = strlen(tmp);
    ft_memdel(tmp);
    return (ret_len);
}

int get_var_len(const char *str, int pos, t_sll *env, int ret)
{
    char var_name[BUFF_SIZE];
    char *var_value;
    int i;

    i = 0;
    if (str[pos] == '?')
        return (ret_size(ret));
    if (ft_isdigit(str[pos]))
        return (0);
    while (str[pos] && is_env_char(str[pos]) && i < BUFF_SIZE)
        var_name[i++] = str[pos++];
    var_name[i] = 0;
    var_value = get_env_value(var_name, env);
    i = strlen(var_value);
    ft_memdel(var_value);
    return (i);
}


int arg_alloc_len(const char *str, t_sll *env, int ret)
{
    int i;
    int size;

    i = -1;
    size = 0;
    while (str[++i])
    {
        if (str[i] == EXPANSION)
        {
            i++;
            if ((str[i] == 0 || !ft_isalnum(str[i])) && str[i] != '?')
                size++;
            else 
                size += get_var_len(str, i, env, ret);
            if (!ft_isdigit(str[i]))
            {
                while (str[i + 1] && is_env_char(str[i]))
                    i++;
            }else
                size--;
        }
        size++;
    }
    return (size);
}

char *get_var_value(const char *str, int pos, t_sll *env, int ret)
{
    char var_name[BUFF_SIZE];
    char *var_value;
    int i;

    i = 0;
    if (str[pos] == '?')
    {
        var_value = ft_itoa(ret);
        return (var_value);
    }
    if (ft_isdigit(str[pos]))
        return (NULL);
    while (str[pos] && is_env_char(str[pos]) && i < BUFF_SIZE)
        var_name[i++] = str[pos++];
    var_name[i] = 0;
    var_value = get_env_value(var_name, env);
    return (var_value);
}