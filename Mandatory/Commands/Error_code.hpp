#ifndef ERROR_CODE_HPP
# define ERROR_CODE_HPP

# define ERR_NOSUCHNICK			401
# define ERR_NOSUCHCHANNEL		403
# define ERR_NOTEXTTOSEND		412
# define ERR_NONICKNAMEGIVEN	431
# define ERR_ERRONEUSNICKNAME	432
# define ERR_NICKNAMEINUSE		433
# define ERR_RECIPIENT			441
# define ERR_NOTONCHANNEL		442
# define ERR_USERONCHANNEL		443
# define ERR_NOTREGISTERED		451
# define ERR_NEEDMOREPARAMS		461
# define ERR_ALREADYREGISTERED	462
# define ERR_PASSWDMISTMATCH	464
# define ERR_KEYSET				467
# define ERR_CHANNELISFULL		471
# define ERR_UNKNOWNMODE		472
# define ERR_INVITEONLYCHAN		473
# define ERR_BADCHANNELKEY		475
# define ERR_CHANOPRIVSNEEDED	481

# define RPL_CHANNELMODEIS		324
# define RPL_NOTOPIC			331
# define RPL_TOPIC				332
# define RPL_INVITING			341
# define RPL_NAMREPLY			353
# define RPL_ENDOFNAMES			366

/*#######################################################################################
##########------PASS-----##########
ERR_NEEDMOREPARAMS		461
ERR_ALREADYREGISTERED	462 //When a User try to register, but has already did it
ERR_PASSWDMISTMATCH		464 // when the password is wrong

##########-----NICK-----##########
ERR_NONICKNAMEGIVEN		431 //when using command need a NICKNAME param but not given
ERR_NICKNAMEINUSE		433 // When a NICK is already in use
ERR_ERRONEUSNICKNAME	432 // When a nickname has wrong character
ERR_ALREADYREGISTERED	462 //When a User try to register, but has already did it

##########-----USER-----##########
ERR_NEEDMOREPARAMS		461 //when client has not given enough parameter
ERR_ALREADYREGISTERED	462 //When a User try to register, but has already did it

##########-----JOIN-----##########
ERR_NOTREGISTERED	451 // when a client isn't registered yet and try to do a command.
ERR_NEEDMOREPARAMS	461
ERR_INVITEONLYCHAN	473 //when a client try to join a channel but has not be invited.
ERR_NOSUCHCHANNEL	403 //channel not found.
ERR_CHANNELISFULL	471 //when a client try to join a full channel.
ERR_BADCHANNELKEY	475 //when a client try to join a channel with/without the good key.
ERR_USERONCHANNEL	443 // when a user is already in the channel

RPL_TOPIC			332 // if a channel topic is set
RPL_NOTOPIC			331 // if a channel topic isn't set

##########-----MODE-----##########
ERR_NOTREGISTERED		451 // when a client isn't registered yet and try to do a command.
ERR_NEEDMOREPARAMS		461
ERR_CHANOPRIVSNEEDED	481 // when a client want to use an
							// operator command but isn't himself an op.
ERR_NOSUCHCHANNEL		403
ERR_NOTONCHANNEL		442 // when a client execute a command on a
							// channel where he isn't on it.
ERR_UNKNOWNMODE			472 // Mode inconnu
ERR_KEYSET				467 // When define a key already set
ERR_NOSUCHNICK			401 // when user targeted doesn't exist

RPL_CHANNELMODEIS		324 //when sending the mode active on the channel

##########-----TOPIC-----##########
ERR_NOTREGISTERED		451 // when a client isn't registered yet and try to do a command.
ERR_NEEDMOREPARAMS		461
ERR_CHANOPRIVSNEEDED	481
ERR_NOTONCHANNEL		442

RPL_TOPIC 332 // if a channel topic is set
RPL_NOTOPIC 331 // if a channel topic isn't set


#########-----INVITE-----##########
ERR_NOTREGISTERED		451 // when a client isn't registered yet and try to do a command.
ERR_NEEDMOREPARAMS		461
ERR_NOTONCHANNEL		442
ERR_NOSUCHCHANNEL		403 //channel not found.
ERR_CHANOPRIVSNEEDED	481
ERR_NOSUCHNICK			401 // when user targeted doesn't exist
ERR_USERONCHANNEL		443 // when a user is already in the channel

RPL_INVITING			341 // sending it to the client inviting the other to notice
								//	that the invit has be registered and send to the target

##########-----KICK-----##########
ERR_NOTREGISTERED		451 // when a client isn't registered yet and try to do a command.
ERR_NEEDMOREPARAMS		461
ERR_NOTONCHANNEL		442
ERR_NOSUCHCHANNEL		403 //channel not found.
ERR_CHANOPRIVSNEEDED	481


##########------PRIVMSG------##########

ERR_NOTREGISTERED	451 // when a client isn't registered yet and try to do a command.
ERR_NORECIPIENT		441 //Pas de destinataire donné
ERR_NOTEXTTOSEND	412 //Pas de texte a envoyer
ERR_NOSUCHNICK		401 // when user targeted doesn't exist
ERR_NOSUCHCHANNEL	403 //channel not found.
#######################################################################################*/

#endif
