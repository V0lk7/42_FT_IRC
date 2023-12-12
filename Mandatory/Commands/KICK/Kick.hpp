#ifndef KICK_HPP
# define KICK_HPP

# include <string>

class Channel;
class Client;

// ########################################################################## //
// #_TODO___________________________________________________________________# //
// #-> better handling error msg: 1.                                        # //
// #-> need hard testing                                                    # //
// #-> need to implement checking right of client kickmaker                 # //
// #-> need str is : KICK [&#]CHANNEL target                                # //
// ########################################################################## //

typedef enum Err { NONE, EMPTY, NOTARGET, CONTINUE, NOCHANNEL, NORIGHT } Err;

enum Err
parseCmd( const std::string& cmd, Channel* channel, Client& client );

#endif 
