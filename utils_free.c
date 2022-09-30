#include "minishell.h"

void    free_env(t_sll *env)
{
    t_sll *tmp;

    while (env && env->next)
    {
        tmp = env;
        env = env->next;
        // printf("%s\n", tmp->value);
        ft_memdel(tmp->value);
        ft_memdel(tmp);
    }
    if (env)
    {
        ft_memdel(env->value);
        ft_memdel(env);
    }
}

