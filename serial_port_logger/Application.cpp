#include "Application.h"

Application::Application(const fs::path& config_file)
{
    auto configs {_config_file_parser.get_devices_info(config_file)};
    _serial_port_loggers.reserve(configs.size());
    for(const auto& config : configs) {
        _serial_port_loggers.emplace_back(config, _dependency_injector);
    }
}

int Application::run()
{
    Signals_handler_type signals_handler {
        _dependency_injector.get_io_context(),
        _signal_set_factory, 
        _serial_port_loggers, 
        _signals};
    signals_handler.async_wait();
    _dependency_injector.get_io_context().run();
    return 1;
}
