#include "minishell.h"

static size_t env_size(const char *str)
{
    size_t size;

    size = 0;
    while (str && str[size] !='=')
        size++;
    return (size);   
}

static void free_node(t_prog *prog, t_sll *env)
{
    if (prog->env == env && env->next == NULL)
    {
        ft_memdel(prog->env->value);
        prog->env->value = NULL;
        prog->env->next = NULL;
        return ;
    }
    ft_memdel(env->value);
    ft_memdel(env);
}

int ft_unset(char **str, t_prog *prog)
{
    t_sll *env;
    t_sll *temp;

    env = prog->env;
    if (str && !str[1])
        return (SUCCESS);
    // this apparently removes a variable form an environment. 
    if (!strncmp(str[1], env->value, env_size(env->value)))
    {
        if (env->next)
            prog->env = env->next;
            // we gon delete the current env element. 
            // we already removed it from the program's link.
            free_node(prog, env);
    }
    // Checking the remaining items.
    // in case of any redunduncy, delete the item from env.
    while (env && env->next)
    {
        if (!strncmp(str[1], env->next->value, env_size(env->next->value)))
        {
            temp = env->next->next;
            free_node(prog, env->next);
            env->next = temp;
            return (SUCCESS);
        }
        env = env->next;
    }
    return (SUCCESS);
}