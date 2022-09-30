#include "minishell.h"

int nb_args(char **str)
{
    int count;

    count = 0;
    while (str[count])
    {
        count++;
    }
    return (count);
}