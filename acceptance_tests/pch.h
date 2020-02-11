#pragma once

#include <string>
#include <memory>
#include <regex>
#include <iostream>
#include <random>
#include <future>
#include <filesystem>
#include <fstream>
#include <string_view>

#include <boost/process.hpp>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;
namespace bp = boost::process;
namespace fs = std::filesystem;