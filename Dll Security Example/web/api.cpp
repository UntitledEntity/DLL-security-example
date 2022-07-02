#include "api.hpp"

#pragma comment(lib, "rpcrt4.lib")

static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

static std::string req(std::string data)
{
	CURL* curl = curl_easy_init();

	if (!curl)
		return ("NULL");

	std::string to_return;

	curl_easy_setopt(curl, CURLOPT_URL, STR("APILINKHERE"));

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &to_return);

	auto code = curl_easy_perform(curl);

	if (code != CURLE_OK)
		MessageBoxA(0, curl_easy_strerror(code), 0, MB_ICONERROR);

	curl_easy_cleanup(curl);

	return to_return;
}


std::string api::str2hex(const std::string& in) {
	std::stringstream ss;

	ss << std::hex << std::setfill('0');
	for (size_t i = 0; in.length() > i; ++i) {
		ss << std::setw(2) << static_cast<unsigned int>(static_cast<unsigned char>(in[i]));
	}

	return ss.str();
}

std::string api::hex2str(const std::string & in) {
	std::string output;

	size_t cnt = in.length() / 2;

	for (size_t i = 0; cnt > i; ++i) {
		uint32_t s = 0;
		std::stringstream ss;
		ss << std::hex << in.substr(i * 2, 2);
		ss >> s;

		output.push_back(static_cast<unsigned char>(s));
	}

	return output;
}

std::string sha256(const std::string str)
{
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, str.c_str(), str.size());
	SHA256_Final(hash, &sha256);
	
	std::stringstream ss;
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
	{
		ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
	}
	return ss.str();
}

// PLEASE add better encryption if you want to use this for publication purposes
void api::on_init()
{
	auto data =
		STR("&type=") + std::string(STR("inject")) +
		STR("&gamever=") + std::to_string(0) /* You could replace this with the game version, then update the modern game version in the API */ +
		STR("&user=") + str2hex(user) +
		STR("&hwid=") + str2hex(sha256(client_hwid));
	
	std::string resp = req(data);
	resp = hex2str(resp);
	if (!resp.data())
		ExitProcess(rand() % RAND_MAX);
		
	auto json = response_decoder.parse(resp);

	data.clear();

	std::strcpy(subscription, std::string(json[("sub")]).c_str());
	std::strcpy(expiry, std::string(json[("expiry")]).c_str());

	// remove this for release
	MessageBoxA(NULL, std::string(json[("message")]).c_str(), "", MB_OK);

	if (!json[("success")])
		ExitProcess(rand() % RAND_MAX);
}