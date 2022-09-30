#include "minishell.h"


// This function will intialise a singly linked list with
// the env variables stores in an array of strings.
// I am worried about not checking the empty env values before
// filling the first element of the env. This might cause an error
// if not handled elsewhere.
int init_env(t_prog *prog, char **env_vals)
{
    t_sll *env;
    t_sll *new;
    int i;

    env = (t_sll *)malloc(sizeof(t_sll));
    if (!env)
        return (1);
    env->value = ft_strjoin("", env_vals[0]);
    env->next = NULL;
    prog->env = env;
    i = 1;
    while(env_vals && env_vals[0] && env_vals[i])
    {
        new = (t_sll *)malloc(sizeof(t_sll));
        if (!new)
            return (1);
        new->value = ft_strjoin("",env_vals[i]);
        new->next = NULL;
        env->next = new;
        env = new;
        i++;
    }
    return (0);
}

int init_senv(t_prog *prog, char **env_vals)
{
    t_sll *env;
    t_sll *new;
    int i;

    env = (t_sll *)malloc(sizeof(t_sll));
    if (!env)
        return (1);
    env->value = ft_strjoin("", env_vals[0]);
    env->next = NULL;
    prog->secret_env = env;
    i = 1;
    while (env_vals && env_vals[0] && env_vals[i])
    {
        new = (t_sll *)malloc(sizeof(t_sll));
        if (!new)
            return (1);
        new->value = ft_strjoin("", env_vals[i]);
        // printf("%s\n", new->value);
        // printf("%d\n", i);
        new->next = NULL;
        env->next = new;
        env = new;
        i++;
    }
    return (0);
}

int is_valid_env(const char *env)
{
    int i;

    i = 0;
    if(ft_isdigit(env[i]))
        return (0);
    while (env[i] && env[i] != '=')
    {
        if (!ft_isalnum(env[i]))
            return (-1);
        i++;
    }
    if (env[i] != '=')
        return (2);
    return (1);
}

static size_t size_env(t_sll *head)
{
    size_t size;

    size = 0;
    // I think we should remove the second condition.
    while (head && head->next)
    {
        if (head->value)
        {
            size += strlen(head->value);
            size++;
        }
        head = head->next;
    }
    return (size);
}

char *env_to_str(t_sll *head)
{
    char *env;
    int i;
    int j;

    env = (char *)malloc(size_env(head) + 1);
    if (!env)
        return (NULL);
    i = 0;
    while (head && head->next)
    {
        if (head->value)
        {
            j = 0;
            while (head->value[j])  
                env[i++] = head->value[j++];
        }
        if (head->next->next != NULL)
            env[i++] = '\n';
        head = head->next;
    }
    env[i] = 0;
    return (env);
}

void sort_env(char **tab, int env_len)
{
    int ordered; 
    int i;
    char *temp;

    ordered = 0;
    while (tab && !ordered)
    {
        ordered = 1;
        i = 0;
        while (i < env_len - 1)
        {
            if (strcmp(tab[i], tab[i + 1]) > 0)
            {
                temp = tab[i];
                tab[i] = tab[i + 1];
                tab[i + 1] = temp;
                ordered = 0;
            }
            i++;
        }
        env_len--;
    }
}

int  str_env_len(char **env)
{
    int i;

    i = 0;
    while (env[i])
        i++;
    return (i);
}

void print_sorted_env(t_sll *env)
{
    int i;
    char **tab;
    char *str_env;

    str_env = env_to_str(env);
    tab = ft_split(str_env, '\n');
    ft_memdel(str_env);
    sort_env(tab, str_env_len(tab));
    i = 0;
    while (tab[i])
    {
        ft_putstr_fd("declare -x ", 1);
        ft_putstr_fd(tab[i], 1);
        write(1, "\n", 1);
        i++;
    }
    free_tab(tab);
}