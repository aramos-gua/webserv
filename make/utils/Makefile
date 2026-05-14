# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/14 18:32:29 by emflynn           #+#    #+#              #
#    Updated: 2026/05/14 21:32:02 by emflynn          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME :=					utils

REPO_DIR :=				../..

SRC_DIR :=				$(REPO_DIR)/src
OBJ_DIR :=				$(REPO_DIR)/obj
BIN_DIR :=				$(REPO_DIR)/bin
LIB_DIR :=				$(REPO_DIR)/lib
MAIN_DIR :=				$(REPO_DIR)/main
TEST_DIR :=				$(REPO_DIR)/test
MAKE_DIR :=				$(REPO_DIR)/make

SRCS_BASE :=			StringBase.cpp
SRCS :=					$(addprefix $(SRC_DIR)/$(NAME)/,$(SRCS_BASE))
OBJS :=					$(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
LIB :=					$(LIB_DIR)/lib$(subst /,-,$(NAME)).a
DEPS :=					$(OBJS:%.o=%.d)

CXX :=					c++
CXXFLAGS :=				-Wall -Wextra -Werror -std=c++98 -MMD \
						-I$(SRC_DIR)/$(NAME)
LDFLAGS :=				-L$(LIB_DIR)
LDLIBS :=				-l$(subst /,-,$(NAME))
AR :=					ar
ARFLAGS :=				-rcs
MAKE :=					make
MKDIR :=				mkdir -p
RMDIR :=				rmdir -p
RM :=					rm -f

$(NAME):				$(LIB)

$(OBJ_DIR)/$(NAME):
						@$(MKDIR) $@

$(OBJ_DIR)/$(NAME)/%.o:	$(SRC_DIR)/$(NAME)/%.cpp | $(OBJ_DIR)/$(NAME)
						@$(CXX) $(CXXFLAGS) -c $< -o $@

$(LIB_DIR):
						@$(MKDIR) $@

$(LIB):					$(OBJS) | $(LIB_DIR)
						@$(AR) $(ARFLAGS) $@ $^

-include $(DEPS)

all:					$(LIB)

clean:
						@$(RM) -R $(OBJ_DIR)/$(NAME)
						@$(RM) $(LIB)
						@$(RMDIR) $(OBJ_DIR) $(LIB_DIR) || true

fclean:					clean

re:						fclean all

main:					;

test:					;

valgrind:				;

.PHONY:					$(NAME) \
						all \
						clean \
						fclean \
						re \
						main \
						test \
						valgrind
