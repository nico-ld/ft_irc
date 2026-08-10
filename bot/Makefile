NAME		:= RouxBot
CXXFLAGS	:= -Wall -Wextra -Werror -g -std=c++98
CXX			:= c++
INC			:= -Iincludes

# ~~ Source directories ~~
SRC_DIR		:= src/
LOG_DIR		:= $(SRC_DIR)dashboard/
CLT_DIR		:= $(SRC_DIR)client/
GAME_DIR	:= $(SRC_DIR)game/
PARS_DIR	:= ../src/parser/Parser/

# ~~ Source files ~~
LOG			:= \
				$(LOG_DIR)dashboard.cpp \
				$(LOG_DIR)dashboardInit.cpp

PARSER		:= \
				$(PARS_DIR)Parser.cpp \
				$(PARS_DIR)ParserInit.cpp

CLIENT		:= \
				$(CLT_DIR)botToServer.cpp

GAME		:= \
				$(GAME_DIR)Game.cpp \
				$(GAME_DIR)Uno/Uno.cpp

SRC			:= \
				$(CLIENT) \
				$(LOG) \
				$(GAME) \
				$(SRC_DIR)testing_main.cpp
# 				$(SRC_DIR)main.cpp

OBJ_DIR		:= obj/
OBJ			:= $(patsubst $(SRC_DIR)%.cpp,$(OBJ_DIR)%.o,$(SRC))

# ~~ ANSI code ~~
RESET		:= \033[0m

BOLD		:= \033[1m

RED			:= \033[91m
GREEN		:= \033[92m
YELLOW		:= \033[93m

# ~~ Loading bar config ~~
TOTAL_SRCS		:= $(words $(SRC))
BAR_WIDTH		:= 30
COUNTER_FILE	:= /tmp/.make_counter_$(shell echo $$PPID)
TOTAL_FILE		:= /tmp/.make_total_$(shell echo $$PPID)

define progress_bar
	@{ \
		TOTAL=$$(cat $(TOTAL_FILE) 2>/dev/null || echo 1); \
		LABEL="$(1)"; \
		COUNT=$$(cat $(COUNTER_FILE) 2>/dev/null || echo 0); \
		COUNT=$$((COUNT + 1)); \
		echo $$COUNT > $(COUNTER_FILE); \
		FILLED=$$((COUNT * $(BAR_WIDTH) / TOTAL)); \
		EMPTY=$$(($(BAR_WIDTH) - FILLED)); \
		BAR=""; \
		for i in $$(seq 1 $$FILLED); do BAR="$$BAR█"; done; \
		for i in $$(seq 1 $$EMPTY);  do BAR="$$BAR░"; done; \
		PCT=$$((COUNT * 100 / TOTAL)); \
		if [ $$COUNT -eq $$TOTAL ]; then \
			printf "\r $(GREEN)[$$BAR]$(RESET) %3d%%  $(GREEN)✔$(RESET)\033[K\n" $$PCT; \
		else \
			printf "\r $(YELLOW)[$$BAR]$(RESET) %3d%% \033[K" $$PCT; \
		fi; \
	}
endef

# ~~ Rules ~~

all: _init_srcs $(NAME)

_init_srcs:
	@echo 0 > $(COUNTER_FILE)
	@$(MAKE) -n $(OBJ) 2>/dev/null | grep -c '^c++' > $(TOTAL_FILE) || echo 0 > $(TOTAL_FILE)
	@TOTAL=$$(cat $(TOTAL_FILE)); \
	if [ "$$TOTAL" -eq 0 ]; then \
		printf "$(BOLD) You already got the latest version$(GREEN) ✔\n$(RESET)"; \
	else \
		printf "$(BOLD) Compiling $$TOTAL file(s) of $(NAME)...\n$(RESET)"; \
	fi

$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) $(INC) $(OBJ) -o $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) -c $(CXXFLAGS) $(INC) $< -o $@
	$(call progress_bar,$TOTAL_SRCS,$<)

clean:
	@rm -rf $(OBJ_DIR)
	@printf "$(RED) ✘$(RESET)  Objects removed\n"

fclean: clean
	@rm -f $(NAME)
	@printf "$(RED) ✘$(RESET)  $(NAME) removed\n"
	@rm -f bot.log

re: fclean all

.PHONY: all clean fclean re
