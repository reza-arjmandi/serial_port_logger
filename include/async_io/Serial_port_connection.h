#pragma once

template <
typename Serial_port_factory_type, 
typename Session_factory_type>
class Serial_port_connection {

public:

    Serial_port_connection(
        const std::string& device,
        Serial_port_factory_type& serial_port_factory,
        const typename 
            Serial_port_factory_type::Serial_port_options_type& options,
        Session_factory_type& session_factory):
        _device {device},
        _options {options},
        _session_factory {session_factory}
    {
        _serial_port = serial_port_factory.create();
    }

    template<typename Error_code_type>
    void is_connected(Error_code_type& ec)
    {
        _serial_port->open(_device, ec);
        if(!ec){
            _serial_port->close(ec);
        }
    }

    template<typename Error_code_type>
    void start_async(Error_code_type& ec)
    {
        if(_serial_port->is_open()){
            return;
        }

        _serial_port->open(_device, ec);
        if(ec){
            return;
        }

        if(!set_options(ec)){
            return;
        }

        _session = _session_factory.create(_serial_port);
        _session->start();
    }

    template<typename Error_code_type>
    void stop(Error_code_type& ec)
    {
        if(_session){
            _session->stop();
        }
        if(_serial_port->is_open()){
            _serial_port->close(ec);
        }
    }

private:

    bool set_options(auto& ec)
    {
        _serial_port->set_option(_options.baud_rate, ec);
        if(ec){
            return false;
        }
        _serial_port->set_option(_options.flow_control, ec);
        if(ec){
            return false;
        }
        _serial_port->set_option(_options.parity, ec);
        if(ec){
            return false;
        }
        _serial_port->set_option(_options.stop_bits, ec);
        if(ec){
            return false;
        }
        _serial_port->set_option(_options.character_size, ec);
        if(ec){
            return false;
        }
        return true;
    }

    typename Serial_port_factory_type::Serial_port_type _serial_port {nullptr};
    typename Session_factory_type::Session_type _session {nullptr};
    typename Serial_port_factory_type::Serial_port_options_type _options;
    Session_factory_type& _session_factory;
    std::string _device {""};

};