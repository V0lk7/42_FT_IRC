#include <map>
#include <vector>
#include <string>
#include <stdexcept>

#include "Tools.hpp"

static void	DivideParamsType(	std::vector<std::string> &CmdParts,
								std::vector<std::string> &Channel,
								std::vector<std::string> &Key);

static void	AssignChannel(	std::map<std::string, std::string> &Request,
							std::vector<std::string> &Channel);
static void	AssignKeyToChan(	std::map<std::string, std::string> &Request,
								std::vector<std::string> &Key);

static bool	CheckDelimFormat(std::string const &src, const char &delim);
static bool	VerifyParamsFormat(std::vector<std::string> &CmdParts);


int	OrganiseRequest(	std::map<std::string, std::string> &Request,
						std::vector<std::string> &CmdParts)
{
	std::vector<std::string>	Channel;
	std::vector<std::string>	Key;

	CmdParts.erase(CmdParts.begin());

	if (CmdParts.size() == 0)
		throw std::runtime_error("NO_PARAMETERS");
	else if (CmdParts.size() > 2)
		throw std::runtime_error("TOO_MANY_PARAMETERS");

	if (VerifyParamsFormat(CmdParts) != true)
		throw std::runtime_error("WRONG_FORMAT");

	DivideParamsType(CmdParts, Channel, Key);
	AssignChannel(Request, Channel);
	AssignKeyToChan(Request, Key);
	return (0);
}

static void	DivideParamsType(	std::vector<std::string> &CmdParts,
								std::vector<std::string> &Channel,
								std::vector<std::string> &Key)
{
	csplit(Channel, CmdParts[0], ",");
	if (CmdParts.size() > 1)
		csplit(Key, CmdParts[1], ",");
}

static void	AssignChannel(	std::map<std::string, std::string> &Request,
							std::vector<std::string> &Channel)
{
	for (size_t i = 0; i < Channel.size(); i++)
		Request[Channel[i]] = "";
}

static void	AssignKeyToChan(	std::map<std::string, std::string> &Request,
								std::vector<std::string> &Key)
{
	std::map<std::string, std::string>::iterator	It = Request.begin();
	std::vector<std::string>::iterator				ItKey = Key.begin();

	while (It != Request.end())
	{
		if (ItKey != Key.end())
		{
			It->second = *ItKey;
			if (Key.size() > 1)
				ItKey++;
		}
		It++;
	}
}

static bool	VerifyParamsFormat(std::vector<std::string> &CmdParts)
{
	if (CheckDelimFormat(CmdParts[0], ',') == false)
		return (false);
	if (CmdParts.size() > 1)
	{
		if (CheckDelimFormat(CmdParts[1], ',') == false)
			return (false);
	}
	return (true);
}

static bool	CheckDelimFormat(std::string const &src, const char &delim)
{
	size_t	pos = 0;

	if (src[0] == delim || src[src.size() - 1] == delim)
		return (false);
	while (pos < src.size() || pos != std::string::npos)
	{
		pos = src.find(delim, pos);
		if (pos != std::string::npos)
		{
			if (src[pos + 1] == delim)
				return (false);
			else
				pos++;
		}
	}
	return (true);
}
