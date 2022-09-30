#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/wait.h>
# include <limits.h>
# include <errno.h>
# include <signal.h>

typedef enum types {
    empty,
    cmd,
    arg,
    trunc,
    append,
    input,
    pipx,
    end
} e_ty;

typedef enum std {
    in,
    out,
    err
} e_std;

typedef enum skip {
    yes,
    no
} e_sk;

# define BUFF_SIZE 4096
# define EXPANSION -36
# define ERROR 1
# define SUCCESS 0
# define ISDIR 126
# define UNKNOWN 127

typedef struct s_dll {
    char *value;
    e_ty key;
    struct s_dll *prev;
    struct s_dll *next;
} t_dll ;

typedef struct s_expansion {
    char *content;
    int i;
    int j;
} t_expansion;

typedef struct s_sll {
    char *value;
    char *key;
    struct s_sll *next;
} t_sll;

typedef struct s_prog {
    t_sll *env;
    t_sll *secret_env;
    t_dll *start;
    e_std in;
    e_std out;
    int fdin;
    int fdout;
    int pipin;
    int pipout;
    int pid;
    int exit;
    int ret;
    int no_exec;
    int charge;
    int parent;
    int last;
} t_prog;

typedef struct s_sig {
    int sigint;
    int sigquit; 
    int exit_status;
    pid_t pid;
} t_sig;


// utils.fd
void close_fds(t_prog *prog);
void reset_std(t_prog *prog);
// utils.read
void reset_fds(t_prog *prog);
void ft_close(int fd);
int init_env(t_prog *prog, char **env_vals);
int init_senv(t_prog *prog, char **env_vals);
// libft
char	*ft_strjoin(char const *s1, char const *s2);
void    *ft_memdel(void *ptr);
void	ft_putstr_fd(char *s, int fd);
int	ft_isdigit(int c);
char	*ft_strrchr(const char *s, int c);
int	ft_isalnum(int c);
int	ft_strisnum(const char *str);
// split
char		**ft_split(char const *s, char c);
// executables
int ft_cd(char **str, t_sll *env);
int ft_export(char **str, t_sll *env, t_sll *senv);
int ft_pwd(void);
int ft_echo(char **str);
int ft_exports(char **str, t_sll *env, t_sll *senv);
// utils.env
int is_env_char(int c);
char *get_variable_name(char *var, char *all);
char *get_env_value(char *name, t_sll *env);
void    free_env(t_sll *env);
void sig_int(int sign);
char		*ft_itoa(int n);
void sig_quit(int sign);
int quotes(char *line, int index);
int ft_env(t_sll *env);
int is_valid_env(const char *env);
void print_sorted_env(t_sll *env);
char *env_to_str(t_sll *head);
// utils.parse
void parse(t_prog *prog);
// utils.shell
void increment_shell_level(t_sll *env);
void mini_exit(t_prog *prog, char **str);
// utils.sig
void sig_init(void);
void sig_quit(int sign);
void sig_int(int sign);
// utils.errors
int export_error(int error, const char *str);
// utils.redir 
void redir(t_prog *prog, t_dll *token, e_ty type);
void input_func(t_prog *prog, t_dll *token);
int minipipe(t_prog *prog);
// utils_token
void squish_args(t_prog *prog);
void type_arg(t_dll *token, int separator);
t_dll *get_token(char *line);
// utils.expansion
char    *expansions(char *str, t_sll *env, int ret);
// utils.builtsins
int exec_builtin(char **str, t_prog *prog);
int is_builtin(char *str);
// utils.executables
int nb_args(char **str);
// utils.bins
int exec_bin(char **str, t_sll *env, t_prog *prog);
// utils.exec
void exec_cmd(t_prog *prog, t_dll *token);
// utils.cd 
int go_to_path(int options, t_sll *env);
int update_old_pwd(t_sll *env);
void print_error(char **str);
// utils.unset
int ft_unset(char **str, t_prog *prog);
// utils.expansions 
char    *expansion(char *str, t_sll *env, int ret);
// utils.exports
int is_in_env(t_sll *env, char *str);
int env_add(const char *value, t_sll *env);
// tools.parse
int is_last_valid(t_dll *token);
int check_line(t_prog *prog, t_dll *token);
int is_sep(char *line, int i);
int ignore_sep(char *line, int i);
// tools.token;
t_dll *prev_sep(t_dll *token, e_sk skip);
t_dll *next_run(t_dll *head, e_sk skip);
t_dll *next_sep(t_dll *token, e_sk skip);
// tools.type
int is_types(t_dll *token, char *types);
int is_type(t_dll *token, e_ty type);
int has_pipe(t_dll *token);
// tools.free
void free_token(t_dll *head);
void free_tab(char **tab);
// tools.fd
void fd_close(int fd);
// tools.expansion 
int arg_alloc_len(const char *str, t_sll *env, int ret);
char *get_var_value(const char *str, int pos, t_sll *env, int ret);
// gnl
int		get_next_line(int fd, char **line);

t_sig g_sig;

#endif