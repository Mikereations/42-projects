#include "minishell.h"

int is_builtin(char *str)
{
    if (!strcmp(str, "echo"))
        return (1);
    if (!strcmp(str, "pwd"))
        return (1);
    if (!strcmp(str, "cd"))
        return (1);
    if (!strcmp(str, "env"))
        return (1);
    if (!strcmp(str, "export"))
        return (1);
    if (!strcmp(str, "unset"))
        return (1);
    return (0);
}

int exec_builtin(char **str, t_prog *prog)
{
    int results;

    results = 0;
    if (!strcmp(str[0], "echo"))
        results = ft_echo(str);
    if (!strcmp(str[0], "pwd"))
        results = ft_pwd();
    if (!strcmp(str[0], "cd"))
        results = ft_cd(str, prog->env);
    if (!strcmp(str[0], "env"))
        results = ft_env(prog->env);
    if (!strcmp(str[0], "export"))
        return (1);
        results = ft_export(str, prog->env, prog->secret_env);
    if (!strcmp(str[0], "unset"))
        results = ft_unset(str, prog);
    return (results);
}
