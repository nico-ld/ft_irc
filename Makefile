# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nico <nico@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/13 08:07:33 by nile-dai          #+#    #+#              #
#    Updated: 2026/09/02 11:57:46 by nico             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		:= ircserv
CXXFLAGS	:= -Wall -Wextra -Werror -g -std=c++98
CXX			:= c++
INC			:= -Iincludes -Isrc/network

# ~~ AINSI code ~~
RESET		:= \033[0m

BOLD		:= \033[1m

RED			:= \033[91m
GREEN		:= \033[92m
YELLOW		:= \033[93m

# ~~ Sources dir ~~
SRC_DIR		:= src/
BUF_DIR		:= $(SRC_DIR)buffer/
NET_DIR		:= $(SRC_DIR)network/
CMD_DIR		:= $(SRC_DIR)commands/
REP_DIR		:= $(SRC_DIR)Replies/
PARS_DIR	:= $(SRC_DIR)parser/Parser/
DISP_DIR	:= $(SRC_DIR)parser/dispatcher/
CORE_DIR	:= $(SRC_DIR)core/

# ~~ Sources files ~~
COMMANDS	:= \
				$(CMD_DIR)Join.cpp \
				$(CMD_DIR)Kick.cpp \
				$(CMD_DIR)Topic.cpp \
				$(CMD_DIR)Part.cpp \
				$(CMD_DIR)Invite.cpp \
				$(CMD_DIR)Mode.cpp \
				$(CMD_DIR)ModeHelper.cpp

CORE		:= \
				$(CORE_DIR)Channel.cpp \
				$(CORE_DIR)Server.cpp \
				$(CORE_DIR)ServerHelper.cpp \
				$(CORE_DIR)User.cpp

NETWORK		:= \
				$(NET_DIR)NetworkBuffer.cpp \
				$(NET_DIR)NetworkUtils.cpp

PARSER		:= \
				$(PARS_DIR)Parser.cpp \
				$(PARS_DIR)ParserInit.cpp \
				$(PARS_DIR)ParserHelper.cpp

DISPATCHER	:= \
				$(DISP_DIR)dispatcher.cpp \
				$(DISP_DIR)dispatchChanCmd.cpp \
				$(DISP_DIR)dispatchMessage.cpp \
				$(DISP_DIR)dispatchUser.cpp

REPLIES		:= $(REP_DIR)Replies.cpp

SRC			:= \
				$(CORE) \
				$(NETWORK) \
				$(PARSER) \
				$(REPLIES) \
				$(COMMANDS) \
				$(DISPATCHER) \
				$(SRC_DIR)main.cpp \
				$(SRC_DIR)Message.cpp

# ~~ Objects ~~
OBJ_DIR		:= obj/
OBJS		:= $(patsubst $(SRC_DIR)%.cpp,$(OBJ_DIR)%.o,$(SRC))

DASH_DIR	:= dashboard/
DASHBOARD	:= $(DASH_DIR)Dashboard.a

# ~~ Loading bar config ~~
TOTAL_SRCS		:= $(words $(SRCS))
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

all: $(DASHBOARD) _init_srcs $(NAME)

_init_srcs:
	@echo 0 > $(COUNTER_FILE)
	@$(MAKE) -n $(OBJS) 2>/dev/null | grep -c '^c++' > $(TOTAL_FILE) || echo 0 > $(TOTAL_FILE)
	@TOTAL=$$(cat $(TOTAL_FILE)); \
	if [ "$$TOTAL" -eq 0 ]; then \
		printf "$(BOLD) You already got the latest version$(GREEN) ✔\n$(RESET)"; \
	else \
		printf "$(BOLD) Compiling $$TOTAL file(s) of $(NAME)...\n$(RESET)"; \
	fi

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(INC) $(OBJS) $(DASHBOARD) -o $@

$(DASHBOARD):
	@$(MAKE) --no-print-directory -C $(DASH_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) -c $(CXXFLAGS) $(INC) $< -o $@
	$(call progress_bar,$TOTAL_SRCS,$<)

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) --no-print-directory -C $(DASH_DIR) clean
	@printf "$(RED) ✘$(RESET)  Objects removed\n"

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) --no-print-directory -C $(DASH_DIR) fclean
	@rm -f ircserv.log
	@printf "$(RED) ✘$(RESET)  $(NAME) removed\n"

re: fclean all

.PHONY: all clean fclean re
