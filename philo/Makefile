NAME := philo
CC := cc
CFLAGS := -Wall -Wextra -Werror -g
OBJDIR := objs
SRCS := helper_functions.c initializer.c philosophers.c philo_actions.c waiter_actions.c
OBJS := $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJDIR)/%.o: ./%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re