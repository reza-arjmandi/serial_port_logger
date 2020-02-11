#pragma once

#include <mutex>
#include <condition_variable>
#include <functional>
#include <memory>
#include <string>
#include <future>
#include <random>
#include <array>
#include <iostream>
#include <set>
#include <fstream>
#include <cstdarg>
#include <filesystem>

#include <boost/system/error_code.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio.hpp>

namespace fs = std::filesystem;