ifndef BONUS
NAME		:=	ircserv

OBJ_DIR		:=	.obj

SRC_DIR		:=	Mandatory

INCLUDES	:= 	$(SRC_DIR)/Tools/			\
				$(SRC_DIR)/Commands/		\
				$(SRC_DIR)/Parsing/			\
				$(SRC_DIR)/Class/Client/	\
				$(SRC_DIR)/Class/Server/	\
				$(SRC_DIR)/Class/Channel/	\
				$(SRC_DIR)/Commands/KICK/	\

SRCS		:=	Core/main.cpp 					\
				Core/Arguments_Parsing.cpp		\
				Core/Server_Initialization.cpp	\
				Core/Process_Server.cpp			\

SRCS		+=	Parsing/Parsing.cpp 				\

SRCS		+=	Tools/cSplit.cpp 					\

SRCS		+=	Class/Client/Client.cpp				\
				Class/Channel/Channel.cpp			\
				Class/Server/Server.cpp

SRCS		+=	Commands/TOPIC/Topic.cpp			\

SRCS		+=	Commands/JOIN/Join.cpp				\
				Commands/JOIN/Join_parsing.cpp		\
				Commands/JOIN/Join_utilities.cpp	\

SRCS		+=	Commands/KICK/Kick.cpp				\
				Commands/KICK/ParsingKick.cpp		\

SRCS		+=	Commands/MODE/Mode.cpp				\
				Commands/MODE/Mode_parsing.cpp		\
				Commands/MODE/Mode_utilities.cpp	\

SRCS		+=	Commands/INVITE/Invite.cpp			\

else
NAME		:= Bot

OBJ_DIR		:=	.obj_bonus

SRC_DIR		:=	Bonus

INCLUDES	:= 	$(SRC_DIR)/

SRCS		:=	main.cpp \
				Bot.cpp
endif
