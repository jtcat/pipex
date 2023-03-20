CC:= cc

CFLAGS:= -Wall -Wextra -Werror

NAME:= pipex

SRC:= main.c utils.c

SRC_BONUS:= main_bonus.c utils_bonus.c

LFT_DIR:= libft

LFT:= $(LFT_DIR)/libft.a

LFT_INC:= libft.h

OBJ:= $(SRC:.c=.o)

OBJ_BONUS:= $(SRC_BONUS:.c=.o)

all: $(NAME)

bonus: $(NAME) $(LFT) $(OBJ_BONUS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ_BONUS) $(LFT)
	touch bonus

%.o:	%.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(NAME): $(LFT)	$(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LFT)

$(LFT_INC): $(LFT_DIR)/$(LFT_INC)
	cp $(LFT_DIR)/$(LFT_INC) ./libft.h

$(LFT): $(LFT_INC)
	$(MAKE) all -C $(LFT_DIR)

re:	fclean all

clean:
	$(MAKE) -C $(LFT_DIR) clean
	$(RM) $(OBJ) $(OBJ_BONUS)

fclean:	clean
	$(MAKE) -C $(LFT_DIR) fclean
	$(RM) $(NAME)
