#include "minishell.h"

void reset_fds(t_prog *prog)
{
    prog->fdin = -1;
    prog->fdout = -1;
    prog->pipin = -1;
    prog->pipout = -1;
    prog->pid = -1;
}

void ft_close(int fd)
{
    if (fd > 0)
        close(fd);
}