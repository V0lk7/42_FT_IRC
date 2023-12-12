#include "Tools.hpp"
#include <vector>
#include <map>
#include <string>

/*#############################################################################*/
static void	DivideParamsType(	std::vector<std::string> &CmdParts,
								std::vector<std::string> &Channel,
								std::vector<std::string> &Key);

static void	AssignChannel(	std::map<std::string, std::string> &Request,
							std::vector<std::string> &Channel,
							std::vector<std::string> &Keys);
/*#############################################################################*/

bool	OrganiseRequest(	std::map<std::string, std::string> &Request,
							std::vector<std::string> &CmdParts)
{
	std::vector<std::string>	Channel;
	std::vector<std::string>	Key;

	CmdParts.erase(CmdParts.begin());
	if (CmdParts.size() == 0)
		return (false);
	DivideParamsType(CmdParts, Channel, Key);
	AssignChannel(Request, Channel, Key);
	return (true);
}

static void	DivideParamsType(	std::vector<std::string> &CmdParts,
								std::vector<std::string> &Channel,
								std::vector<std::string> &Key)
{
	Channel = split(CmdParts[0], ",");
	if (CmdParts.size() > 1)
		Key = split(CmdParts[1], ",");
}

static void	AssignChannel(	std::map<std::string, std::string> &Request,
							std::vector<std::string> &Channel,
							std::vector<std::string> &Keys)
{
	size_t	SizeKeys = Keys.size();

	for (size_t i = 0; i < Channel.size(); i++)
	{
		if (SizeKeys > 1 && SizeKeys - 1 >= i)
			Request[Channel[i]] = Keys[i];
		else if (SizeKeys == 1)
			Request[Channel[i]] = Keys[0];
		else
			Request[Channel[i]] = "";
	}
}
