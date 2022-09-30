#include "minishell.h"

t_dll *prev_sep(t_dll *token, e_sk skip)
{
    if (token && skip == yes)
        token = token->prev;
    while (token && token->key < trunc)
        token = token->prev;
    return (token);
}

t_dll *next_sep(t_dll *token, e_sk skip)
{
    if (token && skip == yes)
        token = token->next;
    while (token && token->key < trunc)
        token = token->next;
    return (token);
    
}
// So far what I understand is, "we will keep going until we hit a cmd"
t_dll *next_run(t_dll *head, e_sk skip)
{
    if (head && skip == yes)
        head = head->next;
    while (head && head->key != cmd)
    {
        head = head->next;
        if (head && head->key == cmd && head->prev == NULL)
            ;
        else if (head && head->key == cmd && head->prev->key < end)
            head = head -> next;
    }
    return (head);
}