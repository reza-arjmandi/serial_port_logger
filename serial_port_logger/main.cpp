#include "Application.h"

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
                "set the json config file");
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

        Application app {variable_maps["CONFIG_FILE"].as<fs::path>()};
        return app.run();
}