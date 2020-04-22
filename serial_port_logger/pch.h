#include <boost/program_options.hpp>
#include <boost/asio.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/asio/signal_set.hpp>

#include <iostream>
#include <filesystem>
#include <fstream>
#include <csignal>

namespace fs = std::filesystem;
namespace po = boost::program_options;