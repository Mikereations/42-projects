#include "minishell.h"

int is_type(t_dll *token, e_ty type)
{
    if (token && token->key == type)
        return (1);
    else 
        return (0);
}

int is_types(t_dll *token, char *types)
{
    if (ft_strrchr(types, ' ') && is_type(token, empty))
        return (1);
    else if (ft_strrchr(types, 'X') && is_type(token, cmd))
        return (1);
    else if (ft_strrchr(types, 'x') && is_type(token, arg))
        return (1);
    else if (ft_strrchr(types, 'T') && is_type(token, trunc))
        return (1); 
    else if (ft_strrchr(types, 'A') && is_type(token, append))
        return (1);
    else if (ft_strrchr(types, 'I') && is_type(token, input))
        return (1);
    else if (ft_strrchr(types, 'P') && is_type(token, pipx))
        return (1);
    else if (ft_strrchr(types, 'E') && is_type(token, end))
        return (1);
    return (0);
} 

int has_pipe(t_dll *token)
{
    while (token && !is_type(token, end))
    {
        if (is_type(token, pipx))
            return (1);
        token = token->next;
    }
    return (0);
}