

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif
NAME = libft_malloc_$(HOSTTYPE).so
SYMLINK = libft_malloc.so


CC = gcc

#F = -Wall -Wextra -Werror -fPIC
F = -Wall -Wextra  -fPIC
FSHARED = -shared -fPIC 


#*** libft ********************************************************************#
LIB_PATH = ./libft
LIB_INC = $(LIB_PATH)/include
LIB = $(LIB_PATH)/libft.a

#*** printf *******************************************************************#
PF_PATH = printf
PF_INC = $(PF_PATH)
PF = $(PF_PATH)/libftprintf.a


#******************************************************************************#
# HEADER
#******************************************************************************#
#INC_FILE_NAME = malloc.h
INC_DIR = ./include
#INC = $(addprefix $(INC_DIR)/, $(INC_FILE_NAME))


#******************************************************************************#
# SOURCES/OBJECT
#******************************************************************************#

SRC_FILE_NAME =		malloc.c               \
					show_alloc_mem.c       \
					free.c                 \
					alloc_meneger_api.c    \
					defragmentation.c      \
					realloc.c              \
					unmmap.c               \
#add .c file -->

SRC_DIR = src
SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILE_NAME))


OBJ_FILE_NAME = $(SRC_FILE_NAME:.c=.o)
OBJ_DIR = .obj
OBJ = $(addprefix $(OBJ_DIR)/, $(OBJ_FILE_NAME))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(F) -o $@ -c $< -I$(INC_DIR) -I$(LIB_INC) -I./src

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

#** rules *********************************************************************#

.PHONY: all clean fclean re bug debug

all: $(NAME) $(SYMLINK)

#$(NAME): $(LIB) $(PF) $(OBJ) $(INC)
$(NAME): $(LIB) $(OBJ) $(INC) src/malloc_internal_api.h
		$(CC) $(F) $(FSHARED) $(OBJ) $(LIB) -o $(NAME) 

$(SYMLINK):
	ln -s $(NAME) $(SYMLINK)
	
$(LIB): libft.all

$(PF): printf.all

clean: libft.clean printf.clean
	rm -rf $(OBJ)

fclean: libft.fclean printf.fclean clean
	rm -f $(NAME) $(SYMLINK) a.out

re: fclean all

clean.light:
	rm -rf $(OBJ) .obj/main.o

rew: clean.light all test
#	/usr/bin/time ./a.out
#	/usr/bin/time -l ./a.out
	/usr/bin/time --verbose ./a.out

test: all
	$(CC) src/main.c -c -o .obj/main.o -I./include
	$(CC) .obj/main.o libft_malloc.so -Wl,-rpath=. -I./include
#	$(CC) .obj/main.o $(NAME) -o a.out 


#** rules * libft *************************************************************#
libft.all:
	make -C $(LIB_PATH)/ all

libft.clean:
	make -C $(LIB_PATH)/ clean

libft.fclean:
	make -C $(LIB_PATH)/ fclean

libft.re:
	make -C $(LIB_PATH)/ re

#** rules * printf *************************************************************#
printf.all:
	make -C $(PF_PATH)/ all

printf.clean:
	make -C $(PF_PATH)/ clean

printf.fclean:
	make -C $(PF_PATH)/ fclean

printf.re:
	make -C $(PF_PATH)/ re



	
#	$(CC) src/main.c libft_malloc.so -Wl,-rpath=. -I./include
	
#	gcc src/main.c -o a.out -I./include #-I$(LIB_INC)# -fPIC libft_malloc.so
	
	#gcc .obj/main.o $(OBJ) $(LIB)  -o a.out 
	


DEBUGBIN = a.debug

# bug:
# 	gcc -g $(F) -o $(NAME) push_swap.c list_push_back.c ps_error.c print_stacks.c make_ss.c make_rr.c make_rrr.c pre_validate.c \
# 	init_var.c read_flags.c operation_push_back.c frst_sort_3_a.c next_sort_3_a.c \
# 	next_sort_3_b.c last_sort_3_b.c add_function.c optimization.c sorting_logic.c  print_operation.c $(INCL)

bug:
	$(CC) -g $(F) -o $(NAME) $(SRC) src/main.c $(LIB) -I$(INC_DIR) -I$(LIB_INC)

debug: bug
	lldb $(NAME)






