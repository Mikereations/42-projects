#include "minishell.h"

// It allocates enough space for the new line.
// Not changing anything in the previous line.
char *space_alloc(char *line)
{
    char *new;
    int count;
    int i;

    count = 0;
    i = 0;
    // Loop through the line.
    while (line[i])
    {
        // Count how many seperators. "<>|;"
        if (is_sep(line, i))
            count++;
        // Count how many characters.
        i++;
    }
    // Allocates space = number of characters + 2 * number of seperators + end of line.
    new = (char *)malloc(i + 2 * count + 1);
    if (!new)
        return (NULL);
    return (new);
}

// I tried printing this out but I just get the exact same line.
// So I guess we will have to do this the old fashion way, 
// Tracing.
char *space_line(char *line)
{
    // The new line will be stored here.
    char *new;
    int i;
    int j;

    i = 0;
    j = 0;
    // Allocate enough space.
    new = space_alloc(line);
    // Looping through the line.
    while (new && line[i])
    {
        // Basically if this is a $ outside of a double qoutes and not following a backslash.
        if (quotes(line, i) != 2 && line[i] == '$' && i && line[i - 1] != '\\')
            // Store it like this cause dollars are trouble.
            new[j++] = (char)(-line[i++]);
        // If a seperator appears outside of qoutations.
        else if (quotes(line, i) == 0 && is_sep(line, i))
        {
            // you insert an extra space
            new[j++] = ' ';
            // Then you store the variable.
            new[j++] = line[i++];
            // Just in case you had >> not to include double spaces
            if (quotes(line, i) == 0 && line[i] == '>')
                new[j++] = line[i++];
            // Then one extra space after it.
            new[j++] = ' ';
        }
        else
            // Otherwise just copy the line element.
            new[j++] = line[i++];
    }
    // Terminate the line
    new[j] = 0;
    // delete the previous inferior line with single spaces.
    ft_memdel(line);
    return (new);
}

int qoute_check(t_prog *prog, char **line)
{
    // Here we will check the line for matching qoutes.
    // If there are unmatched qoutes, then we dont register the commands.
    if (quotes(*line, 2147483647))
    {
        ft_putstr_fd("minishell: syntax error with open quotes", err);
        ft_memdel(*line);
        prog->ret = 2;
        prog->start = NULL;
        return (1);
    }
    return (0);
}

// When you reach this point in the program. You kinda forget about the env variables for a hot min.

void parse(t_prog *prog)
{
    char *line;
    t_dll *token;

    // First, we will set those two signals to kill the program.
    signal(SIGINT, &sig_int);
    signal(SIGQUIT, &sig_quit);
    // We check, if either of the signals was fired, we display a different emoji.
    // Unnecessery. The numbers are for colors.
    if (!prog->ret)
        ft_putstr_fd("😎 ", err);
    else
        ft_putstr_fd("🤬 ", err);
    ft_putstr_fd("\033[0;36m\033[1mminishell ▸ \033[0m", err);
    // Get the standard input and store it in line.
    // But what is code -2.? : from get next line.
    // Here, we wait for a line from the terminal,
    // Typically, this will be a command, or a number
    // of commands linked with seperators.
    if (get_next_line(0, &line) == -2 && (prog->exit = 1))
    {
        ft_putstr_fd("exit\n", err);
        exit(1);
    }
    // If someone clicked ctrl C already,
    if (g_sig.sigint == 1)
        prog->ret = g_sig.exit_status;
    // If the qoutes are unmatched, then we dont consider the command.
    if (qoute_check(prog, &line))
        return;
    // This new line will have double spaces before and after each command.
    line = space_line(line);
    if (line && line[0] == '$')
        line = (char)(-line[0]);
    // seperate everything in the input that has spaces between them into different tokens
    prog->start = get_token(line);
    // now get rid of the old line, redundant info.
    ft_memdel(line);
    // if multiple consicutive tokens are arg type. collect them together.
    squish_args(prog);
    token = prog->start;
    while (token)
    {
        if (is_type(token, arg))
            type_arg(token, 0);
        token = token->next;
    }
}