NAME :=					server

REPO_DIR :=				../..

SRC_DIR :=				$(REPO_DIR)/src
OBJ_DIR :=				$(REPO_DIR)/obj
BIN_DIR :=				$(REPO_DIR)/bin
LIB_DIR :=				$(REPO_DIR)/lib
MAIN_DIR :=				$(REPO_DIR)/main
TEST_DIR :=				$(REPO_DIR)/test
MAKE_DIR :=				$(REPO_DIR)/make

SRCS_BASE :=			Server.cpp \
						Client.cpp \
						ServerConfig.cpp
SRCS :=					$(addprefix $(SRC_DIR)/$(NAME)/,$(SRCS_BASE)) \
						$(SRC_DIR)/RequestParser.cpp
OBJS :=					$(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
BIN :=					$(BIN_DIR)/$(NAME)
LIB :=					$(LIB_DIR)/lib$(NAME).a
MAIN :=					$(MAIN_DIR)/$(NAME).cpp
TEST :=					$(TEST_DIR)/$(NAME).sh
DEPS :=					$(OBJS:%.o=%.d) $(BIN:%=%.d)

MODULES :=				utils
MODULE_LIBS_BASE :=		$(addprefix lib,$(addsuffix .a,$(subst /,-,$(MODULES))))
MODULE_LIBS :=			$(addprefix $(LIB_DIR)/,$(MODULE_LIBS_BASE))

CXX :=					c++
CXXFLAGS :=				-Wall -Wextra -Werror -std=c++98 -MMD \
						-I$(SRC_DIR)/$(NAME) -I$(SRC_DIR) \
						$(addprefix -I$(SRC_DIR)/,$(MODULES))
LDFLAGS :=				-L$(LIB_DIR)
LDLIBS :=				-l$(NAME) $(addprefix -l,$(subst /,-,$(MODULES)))
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

$(OBJ_DIR):
						@$(MKDIR) $@

$(OBJ_DIR)/%.o:			$(SRC_DIR)/%.cpp | $(OBJ_DIR)
						@$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR):
						@$(MKDIR) $@

$(BIN):					$(MAIN) $(LIB) $(MODULE_LIBS) | $(BIN_DIR)
						@$(CXX) $(CXXFLAGS) $(LDFLAGS) $< -o $@ $(LDLIBS)

$(LIB_DIR):
						@$(MKDIR) $@

$(LIB):					$(OBJS) | $(LIB_DIR)
						@$(AR) $(ARFLAGS) $@ $^

$(LIB_DIR)/lib%.a:		| $(LIB_DIR)
						@$(MAKE) -C $(MAKE_DIR)/$(subst -,/,$*)

-include $(DEPS)

all:					$(BIN)

clean:
						@$(RM) $(OBJS) $(OBJS:%.o=%.d)
						@$(RM) $(LIB)
						@$(RMDIR) $(OBJ_DIR)/$(NAME) $(OBJ_DIR) $(LIB_DIR) || true

fclean:					clean
						@$(RM) $(BIN) $(BIN).d
						@$(RMDIR) $(BIN_DIR) || true

re:						fclean all

main:					$(BIN)
						@$(BIN)

test:					;

valgrind:				$(BIN)
						@valgrind $(BIN)

.PHONY:					$(NAME) \
						all \
						clean \
						fclean \
						re \
						main \
						test \
						valgrind
