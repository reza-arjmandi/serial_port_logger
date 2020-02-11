#pragma once

#include "test/Random_generator.h"
#include "test/FS_utils.h"

class Virtual_serial_port {

public:
    
    struct Drivers {
        fs::path input;
        fs::path output;
    };

    Virtual_serial_port()
    {
        _drivers.input = 
            fs::current_path() / _random_generator.generate_string(2);
        _drivers.output = 
            fs::current_path() / _random_generator.generate_string(2);
        _FS_utils.create_if_not_exist(_drivers.input);
        _FS_utils.create_if_not_exist(_drivers.output);
    }

    ~Virtual_serial_port()
    {
        fs::remove(_drivers.input);
        fs::remove(_drivers.output);
    }

    Drivers get_drivers() const
    {
        return _drivers;
    }

    void run()
    {
        auto command {
            std::string {"socat pty,link="}
            +  _drivers.input.string() 
            + ",raw,echo=0 pty,link=" 
            + _drivers.output.string()
            + ",raw,echo=0"};
        _process = std::make_shared<bp::child>(command);
    }

    void terminate()
    {
        if(!_process){
            return;
        }

        _process->terminate();
    }

    bool running()
    {
        if(!_process){
            return false;
        }

        return _process->running();
    }

    void wait()
    {
        if(!_process){
            return;
        }
        
        _process->wait();
    }

private:

    Drivers _drivers; 
    Random_generator _random_generator;
    std::shared_ptr<bp::child> _process;
    FS_utils _FS_utils;

};