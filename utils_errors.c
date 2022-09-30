#include "minishell.h"

int export_error(int error, const char *str)
{
    int i;

    // exporting a wild card.
    if (error == -1)
        ft_putstr_fd("export : not vlaid in this context", err);
    // 0 variable name start with a digit, -3 if the variable name is missing
    else if (error == 0 || error == -3)
        ft_putstr_fd("export : not a valid identifier", err);
    i = 0;
    while (str[i] && (str[i] != '=' || error != -3))
    {
        write(err, &str[i], 1);
        i++;
    }
    write(err, "\n", 1);
    return (ERROR);
}