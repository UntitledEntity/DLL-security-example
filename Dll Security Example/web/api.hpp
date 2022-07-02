#pragma once
#include "../includes.hpp"

namespace api
{
	void on_init();

	inline char user[255];
	inline char subscription[255];
	inline char expiry[255];
	inline char client_hwid[255];

	std::string str2hex(const std::string& in);
	std::string hex2str(const std::string& in);

}