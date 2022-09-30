#include "minishell.h"

int ft_echo(char **str)
{
    int i;
    int op;

    i = 1;
    op = 0;
    if (nb_args(str) > 1)
    {
        while (str[i] && !strcmp(str[i], "-n"))
        {
            op = 1;
            i++;
        }
        while (str[i])
        {
            ft_putstr_fd(str[i],1);
            if (str[i + 1] && str[i][0] != 0)
                write(1, " ", 1);
            i++;
        }
    }
    if (op == 0)
        write(1, "\n", 1);
    return (SUCCESS);
}

int ft_pwd(void)
{
    char cwd[PATH_MAX];

    if (getcwd(cwd, PATH_MAX))
    {
        ft_putstr_fd(cwd, 1);
        write(1, "\n", 1);
        return (SUCCESS);
    }
    return (ERROR);
}

int ft_cd(char **str, t_sll *env)
{
    int cd_ret;

    if (!str[1])
        cd_ret = go_to_path(0, env);
    else if (!strcmp(str[1], "-"))
        cd_ret = go_to_path(1,env);
    else {
        update_old_pwd(env);
        cd_ret = chdir(str[1]);
        if (cd_ret < 0)
            cd_ret *= -1;
        if (!cd_ret)
            print_error(str);
    }
    return (cd_ret);
}

int ft_export(char **str, t_sll *env, t_sll *senv)
{
    int new_env;
    int error_ret;

    new_env = 0;
    if (!str[1])
    {
        print_sorted_env(senv);
        return (SUCCESS);
    }
    else 
    {
        // 0 if we have a digit, -1 if its not a letter (special caracter),
        // 2 if we never reach the = sign. 1 if we do and find a variable name.
        error_ret = is_valid_env(str[1]);
        if (str[1][0] == '=')
            error_ret = -3;
        if (error_ret <= 0)
            // will return an error code
            return (export_error(error_ret, str[1]));
        // if we have a variable name with no value;
        if (error_ret == 2)
            new_env = 1;
        else
            is_in_env(env, str[1]);
        // not in the new env or has no value;
        if (!new_env)
        {
            // we find the variable name but its not in the environment.
            if (error_ret == 1)
                env_add(str[1], env);
            // so the sercre env will contain variables with no values.
            // and variables that do have values as well.
            env_add(str[1], senv);
        }
    }
    return (SUCCESS);
}