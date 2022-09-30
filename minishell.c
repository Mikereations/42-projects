# include "minishell.h"

void redir_and_exec(t_prog *prog, t_dll *current)
{
    t_dll *prev;
    t_dll *next;
    int pipe;

    prev = prev_sep(current, no);
    next = next_sep(current, no);
    pipe = 0;
    // redirect the output to the file
    if (is_type(prev, trunc))
        redir (prog, current, trunc);
    else if (is_type(prev, append))
        redir (prog, current, append);
    else if (is_type(prev, input))
        input_func(prog, current);
    else if (is_type(prev, pipx))
        pipe = minipipe(prog);
    if (next && !is_type(next, end) && pipe != 1)
        redir_and_exec(prog, next->next);
    if ((is_type(prev, end) || is_type(prev, pipx) || !prev) 
        && pipe != 1 && prog->no_exec == 0)
        exec_cmd(prog, current);
}

void minishell(t_prog *prog)
{
    t_dll *head;
    int status;

    head = next_run(prog->start, no);
    // if its > >> ; | move to the next
    if (is_types(head, "TAI"))
        head = prog->start->next;
    while (!prog->exit && head)
    {
        prog->charge = 1;
        prog->parent = 1;
        prog->last = 1;
        redir_and_exec(prog, head);
        reset_fds(prog);
        close_fds(prog);
        reset_fds(prog);
        waitpid(-1, &status, 0);
        status = WEXITSTATUS(status);
        if (!prog->last)
            prog->ret = status;
        if (!prog->parent)
        {
            free_token(prog->start);
            exit(prog->ret);
        }
        prog->no_exec = 0;
        head = next_run(head, yes);
    }
    

}

int main(int argc, char **argv, char **env)
{
    t_prog prog;
    
    (void)argc;
    (void)argv;
    prog.in = in;
    prog.out = out;
    prog.exit = 0;
    prog.ret = 0;
    printf("%s\n", env[3]);
    reset_fds(&prog);
    init_env(&prog, env);
    init_senv(&prog, env);
    // Abolutely no idea with this does even after everything.
    increment_shell_level(prog.env);
    while (!prog.exit)
    {
        // Up to here we will only de doing parsing.
        // So we have to signals, this override the kill signals, and ignores them.
        sig_init();
        // So I think the general idea is that older commands will get overritten.
        parse(&prog);
        // start is basically the line. If we dont have a new line we just move on without
        // running minishell
        if (prog.start != NULL && check_line(&prog, prog.start))
            minishell(&prog);
        // we delete the last line.
        free_token(prog.start);
    }
    free_env(prog.env);
    free_env(prog.secret_env);
    return (prog.ret);
}