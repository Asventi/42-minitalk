# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pjarnac <pjarnac@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/19 11:53:22 by pjarnac           #+#    #+#              #
#    Updated: 2024/12/20 20:08:38 by pjarnac          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minitalk

# ================FILES================ #

MAKE_DIR		:=	.make/
BUILD_DIR		:=	$(MAKE_DIR)build_$(shell git branch --show-current)/
BASE_BUILD_DIR	:= normal/

SRC_DIR			=	src/

OBJS_CL			=	$(patsubst %.c, $(BUILD_DIR)%.o, $(SRC_CL))
OBJS_SRV		=	$(patsubst %.c, $(BUILD_DIR)%.o, $(SRC_SRV))

DEPS			=	$(patsubst %.c, $(BUILD_DIR)%.d, $(SRC_CL))
DEPS			=	$(patsubst %.c, $(BUILD_DIR)%.d, $(SRC_SRV))

# ================ROOT================= #

SRC_CL 		=	client.c \


SRC_SRV		=	server.c \


# ==========LIBS / INCLUDES============ #

LIBS_DIR	=	lib/
LIBS_PATH	=	libft/libft.a
LIBS_PATH	:=	$(addprefix $(LIBS_DIR), $(LIBS_PATH))
LIBS		=	$(patsubst lib%.a, %, $(notdir $(LIBS_PATH)))
SYS_LIBS	=
SYS_LIBS	:=	$(addprefix -l, $(SYS_LIBS))

INCS_DIR	=	includes/
INCLUDES	=	$(INCS_DIR) \
				$(addsuffix $(INCS_DIR), $(dir $(LIBS_PATH))) \
				$(dir $(LIBS_PATH))

# ===============CONFIGS=============== #

CC			=	cc
CFLAGS		+=	-Wall -Wextra -Werror
CPPFLAGS	+=	$(addprefix -I, $(INCLUDES)) \
			-MMD -MP

LDFLAGS		+=	$(addprefix -L, $(dir $(LIBS_PATH)))
LDLIBS		+=	$(addprefix -l, $(LIBS)) $(SYS_LIBS)

AR			=	ar
ARFLAGS		=	-rcs

MAKEFLAGS	+=	--no-print-directory

# ================MODES================ #

MODES		:= debug fsanitize optimize full-optimize bonus

MODE_TRACE	:= $(BUILD_DIR).mode_trace
LAST_MODE	:= $(shell cat $(MODE_TRACE) 2>/dev/null)

MODE ?=

ifneq ($(MODE), )
	BUILD_DIR := $(BUILD_DIR)$(MODE)/
else
	BUILD_DIR := $(BUILD_DIR)$(BASE_BUILD_DIR)
endif

ifeq ($(MODE), debug)
	CFLAGS = -g3 -Og
else ifeq ($(MODE), fsanitize)
	CFLAGS = -g3 -Og -fsanitize=address
else ifeq ($(MODE), optimize)
	CFLAGS += -O3
else ifeq ($(MODE), full-optimize)
	CFLAGS += -Ofast
else ifeq ($(MODE), bonus)
	SRC := $(SRC_BONUS) $(filter-out $(patsubst %_bonus.c, %.c, $(SRC_BONUS)), $(SRC))
else ifneq ($(MODE),)
	ERROR = MODE
endif

ifneq ($(LAST_MODE), $(MODE))
$(NAME): force
endif

# ================TARGETS============== #

.PHONY: all
all: client server

client: $(LIBS_PATH) $(OBJS_CL)
	@echo $(MODE) > $(MODE_TRACE)
	$(CC) $(CFLAGS) $(OBJS_CL) $(LDFLAGS) $(LDLIBS) -o $@

server: $(LIBS_PATH) $(OBJS_SRV)
	@echo $(MODE) > $(MODE_TRACE)
	$(CC) $(CFLAGS) $(OBJS_SRV) $(LDFLAGS) $(LDLIBS) -o $@

$(BUILD_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(LIBS_PATH): force
	@$(MAKE) -C $(@D)

.PHONY: $(MODES)
$(MODES):
	@$(MAKE) MODE=$@

.PHONY: clean
clean:
	-for lib in $(dir $(LIBS_PATH)); do $(MAKE) -s -C $$lib $@; done
	rm -rf $(MAKE_DIR)

.PHONY: fclean
fclean:
	-for lib in $(dir $(LIBS_PATH)); do $(MAKE) -s -C $$lib $@; done
	rm -rf $(MAKE_DIR) client server

.PHONY: re
re: fclean
	@$(MAKE)

# ================MISC================= #

.PHONY: print-%
print-%:
	@echo $(patsubst print-%,%,$@)=
	@echo $($(patsubst print-%,%,$@))

.PHONY: force
force:

.PHONY: norminette
norminette:
	@norminette $(addprefix $(SRC_DIR), $(SRC)) $(INCS_DIR)

-include $(DEPS)

.DEFAULT_GOAL := all
