#ifndef TOOLS_HPP
# define TOOLS_HPP

#include <iostream>
#include <string>
#include <vector>


std::vector<std::string>
split(const std::string& s, std::string delimiter);

int	Receive(int Fd, std::string &Buffer, size_t Size);

#endif
