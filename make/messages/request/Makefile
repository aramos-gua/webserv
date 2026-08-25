# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/25 22:02:47 by emflynn           #+#    #+#              #
#    Updated: 2026/08/25 22:03:18 by emflynn          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME :=					messages/request

REPO_DIR :=				../../..

SRC_DIR :=				$(REPO_DIR)/src
OBJ_DIR :=				$(REPO_DIR)/obj
BIN_DIR :=				$(REPO_DIR)/bin
LIB_DIR :=				$(REPO_DIR)/lib
MAIN_DIR :=				$(REPO_DIR)/main
TEST_DIR :=				$(REPO_DIR)/test
MAKE_DIR :=				$(REPO_DIR)/make

SRCS_BASE :=			HttpRequestParser.cpp
SRCS :=					$(addprefix $(SRC_DIR)/$(NAME)/,$(SRCS_BASE))
OBJS :=					$(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
BIN :=					$(BIN_DIR)/$(subst /,-,$(NAME))
LIB :=					$(LIB_DIR)/lib$(subst /,-,$(NAME)).a
MAIN :=					$(MAIN_DIR)/$(NAME)/main.cpp
TEST :=					$(TEST_DIR)/$(NAME)/test.sh
DEPS :=					$(OBJS:%.o=%.d) $(BIN:%=%.d)

MODULES :=				utils
MODULE_LIBS_BASE :=		$(addprefix lib,$(addsuffix .a,$(subst /,-,$(MODULES))))
MODULE_LIBS :=			$(addprefix $(LIB_DIR)/,$(MODULE_LIBS_BASE))

CXX :=					c++
CXXFLAGS :=				-Wall -Wextra -Werror -std=c++98 -MMD \
						$(addprefix -I$(SRC_DIR)/,$(NAME) $(MODULES))
LDFLAGS :=				-L$(LIB_DIR)
LDLIBS :=				$(addprefix -l,$(subst /,-,$(NAME) $(MODULES)))
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

$(BIN_DIR):
						@$(MKDIR) $@

$(BIN):					$(MAIN) $(LIB) $(MODULE_LIBS) | $(BIN_DIR)
						@echo "Linking..."
						@$(CXX) $(CXXFLAGS) $(LDFLAGS) $< -o $@ $(LDLIBS)
						@echo "Ready."

$(LIB_DIR):
						@$(MKDIR) $@

$(LIB):					$(OBJS) | $(LIB_DIR)
						@echo "Rerunning archival..."
						@$(AR) $(ARFLAGS) $@ $^
						@echo "Done."

$(LIB_DIR)/lib%.a:		FORCE | $(LIB_DIR)
						@echo "Checking if library for module $(subst -,/,$*) needs remaking..."
						@$(MAKE) -C $(MAKE_DIR)/$(subst -,/,$*)

-include $(DEPS)

all:					$(BIN)

clean:
						@echo "Removing objects and libraries..."
						@$(RM) -R $(OBJ_DIR)/$(NAME)
						@$(RM) $(LIB)
						@$(RMDIR) $(OBJ_DIR) $(LIB_DIR) || true
						@echo "Done."

fclean:					clean
						@echo "Removing binaries..."
						@$(RM) $(BIN)
						@$(RMDIR) $(BIN_DIR) || true
						@echo "Done."

re:						fclean all

main:					$(BIN)
						@$(BIN) $(CONFIG_FILE)

test:					$(BIN)
						@$(TEST)

valgrind:				$(BIN)
						@valgrind $(TEST)

.PHONY:					$(NAME) \
						FORCE \
						all \
						clean \
						fclean \
						re \
						main \
						test \
						valgrind
