NAME	=	philo
CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror -O3 #-fsanitize=thread -g

SRCDIR	=	.
OBJDIR	=	objs

SRCS	=	$(shell find $(SRCDIR) -name '*.c')
OBJS	=	$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@echo "Inviting philosophers over 🤔🧠💭"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lpthread
	@echo "Cooking up the dinner 🍲🍳🍖"
	@echo "Setting the table 🍽️ 🍴"
	@echo "✅ Ready to eat ✅"

clean:
	@echo "Washing the dishes... 💦🧼🫧"
	@if [ -n "$(OBJDIR)" ] && [ "$(OBJDIR)" != "/" ]; then rm -rf $(OBJDIR); fi

fclean: clean
	@rm -f $(NAME)

re: fclean all
all: $(NAME)

.PHONY: all clean fclean re

