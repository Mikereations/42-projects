#include "minishell.h"

int is_in_env(t_sll *env, char *str)
{
    char var_name[BUFF_SIZE];
    char env_name[BUFF_SIZE];

    // we get the variable name. we recieve name=value and come back with name.
    // then we store name in var_name.
    get_variable_name(var_name, str);
    while (env && env->next)
    {
        // store in env_name the variable names of each of the environment varbiables.
        get_variable_name(env_name, env->value);
        // if we find in the env a variable with the same name. 
        // we first delete it, then update its value.
        // return error because it exists already.
        if (!strcmp(env_name, var_name))
        {
            ft_memdel(env->value);
            env->value = ft_strjoin("", str);
            return (ERROR);
        }
        env = env->next;
    }
    // return success because it doesnt exist.
    return (SUCCESS);
}

int env_add(const char *value, t_sll *env)
{   
    t_sll *new;
    t_sll *tmp;

    if (env && !env->value)
    {
        env->value = ft_strjoin("", value);
        return (SUCCESS);
    }
    if (!(new = malloc(sizeof(t_sll))))
        return (ERROR);
    new->value = ft_strjoin("", value);
    while (env && env->next && env->next->next)
        env = env->next;
    tmp = env->next; 
    env->next = new;
    new->next = tmp;
    return (SUCCESS);
}