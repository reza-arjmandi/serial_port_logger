#include "Application.h"

Application::Application(const Log_type& log_type, const fs::path& config_file)
    : _log_type {log_type}
{
    auto configs {_config_file_parser.get_devices_info(config_file)};
    _serial_port_loggers.reserve(configs.size());
    for(const auto& config : configs) {
        _serial_port_loggers.emplace_back(
            log_type, config, _dependency_injector);
    }
}

int Application::run()
{
    if(_log_type == Log_type::IS_CONEECTED) {
        return 0;
    }

    Signals_handler_type signals_handler {
        _dependency_injector.get_io_context(),
        _signal_set_factory, 
        _serial_port_loggers, 
        _signals};
    signals_handler.async_wait();
    _dependency_injector.get_io_context().run();
    return 0;
}
