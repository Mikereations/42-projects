#include "minishell.h"

void fd_close(int fd)
{
    if (fd > 0)
        close(fd);
}

void close_fds(t_prog *prog)
{
    fd_close(prog->fdin);
    fd_close(prog->fdout);
    fd_close(prog->pipout);
    fd_close(prog->pipin);
}

void reset_std(t_prog *prog)
{
    dup2(prog->in, in);
    dup2(prog->out, out);
}
