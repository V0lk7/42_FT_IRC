#include "Mode.hpp"
#include "Error_code.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"

#include <cstdlib>
#include <cerrno>

int	ChangeInvitOnlyMode(Channel *ChanPtr, CmdNode const &Mode)
{
	bool	ActualState = ChanPtr->GetMode(INVITE_ONLY_SET);

	if (ActualState == Mode.Op)
		return (SILENT);
	ChanPtr->SetMode(INVITE_ONLY_SET, Mode.Op);
	return (INVITONLY_CHANGED);
}

int ChangeTopicMode(Channel *ChanPtr, CmdNode const &Mode)
{
	bool	ActualState = ChanPtr->GetMode(TOPIC_CHANGE_SET);

	if (ActualState == Mode.Op)
		return (SILENT);
	ChanPtr->SetMode(TOPIC_CHANGE_SET, Mode.Op);
	return (TOPIC_CHANGED);
}

int	ChangePasswordMode(Channel *ChanPtr, CmdNode const &Mode)
{
	bool	ActualState = ChanPtr->GetMode(PASSWORD_SET);

	if (Mode.Param.empty() == true && Mode.Op == true)
		return (ERR_NEEDMOREPARAMS);
	else if (ActualState == Mode.Op)
		return (ERR_KEYSET);
	ChanPtr->SetMode(PASSWORD_SET, Mode.Op);
	if (Mode.Op == true)
		ChanPtr->SetPassword(Mode.Param);
	return (PASSWORD_CHANGED);
}

int	ChangeOperatorPrivilege(Server &serv, Channel *ChanPtr, CmdNode const &Mode)
{
	Client	*CliPtr;

	if (Mode.Param.empty() == true)
		return (ERR_NEEDMOREPARAMS);
	CliPtr = serv.GetClient(Mode.Param);
	if (CliPtr == NULL)
		return (ERR_NOSUCHNICK);
	else if (ChanPtr->IsClientOperator(*CliPtr) == Mode.Op)
		return (SILENT);
	ChanPtr->ModifyClientRights(*CliPtr, Mode.Op);
	return (CLIENTRIGHT_CHANGED);
}

int ChangeLimitMode(Channel *ChanPtr, CmdNode const &Mode)
{
	int		limit = 0;

	if (Mode.Param.empty() == true && Mode.Op == true)
		return (ERR_NEEDMOREPARAMS);
	else if (Mode.Param.find_first_not_of("0123456789") != std::string::npos)
		return (ERR_NEEDMOREPARAMS);
	if (Mode.Op == true)
		limit = static_cast<int>(strtod(Mode.Param.c_str(), NULL));
	if (errno == ERANGE || limit == 0)
		return (OUT_OF_RANGE);
	ChanPtr->SetLimitUsers(limit);
	return (LIMIT_CHANGED);
}
