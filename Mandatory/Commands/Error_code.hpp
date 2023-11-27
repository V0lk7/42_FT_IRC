#ifndef ERROR_CODE_HPP
# define ERROR_CODE_HPP

typedef enum ErrorCode {
	
} ErrorCode;

typedef enum ReplyCode {
	
} ReplyCode;

PASS{
	ERR_NEEDMOREPARAMS 461
	ERR_ALREADYREGISTERED 462 //When a User try to register, but has already did it
}

NICK{
	ERR_NONICKNAMEGIVEN	431 //when using command need a NICKNAME param but not given
	ERR_NICKNAMEINUSE 433 // When a NICK command is used to change into anther
					  // nickname but it is alread in use
	ERR_ERRONEUSNICKNAME 432 // When a nickname has wrong character
}

USER{
	ERR_NEEDMOREPARAMS 461 //when client has not given enough parameter
	ERR_ALREADYREGISTERED 462 //When a User try to register, but has already did it
}

JOIN{
	ERR_NEEDMOREPARAMS 461
	ERR_INVITEONLYCHAN 473 //when a clien try to join a channel but has not be invited.
	ERR_NOSUCHCHANNEL 403 //channel not found.
	ERR_CHANNELISFULL 471 //when a client try to join a full channel.
	ERR_BADCHANNELKEY 475 //when a client try to join a channel with/without the good key.
	ERR_NOTREGISTERED 451 // when a client isn't registered yet and try to do a command.
}

MODE{
	ERR_NEEDMOREPARAMS 461
	ERR_CHANOPRIVSNEEDED 481 // when a client want to use an
							 // operator command but isn't himself an op.
	ERR_NOTONCHANNEL 442 // when a client execute a command on a channel where he isn't on it.
	ERR_UNKNOWNMODE 472 // Mode inconnu
	ERR_NOSUCHCHANNEL 403
	ERR_KEYSET	467 //When define a key already set
	ERR_NOSUCHNICK 401// when user targeted doesn't exist
	
	RPL_CHANNELMODEIS 324 //when sending the mode active on the channel
}

#endif
