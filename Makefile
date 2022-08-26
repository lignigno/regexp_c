# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lignigno <lignign@student.21-school.ru>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/07 02:38:20 by lignigno          #+#    #+#              #
#    Updated: 2022/08/24 10:09:36 by lignigno         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all cleen fcleen re check

# _____________________________________________________________________VARIABLES

NAME					:= rt

SRC_DIR					:=	srcs
OBJ_DIR					:=	obj
FLAGS					:=	-O2 -Wall -Wextra -Werror
VPATH					:=	$(shell find $(SRC_DIR) -type d)
INCLUDE					:=	$(addprefix -I , $(VPATH))

SRC						:=	$(wildcard $(addsuffix /*.c, $(VPATH)))
OBJ						:=	$(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.c=.o)))
DEP						:=	$(wildcard $(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.c=.d))))

# _______________________________________________________________________TARGETS

all						:	check $(OBJ_DIR) $(NAME)

check :
	@echo $(INCLUDE)
	@echo $(VPATH)

$(NAME)					:	$(OBJ)
							@gcc $(OBJ) -o $@
							@echo "COMMAND + V"
							@printf "$(shell pwd)/$@ 65.43.75.134 \"(((25[0-5])|(2[0-4][0-9])|(1[0-9]{2})|([1-9]?[0-9])).){3}((25[0-5])|(2[0-4][0-9])|(1[0-9]{2})|([1-9]?[0-9]))\"" | pbcopy

$(OBJ_DIR)/%.o			:	%.c
							gcc $(FLAGS) $(INCLUDE) -MD -c $< -o $@

$(OBJ_DIR)				:
							@mkdir -p $(OBJ_DIR)/

include $(DEP)

# ________________________________________________________________CLEANS TARGETS

clean					:
							@rm -rf $(OBJ_DIR)
							@echo "object files for $(NAME) have been deleted"

fclean					:	clean
							@rm -f $(NAME)
							@echo "$(NAME) have been deleted"

re						:	fclean all