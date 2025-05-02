CC			=	cc -Wall -Wextra -Werror #-g3 -fsanitize=address

NAME		=	philo
OBJDIR		=	objs/
SRCS		=	a.c
OBJS		=	$(SRCS:%.c=$(OBJDIR)%.o)

AR			=	ar rcs
RM			=	rm -f

all:		$(NAME)

$(NAME):    $(OBJS)
				@echo "Inviting philosophers over 🤔🧠💭"
				@$(CC) $(OBJS) -o $(NAME)
				@echo "Cooking up the dinner 🍲🍳🍖"
				@echo "✅ Ready to eat ✅"
				@$(RM) 

$(OBJDIR)%.o: $(SRCDIR)%.c
				@mkdir -p $(OBJDIR)
				@$(CC) -c $< -o $@

clean:
				@$(RM) $(OBJS)
				@rm -rf $(OBJDIR)
				@echo "Washing the dishes... 🧽🧼🍽✨"

fclean:			clean
				@$(RM) $(NAME)
				@$(RM) $(EXEC)

re:				fclean all

.PHONY:       	all clean fclean re