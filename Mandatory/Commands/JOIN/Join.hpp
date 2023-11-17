#ifndef JOIN_HPP
# define JOIN_HPP

# include <map>
# include <vector>
# include <string>

typedef enum ErrorsFlag {
	NONE,
	NO_PARAMETERS,
	TOO_MANY_PARAMETERS,
	WRONG_FORMAT
} ErrorsFlag;

ErrorsFlag	OrganiseRequest(	std::map<std::string, std::string> &Request,
								std::vector<std::string> &CmdParts);

#endif
