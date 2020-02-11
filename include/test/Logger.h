#pragma once

#include "config.h"

class Logger {

public:

    struct {
        std::filesystem::path config_file;
    } Command_line_options;

    void run()
    {
        _process = std::make_shared<bp::child>(create_command());
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

    std::string create_command()
    {
        auto command {
            (fs::path(SERIAL_PORT_LOGGER_DIR) /
             fs::path(_process_name)).string() 
            + std::string {" --CONFIG_FILE="}
            + Command_line_options.config_file.string()};
        return command;
    }

    std::shared_ptr<bp::child> _process;
    std::string _process_name {"serial_port_logger"};

};