# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/12 03:06:34 by emflynn           #+#    #+#              #
#    Updated: 2026/06/03 13:58:43 by emflynn          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME :=					penguinx

REPO_DIR :=				.

SRC_DIR :=				$(REPO_DIR)/src
OBJ_DIR :=				$(REPO_DIR)/obj
BIN_DIR :=				$(REPO_DIR)/bin
LIB_DIR :=				$(REPO_DIR)/lib
MAIN_DIR :=				$(REPO_DIR)/main
TEST_DIR :=				$(REPO_DIR)/test
MAKE_DIR :=				$(REPO_DIR)/make

MODULES :=				config/lexing \
						config/parsing \
						utils
BIN :=					$(BIN_DIR)/$(subst /,-,$(NAME))
LIBS_BASE :=			$(addprefix lib,$(addsuffix .a,$(subst /,-,$(MODULES))))
LIBS :=					$(addprefix $(LIB_DIR)/,$(LIBS_BASE))
MAIN :=					$(MAIN_DIR)/main.cpp
TEST :=					$(TEST_DIR)/main.sh
DEPS :=					$(BIN:%=%.d)

CXX :=					c++
CXXFLAGS :=				-Wall -Wextra -Werror -std=c++98 -MMD \
						$(addprefix -I$(SRC_DIR)/,$(MODULES))
LDFLAGS :=				-L$(LIB_DIR)
LDLIBS :=				$(addprefix -l,$(subst /,-,$(MODULES)))
AR :=					ar
ARFLAGS :=				-rcs
MAKE :=					make
MKDIR :=				mkdir -p
RMDIR :=				rmdir -p
RM :=					rm -f

$(NAME):				$(BIN)

$(BIN_DIR):
						@$(MKDIR) $@

$(BIN):					$(MAIN) $(LIBS) | $(BIN_DIR)
						@echo "Linking..."
						@$(CXX) $(CXXFLAGS) $(LDFLAGS) $< -o $@ $(LDLIBS)
						@echo "Ready."

$(LIB_DIR):
						@git config --local core.hooksPath .githooks
						@$(MKDIR) $@

$(LIB_DIR)/lib%.a:		| $(LIB_DIR)
						@echo "Making library for module $(subst -,/,$*)..."
						@$(MAKE) -C $(MAKE_DIR)/$(subst -,/,$*)

-include $(DEPS)

all:					$(BIN)

clean:
						@echo "Removing objects and libraries..."
						@$(RM) -R $(OBJ_DIR) $(LIB_DIR)

fclean:					clean
						@echo "Removing binaries..."
						@$(RM) -R $(BIN_DIR)

re:						fclean all

main:					$(BIN)
						@printf "Running $(NAME), "
						@if [ "$(CONFIG_FILE)" ]; then \
							echo "CONFIG_FILE=$(CONFIG_FILE)..."; \
						else \
							echo "CONFIG_FILE unset..."; \
						fi
						@$(BIN) "$(CONFIG_FILE)"

main-%:
						@printf "Running main for module $(subst -,/,$*), "
						@if [ "$(CONFIG_FILE)" ]; then \
							echo "CONFIG_FILE=$(CONFIG_FILE)..."; \
						else \
							echo "CONFIG_FILE unset..."; \
						fi
						@$(MAKE) -C $(MAKE_DIR)/$(subst -,/,$*) main \
							CONFIG_FILE=../../../$(CONFIG_FILE)

test:					$(BIN)
						@echo "Running end-to-end tests..."
						@$(TEST)

test-all:				test
						@for MODULE in $(MODULES); do \
							echo "Running tests for module $$MODULE..."; \
							$(MAKE) -C $(MAKE_DIR)/$$MODULE test; \
						done

test-%:
						@echo "Running tests for module $(subst -,/,$*)..."
						@$(MAKE) -C $(MAKE_DIR)/$(subst -,/,$*) test

valgrind:				$(BIN)
						@echo "Running end-to-end tests with Valgrind..."
						@valgrind $(TEST)

valgrind-all:			valgrind
						@for MODULE in $(MODULES); do \
							echo "Running tests for module $$MODULE with Valgrind..."; \
							$(MAKE) -C $(MAKE_DIR)/$$MODULE valgrind; \
						done

valgrind-%:
						@echo "Running tests for module $(subst -,/,$*) with Valgrind..."
						@$(MAKE) -C $(MAKE_DIR)/$(subst -,/,$*) valgrind

.PHONY:					$(NAME) \
						all \
						clean \
						fclean \
						re \
						main \
						test \
						test-all \
						valgrind \
						valgrind-all
