#include "minishell.h"

static char **cmd_tab(t_dll *head)
{
    t_dll *token;
    char **tab;
    int     i;

    if (!head)
        return (NULL);
    token = head->next;
    i = 2;
    while (token && token->key < trunc)
    {
        token = token->next;
        i++;
    }
    tab = (char **)malloc(sizeof(char *) * i);
    if (!tab)
        return (NULL);
    token = head->next;
    tab[0] = head->value;
    i = 1;
    while (token && token->key < trunc)
    {
        tab[i++] = token->value;
        token = token->next;
    }
    tab[i] = 0;
    return (tab);
}

void exec_cmd(t_prog *prog, t_dll *token)
{
    char **str;
    int     i;

    if (!prog->charge)
        return;
    str = cmd_tab(token);
    i = 0;
    while (str && str[i])
    {
        str[i] = expansions(str[i], prog->env, prog->ret);
        i++;
    }
    if (str && !strcmp(str[0], "exit") && !has_pipe(token))
        mini_exit(prog, str);
    // if the command is one of the buit-ins we just activate it.
    else if (str && is_builtin(str[0]))
        prog->ret = exec_builtin(str, prog);
    // if we have some other command, not one of the 5 defined. 
    else if (str)
        prog->ret = exec_bin(str, prog->env, prog);
    free_tab(str);
    ft_close(prog->pipout);
    ft_close(prog->pipin);
    prog->pipin = -1;
    prog->pipout = -1;
    prog->charge = 0;
}
