#include "minishell.h"

void print_error(char **str)
{
    ft_putstr_fd("cd : ", err);
    if (str[2])
        ft_putstr_fd("string not in pwd ", err);
    else {
        ft_putstr_fd(strerror(errno), err);
        ft_putstr_fd(": ", err);
    }   
    ft_putstr_fd(str[1],err);
    write(err, "\n", 1);
}

static char *get_env_path(t_sll *env, const char *var, size_t len)
{
    char *oldpwd;
    int i;
    int j;
    int s_alloc;

    while (env && env->next)
    {
        if (!strncmp(env->value, var, len))
        {
            // start after the variable name;
            // and gets the length of the variable after VAR passed.
            // So it stores only the vlaue of the variable.
            // We could substitute this with searching for variable with variable name
            // using env_variable_name and then getting its value using env_value or smth
            s_alloc = strlen(env->value) - len;
            oldpwd = malloc(s_alloc + 1);
            if (!oldpwd)
                return (NULL);
            i = 0;
            j = 0;
            // ignore the = sign
            while (env->value[i++])
            {
                if (i > (int)len)
                    oldpwd[j++] = env->value[i];
            }
            oldpwd[j] = 0;
            return (oldpwd);
        }
        env = env->next;
    }
    return (NULL);
}

int update_old_pwd(t_sll *env)
{
    char cwd[PATH_MAX];
    char *oldpwd;

    if(!getcwd(cwd, PATH_MAX))
        return (ERROR);
    oldpwd = ft_strjoin("OLDPWD=", cwd);
    if (!oldpwd)
        return (ERROR);
    if (!is_in_env(env, oldpwd))
        env_add(oldpwd, env);
    ft_memdel(oldpwd);
    return (SUCCESS);
}

int go_to_path(int options, t_sll *env)
{
    int ret;
    char *env_path;

    env_path = NULL;
    // This is the case when we do cd without an argument.
    if (!options)
    {
        update_old_pwd(env);
        env_path = get_env_path(env, "HOME", 4);
        if (!env_path)
            ft_putstr_fd("minishell : cd: HOME not set\n", err);
        if (!env_path)
            return (ERROR);
    }
    else if (options == 1)
    {
        env_path = get_env_path(env, "OLDPWD", 6);
        if (!env_path)
            ft_putstr_fd("minishell : cd: OLDPWD not set", err);
        if (!env_path)
            return (ERROR);
        update_old_pwd(env);
    }
    ret = chdir(env_path);
    ft_memdel(env_path);
    return (ret);
}