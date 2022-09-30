#include "minishell.h"

char *path_join(char *str, const char *dst)
{
    char *tmp;
    char *path;

    tmp = ft_strjoin(str, "/");
    path = ft_strjoin(tmp, dst);
    ft_memdel(tmp);
    return (path);
}

char *checkdir(char *bin, char *cmd)
{
    DIR *folder;
    struct dirent	*item;
    char *path;

    path = NULL;
    folder = opendir(bin);
    if (folder == NULL)
        return (NULL);
    while ((item = readdir(folder)))
    {
        if (!strcmp(item->d_name, cmd))
            path = path_join(bin, item->d_name);
    }
    closedir(folder);
    return (path);
}

int error_message(char *str)
{
    DIR *folder;
    int fd;
    int ret;

    fd = open(str, O_WRONLY);
    folder = opendir(str);
    ft_putstr_fd("minishell : ", err);
    ft_putstr_fd(str, err);
    if (ft_strrchr(str, '/'))
        ft_putstr_fd(": command not found \n", err);
    else if (fd == -1 && folder == NULL)
        ft_putstr_fd(": no such a file or directory\n", err);
    else if (fd == -1 && folder != NULL)
        ft_putstr_fd(": is a directory\n", err);
    else if (fd != -1 && folder == NULL)
        ft_putstr_fd(": permession denied\n", err);
    if (ft_strrchr(str,'/') == NULL || (fd != -1 && folder == NULL))
        ret = UNKNOWN;
    else 
        ret = ISDIR;
    if (folder)
        closedir(folder);
    ft_close(fd);
    return (ret);
}

int magic_box(char *str, char **strs, t_sll *env, t_prog *prog)
{
    char **env_arr;
    char *ptr;
    int ret;

    ret = SUCCESS;
    g_sig.pid = fork();
    if (!g_sig.pid)
    {
        ptr = env_to_str(env);
        env_arr = ft_split(ptr, '\n');
        ft_memdel(ptr);
        if (ft_strrchr(str, '/'))
            execve(str, strs, env_arr);
        ret = error_message(str);
        free_tab(env_arr);
        free_token(prog->start);
        exit(ret);
    }
    else 
        waitpid(g_sig.pid, &ret, 0);
    if (g_sig.sigint == 1 || g_sig.sigquit == 1)
        return (g_sig.exit_status);
    if (ret == 32256 || ret == 32512)
        ret /= 256;
    else 
        ret = !!ret;
    return (ret);
}

int exec_bin(char **str, t_sll *env, t_prog *prog)
{
    int i;
    int ret;
    char **bins;
    char *path;

    i = 0;
    ret = UNKNOWN;
    // Check if the path variable is defined.
    while (env && env->value && strncmp(env->value, "PATH=", 5))
        env = env->next;
    // If we dont find the path.
    // Then we will look for the executable in the current directory.
    if (!env || !env->next)
        return magic_box(str[0], str, env, prog);
    // if we find the env variable. 
    // Otherwise, we will check all possible paths for this executable.
    // If we find it, we execute it.
    bins = ft_split(env->value, ':');
    // if both the command and the variable are missing.
    if (!str[0] && !bins[0])
        return (ERROR);
    i = 1;
    path = checkdir(bins[0] + 5, str[0]);
    while (str[0] && bins[i] && path == NULL)
        path = checkdir(bins[i] + 5, str[0]);
    // if the path exists, we execute it.
    if (path != NULL)
        ret = magic_box(path, str, env, prog);
    // if the path doesnt exist, we try to execute the command here.
    else 
        ret = magic_box(str[0], str, env, prog);
    free_tab(bins);
    ft_memdel(path);
    return (ret);
}