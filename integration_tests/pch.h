#pragma once

#include <memory>
#include <filesystem>
#include <random>
#include <exception>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/asio.hpp>

using namespace testing;
namespace fs = std::filesystem;
