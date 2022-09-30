#include "minishell.h"

void sig_init(void)
{
    g_sig.exit_status = 0;
    g_sig.sigint = 0;
    g_sig.sigquit = 0;
    g_sig.pid = 0;
}

void sig_int(int sign)
{
    (void) sign;
    if (g_sig.pid == 0)
    {
        ft_putstr_fd("\b\b", err);
        ft_putstr_fd("\n", err);
        ft_putstr_fd("\033[0;36m\033[1m🤬 minishell ▸ \033[0m", err);
        g_sig.exit_status = 1;
    }
    else
    {
        ft_putstr_fd("\n", err);
        g_sig.exit_status = 130;
    }
    g_sig.sigint = 1;
}

void sig_quit(int sign)
{
    char *nbr;

    nbr = ft_itoa(sign);
    if (g_sig.pid != 0)
    {
        ft_putstr_fd("Quit : ", err);
        ft_putstr_fd(nbr, err);
        g_sig.exit_status = 131;
        g_sig.sigquit = 1;
    }
    else 
        ft_putstr_fd("\b\b \b\b",err);
    ft_memdel(nbr);
}