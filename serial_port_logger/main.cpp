#include "Application.h"
#include "serial_port_logger/Log_type.h"

bool is_variable_exist(
        const po::variables_map& variable_map, const std::string& var)
{
        if (!variable_map.count(var)){
                return false;
        }
        return true;
}

int main(int argc, char ** argv)
{
        auto descriptions {po::options_description()};
        descriptions.add_options()
                ("help", "Print all commands and usage")
                ("CONFIG_FILE", po::value<fs::path>(), 
                "set the json config file")
                ("IS-CONNECTED", "check serial port connection")
                ("SYNC-EVERY", po::value<std::chrono::milliseconds>(), 
                "set synchronization time in milli seconds");
        auto variable_maps {po::variables_map()};
        po::store(po::parse_command_line(argc, argv, descriptions), 
                variable_maps);
        po::notify(variable_maps);
        if (variable_maps.count("help")){
                std::cout << descriptions << std::endl;
        }
        for(const auto& var : 
                {"CONFIG_FILE"}){
                if(!is_variable_exist(variable_maps, var)){
                        std::cerr << "The --" << var << " must be set";
                        return -1;
                }
        }

        Log_type log_type = Log_type::LOG_DATA_TO_FILE;
        if(variable_maps.count("IS-CONNECTED")) {
                log_type = Log_type::IS_CONEECTED;
        }

        std::chrono::milliseconds sync_time = std::chrono::milliseconds(0);
        if(variable_maps.count("SYNC-EVERY")) {
                sync_time = variable_maps[
                        "SYNC-EVERY"].as<std::chrono::milliseconds>();
        }

        Application app {
                log_type, 
                variable_maps["CONFIG_FILE"].as<fs::path>(),
                sync_time};
        return app.run();
}