#include "minishell.h"

void free_token(t_dll *head)
{
    while (head && head->next)
    {
        ft_memdel(head->value);
        head = head->next;
        ft_memdel(head->prev);
    }
    if (head)
    {
        ft_memdel(head->value);
        ft_memdel(head);
    }
}

// Maybe need to delete the \0 also. Not sure.
// Used again, in the program to delete the env.
void free_tab(char **tab)
{
    int i;

    i = 0;
    while (tab[i])
    {
        ft_memdel(tab[i]);
        i++;
    }
    if (tab)
        ft_memdel(tab);
}