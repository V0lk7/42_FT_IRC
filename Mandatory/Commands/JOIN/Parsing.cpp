#include "Tools.hpp"
#include "Join.hpp"

static void	DivideParamsType(	std::vector<std::string> &CmdParts,
								std::vector<std::string> &Channel,
								std::vector<std::string> &Key);

static bool	AssignChannel(	std::map<std::string, std::string> &Request,
							std::vector<std::string> &Channel);
static void	AssignKeyToChan(	std::map<std::string, std::string> &Request,
								std::vector<std::string> &Key);

static bool	CheckDelimFormat(std::string const &src, const char &delim);
static bool	VerifyParamsFormat(std::vector<std::string> &CmdParts);


ErrorsFlag	OrganiseRequest(	std::map<std::string, std::string> &Request,
								std::vector<std::string> &CmdParts)
{
	std::vector<std::string>	Channel;
	std::vector<std::string>	Key;

	CmdParts.erase(CmdParts.begin());
	if (CmdParts.size() == 0)
		return (NO_PARAMETERS);
	else if (CmdParts.size() > 2)
		return (TOO_MANY_PARAMETERS);

	if (VerifyParamsFormat(CmdParts) != true)
		return (WRONG_FORMAT);

	DivideParamsType(CmdParts, Channel, Key);
	if (AssignChannel(Request, Channel) != true)
		return (WRONG_FORMAT);

	AssignKeyToChan(Request, Key);
	return (NONE);
}

static void	DivideParamsType(	std::vector<std::string> &CmdParts,
								std::vector<std::string> &Channel,
								std::vector<std::string> &Key)
{
	Channel = split(CmdParts[0], ",");
	if (CmdParts.size() > 1)
		Key = split(CmdParts[1], ",");
}

static bool	AssignChannel(	std::map<std::string, std::string> &Request,
							std::vector<std::string> &Channel)
{
	std::string	tmp;

	for (size_t i = 0; i < Channel.size(); i++)
	{
		tmp = Channel[i];
		if (tmp[0] != '#' && tmp[0] != '&')
			return (false);
		tmp.erase(tmp.begin());
		Request[tmp] = "";
	}
	return (true);
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
