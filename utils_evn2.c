#include "minishell.h"


// This function might be the source of potential error.
char *env_varible_vlaue(char *env, char *name)
{
    int i;
    int j;
    int size;
    char *var_value;

    if (strnstr(env, name, strlen(env)) == NULL)
        return NULL;
    size = strlen(strnstr(env, name, strlen(env)));
    var_value = (char *)malloc(strlen(env) - size);
    size++;
    i = 0;
    while (size < strlen(env))
    {
        var_value[i] = env[size];
        i++;
        size++;
    }
    var_value[i] = 0;
    return (var_value);
}

char *get_env_value(char *name, t_sll *env)
{
    char env_name[BUFF_SIZE];
    char *env_val;

    env_val = NULL;
    while (env && env->value)
    {
        get_variable_name(env_name, env->value);
        if (!strcmp(name,env_name))
        {
            env_val = env_varible_vlaue(env->value, name);
            return (env_val);
        }
        env = env->next;
    }
    return (NULL);
}

char *get_variable_name(char *var, char *all)
{
    int i;

    i = 0;
    while (i < BUFF_SIZE && all[i] && all[i] != '=')
    {
        var[i] = all[i];
        i++;
    }
    var[i] = 0;
    return (var);
}

int is_env_char(int c)
{
    if (ft_isalnum(c) || c == '_')
    {
        return (1);
    }
    return (0);
}

int ft_env(t_sll *env)
{
    while (env && env->next)
    {
        ft_putstr_fd(env->value, err);
        ft_putstr_fd("\n", err);
        env = env->next;
    }
    if (env)
    {
        ft_putstr_fd(env->value, err);
        ft_putstr_fd("\n", err); 
    }
    return (SUCCESS);
}