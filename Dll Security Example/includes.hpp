#pragma once
#include <windows.h>
#include <iostream>
#include <cstdint>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <array>
#include <fstream>
#include <istream>
#include <unordered_map>
#include <intrin.h>
#include <filesystem>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <atlsecurity.h> 
#include <strsafe.h> 

#include <openssl/sha.h>

#include "xorstr.hpp"

inline nlohmann::json response_decoder;