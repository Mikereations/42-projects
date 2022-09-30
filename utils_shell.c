#include "minishell.h"

static int invalid_lvl(char *str, int i)
{
    int j;

    j = i;
    while (str[j])
    {
        if (str[j] <= '0' || str[j] >= '9')
            return (0);
        j++;
    }
    return (1);
}

static int  get_lvl(char *shell_value)
{
    int i;
    int sign;
    int num;

    i = 0;
    sign = 1;
    num = 0;
    while (shell_value[i] && shell_value[i] <= 32)
        i++;
    if (shell_value[i] == '-')
        sign = -1;
    if (shell_value[i] == '-' || shell_value[i] == '+')
        i++;
    if (!invalid_lvl(shell_value, i))
        return (0);
    while (shell_value[i])
        num = num * 10 + (shell_value[i++] - '0');
    return (num * sign);
}

void increment_shell_level(t_sll *env)
{
    int shell_level;
    char env_name[BUFF_SIZE];
    char *shlvl;
    char *shell_value;

    shell_value = get_env_value("SHLVL", env);
    if (!strcmp(shell_value, ""))
        return ;
    shell_level = get_lvl(shell_value) + 1;
    ft_memdel(shell_value);
    while (env && env->next)
    {
        get_variable_name(env_name, env->value);
        if (!strcmp("SHLVL", env_name))
        {
            ft_memdel(env->value);
            shlvl = ft_itoa(shell_level);
            env->value = ft_strjoin("SHLVL=", shlvl);
            ft_memdel(shlvl);
            return ;
        }
        env = env->next;
    }
}

void mini_exit(t_prog *prog, char **str)
{
    prog->exit = 1;
    ft_putstr_fd("exit ", err);
    if (!str[1])
        ft_putstr_fd("❤️\n", err);
    else 
        ft_putstr_fd("💚\n", err);
    if (str[1] && str[2])
    {
        prog->ret = 1;
        ft_putstr_fd("minishell: exit: too many arguments\n", err);
    }else if (str[1] && ft_strisnum(str[1]))
    {
        prog->ret = 255;
        ft_putstr_fd("minishell: exit: ", err);
        ft_putstr_fd(str[1], err);
        ft_putstr_fd(": numeric argument required\n", err);
    }
    else if (str[1])
        prog->ret = atoi(str[1]);
    else 
        prog->ret = 0;
        
    

}