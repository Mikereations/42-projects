#include "minishell.h"

void redir(t_prog *prog, t_dll *token, e_ty type)
{
    ft_close(prog->fdout);
    if (type == trunc)
        prog->fdout = open(token->value, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    else    
        prog->fdout = open(token->value, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
    if (prog->fdout == -1)
    {
        ft_putstr_fd("minishell: ", err);
        ft_putstr_fd(token->value, err);
        // ft_putendl_fd();
        ft_putstr_fd(": no such a file or directory" ,err);
        prog->ret = 1;
        prog->no_exec = 1;
        return ;
    }
    dup2(prog->fdout, out);
}

void input_func(t_prog *prog, t_dll *token)
{
    ft_close(prog->fdin);
    prog->fdin = open(token->value, O_RDONLY | S_IRWXU);
    if (prog->fdin == -1)
    {
        ft_putstr_fd("minishell: ", err);
        ft_putstr_fd(token->value, err);
        ft_putstr_fd(": No such a file or directory", err);
        prog->ret = 1;
        prog->no_exec = 1;
        return ;
    }
    dup2(prog->fdin, in);
}

int minipipe(t_prog *prog)
{
    pid_t pid; 
    int pipefd[2];

    pipe(pipefd);
    pid = fork();
    if (!pid)
    {
        ft_close(pipefd[1]);
        dup2(pipefd[0], in);
        prog->pipin = pipefd[0];
        prog->pid = -1;
        prog->parent = 0;
        prog->no_exec = 0;
        return (2);
    }
    else
    {
        ft_close(pipefd[0]);
        dup2(pipefd[1], out);
        prog->pipout = pipefd[1];
        prog->pid = pid;
        prog->last = 0;
        return (1);
    }
}