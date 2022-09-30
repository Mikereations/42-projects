#include "minishell.h"

// here you will see if that argument is actually a special seperator
// one that was not meant to be ignored. 
void type_arg(t_dll *token, int separator)
{
    // you will assign each token a proper key indicator to its nature.
    if (!strcmp(token->value, ""))
        token->key = empty;
    // The following are the special seperators (commands).
    else if (!strcmp(token->value, ">") && !separator)
        token->key = trunc;
    else if (!strcmp(token->value, ">>") && !separator)
        token->key = append;
    else if (!strcmp(token->value, "<") && !separator)
        token->key = input;
    else if (!strcmp(token->value, "|") && !separator)
        token->key = pipx;
    else if (!strcmp(token->value, ";") && !separator)
        token->key = end;
    // If you are at the start of the command or right after the >,>>,<,|,; signs
    // those are key words you wanna pay attention to 
    else if (token->prev == NULL || token->prev->key >= trunc)
        token->key = cmd;
    // anything else is just an argument.
    else
        token->key = arg;
}

void squish_args(t_prog *prog)
{
    t_dll *head;
    t_dll *prev;

    head = prog->start;
    while (head)
    {
        // Find the last < > >> occurance.
        prev = prev_sep(head, no);
        // if they already appeared before. and currently we are on an argument.
        // rearrange, such that all arguments are there together.
        if (is_type(head, arg) && is_types(prev, "ATI"))
        {
            // This one should always pass, since prev is ATI already.
            while (!is_last_valid(prev))
                // you go to the first non ATI before the seperator
                prev = prev->prev;
            // remove head from its current position,
            // connecting whats before it and after it.
            head->prev->next = head->next;
            if (head->next)
                head->next->prev = head->prev;
            head->prev = prev;
            // inserting the head after the prev token.
            // Otherwise to the start of the list.
            if (prev)
                head->next = prev->next;
            else    
                head->next = prog->start;
            if (!prev)
                prev = head;
            prev->next->prev = head;
            if (!prog->start->prev)
                prev->next = head;
            else
                prog->start = prog->start->prev;
        }
        head = head->next;
    }
}

// Checks how much space is needed for the content of the token.
int next_alloc(char *line, int *i)
{
    int count;
    int j;
    char c;

    count = 0;
    j = 0;
    c = ' ';
    // This will go through the line, starting from some point and until you find a
    // space in the line.
    while (line[*i + j] && (line[*i + j] != ' ' || c != ' '))
    {
        // If you find an opening quote, change c to that.
        if (c == ' ' && (line[*i + j] == '\'' || line[*i + j] == '\"'))
            c = line[*i + j++];
        // if you already found an opening quote, then a matching closing quote
        else if (c != ' ' && line[*i + j] == c)
        {
            // to be subtracted. You will not include qoutes.
            count += 2;
            c = ' ';
            j++;
        }
        else 
            j++;
        if (line[*i + j - 1] == '\\')
            count--;
    }
    // number of characters between spaces - quotes + EOL
    return (j - count + 1);
}

// This function will arrange the next token nice and nice
t_dll *next_token(char *line, int *i)
{
    // prepare a token for the action (wink)
    t_dll *token;
    int j;
    char c;

    j = 0;
    c = ' ';
    token = malloc(sizeof(t_dll));
    if (!token) 
        return (NULL);
    // Check how much space you need for the content of the next token.
    token->value = malloc(next_alloc(line, i));
    if (!token->value)
    {
        free(token);
        return (NULL);
    }
    while (line[*i] && (line[*i] != ' '|| c!= ' '))
    {
        // When we see quotes, we do not include them in the new line.
        if (c == ' ' && (line[*i] == '\'' || line[*i] == '\"'))
            c = line[(*i)++];
        else if (c != ' ' && line[*i] == c)
        {
            c = ' ';
            (*i)++;
        }
        else if (line[*i] == '\\' && (*i)++)
            token->value[j++] = line[(*i)++];
        else
            token->value[j++] = line[(*i)++];
    }
    token->value[j] = 0;
    // The token returned is basically a break down of everything with
    // spaces between them.
    return (token);
}

// In this function we store the commands in a doubly linked list.
// Based on their appearance in the command.
// This might contain multiple itterations of commands.
t_dll *get_token(char *line)
{
    t_dll *next;
    t_dll *prev;
    int i;
    int sep;

    prev = NULL;
    next = NULL;
    i = 0;
    // Ignore any white spaces at the start.
    while (line[i] == ' ')
            i++;
    while (line[i])
    {
        // the seperator will be ignored if it was following a backslash
        sep = ignore_sep(line, i);
        // the token is a space seperated stement of your input
        next = next_token(line, &i);
        // updating the doubly linked list in both directions.
        next->prev = prev;
        // if not the first node (where prev is NULL) -> set the next to the prev;
        if (prev)
            prev->next = next;
        // move this node (next) one step down to prev. now prev is pointing at the current node.
        prev = next;
        // label each word with its proper label.
        type_arg(next, sep);
        // cut off empty spaces now.
        while (line[i] == ' ')
            i++;
    }
    // try to regain the head of your double linked list.
    if (next)   
        next->next = NULL;
    while (next && next->prev)
        next = next->prev;
    // return it.
    return (next);
}