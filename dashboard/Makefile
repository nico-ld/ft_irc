# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nico <nico@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/13 08:07:33 by nile-dai          #+#    #+#              #
#    Updated: 2026/08/28 18:03:05 by nico             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		:= Dashboard
CXXFLAGS	:= -Wall -Wextra -Werror -g -std=c++98
CXX			:= c++
INC			:= -Iincludes

# ~~ AINSI code ~~
RESET		:= \033[0m

BOLD		:= \033[1m

RED			:= \033[91m
GREEN		:= \033[92m
YELLOW		:= \033[93m

# ~~ Sources dir ~~
SRC_DIR		:= src/
DASH_DIR	:= $(SRC_DIR)dashboard/
REND_DIR	:= $(SRC_DIR)render/
SECT_DIR	:= $(SRC_DIR)sections/
HELP_DIR	:= $(SRC_DIR)helper/
# ~~ Sources files ~~

DASHBOARD	:= \
				$(DASH_DIR)Dashboard.cpp \
				$(DASH_DIR)DashboardElemList.cpp

HELPER		:= $(HELP_DIR)helper.cpp

RENDER		:= \
				$(REND_DIR)Render.cpp \
				$(REND_DIR)RenderHelper.cpp \
				$(REND_DIR)RenderColumn.cpp \
				$(REND_DIR)RenderTitles.cpp

SECTIONS	:= \
				$(SECT_DIR)SectionCore.cpp \
				$(SECT_DIR)SectionParser.cpp

SRC			:= \
				$(DASHBOARD) \
				$(HELPER) \
				$(RENDER) \
				$(SECTIONS) \
				$(SRC_DIR)main.cpp

# ~~ Objects ~~
OBJ_DIR		:= obj/
OBJS		:= $(patsubst $(SRC_DIR)%.cpp,$(OBJ_DIR)%.o,$(SRC))

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

all: _init_srcs $(NAME)

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
	@$(CXX) $(CXXFLAGS) $(INC) $(OBJS) -o $@

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

re: fclean all

.PHONY: all clean fclean re
