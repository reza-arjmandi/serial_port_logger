#pragma once

#include "config.h"

class Logger {

public:

    struct {
        std::filesystem::path config_file;
    } Command_line_options;

    void run(std::string argument="")
    {
        _process = std::make_shared<bp::child>(
            create_command(argument), bp::std_out >  _std_out);
    }

    std::string read_a_line_from_stdout()
    {
        std::string line;
        std::getline(_std_out, line);
        return line;
    }

    void wait()
    {
        _process->wait();
    }

    void terminate()
    {
        _process->terminate();
    }

    bool running()
    {
        return _process->running();
    }

private:

    std::string create_command(const std::string& argument)
    {
        auto command {
            (fs::path(SERIAL_PORT_LOGGER_DIR) /
             fs::path(_process_name)).string() 
            + std::string {" --CONFIG_FILE="}
            + Command_line_options.config_file.string()
            + " " + argument 
            };
        return command;
    }

    std::shared_ptr<bp::child> _process;
    bp::ipstream _std_out;
    std::string _process_name {"serial_port_logger"};

};