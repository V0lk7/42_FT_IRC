#ifndef KICK_HPP
# define KICK_HPP

# include <string>

class Channel;
class Client;

typedef enum Err { NONE, EMPTY, NOTARGET, CONTINUE, NOCHANNEL, NORIGHT, HIMSELF, ERR_PARAMS } Err;

enum Err
parseCmd( const std::string& cmd, Channel* channel, Client& client );

#endif 
