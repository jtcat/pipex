CC:= cc

CFLAGS:= -Wall -Wextra -Werror

NAME:= pipex

SRC:= main.c utils.c

INC:= pipex.h

LFT_DIR:= libft

LFT:= $(LFT_DIR)/libft.a

LFT_INC:= libft.h

OBJ:= $(SRC:.c=.o)

all: $(NAME)

%.o:	%.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(NAME): $(LFT)	$(INC) $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LFT)

$(LFT_INC): $(LFT_DIR)/$(LFT_INC)
	cp $(LFT_DIR)/$(LFT_INC) ./libft.h

$(LFT): $(LFT_INC)
	$(MAKE) all -C $(LFT_DIR)

re:	fclean all

clean:
	$(MAKE) -C $(LFT_DIR) clean
	$(RM) $(OBJ)

fclean:	clean
	$(MAKE) -C $(LFT_DIR) fclean
	$(RM) $(NAME)
