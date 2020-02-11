#include "serial_port_logger/Serial_port_config_file_parser.h"
#include "serial_port_logger/Serial_port_config_file_parser_exception.h"

std::vector<Serial_port_config> 
    Serial_port_config_file_parser::get_devices_info(
        const fs::path& config_file)
{
    boost::property_tree::read_json(config_file, _container);
        
    std::vector<Serial_port_config> result;
    for(const auto& elem : _container.get_child("serial_ports")){
        Serial_port_config device_info;
        auto prefix = "serial_ports." + elem.first;
        device_info.log_file = 
            _container.get<fs::path>(prefix +  ".log_file");
        device_info.driver = 
            _container.get<fs::path>(prefix +  ".driver");
        
        device_info.options.baud_rate = get_baud_rate(prefix);

        device_info.options.flow_control = 
            str_to_flow_control(_container.get<std::string>(
                    prefix + ".flow_control"));
        device_info.options.parity = 
            str_to_parity(_container.get<std::string>(
                    prefix + ".parity"));
        device_info.options.stop_bits = 
            float_to_stop_bits(_container.get<float>(
                prefix + ".stop_bits"));
        device_info.options.character_size = get_character_size(prefix);
        result.push_back(device_info);
    }
    return result;
}

Boost_serial_port_options::Baud_rate_type 
    Serial_port_config_file_parser::get_baud_rate(
        const std::string& prefix) const
{   
    try{
        auto baud_rate_value {
            _container.get<unsigned int>(prefix + ".baud_rate")};
        return Boost_serial_port_options::Baud_rate_type {baud_rate_value};
    }
    catch(boost::property_tree::ptree_error& err){
        throw Serial_port_config_file_parser_exception(
            "baud_rate must be unsigned integer");
    }
}

Boost_serial_port_options::Character_size_type 
    Serial_port_config_file_parser::get_character_size(
        const std::string& prefix) const
{   
    try{
        auto character_size_value {
            _container.get<unsigned int>(prefix + ".character_size")};
        return Boost_serial_port_options::Character_size_type {
            character_size_value};
    }
    catch(boost::property_tree::ptree_error& err){
        throw Serial_port_config_file_parser_exception(
            "character_size must be unsigned integer [equal to 8]");
    }
}

Boost_serial_port_options::Flow_control_type 
    Serial_port_config_file_parser::str_to_flow_control(
        const std::string& str)
{
    if(str == "none"){
        return Boost_serial_port_options::Flow_control_type(
            Boost_serial_port_options::Flow_control_type::type::none);
    }
    else if(str == "software"){
        return Boost_serial_port_options::Flow_control_type(
            Boost_serial_port_options::Flow_control_type::type::software);
    }
    else if(str == "hardware"){
        return Boost_serial_port_options::Flow_control_type(
            Boost_serial_port_options::Flow_control_type::type::hardware);
    }
    else{
        throw Serial_port_config_file_parser_exception(
            "flow_control must be one of [none, software, hardware]");
    }
    
}

Boost_serial_port_options::Parity_type 
    Serial_port_config_file_parser::str_to_parity(
        const std::string& str)
{
    if(str == "none"){
        return Boost_serial_port_options::Parity_type(
            Boost_serial_port_options::Parity_type::type::none);
    }
    else if(str == "odd"){
        return Boost_serial_port_options::Parity_type(
            Boost_serial_port_options::Parity_type::type::odd);
    }
    else if(str == "even"){
        return Boost_serial_port_options::Parity_type(
            Boost_serial_port_options::Parity_type::type::even);
    }
    else{
        throw Serial_port_config_file_parser_exception(
            "parity must be one of [none, odd, even]");
    }
}

Boost_serial_port_options::Stop_bits_type 
    Serial_port_config_file_parser::float_to_stop_bits(float value)
{
    if(value == 1.0){
        return Boost_serial_port_options::Stop_bits_type(
            Boost_serial_port_options::Stop_bits_type::type::one);
    }
    else if(value == 1.5){
        return Boost_serial_port_options::Stop_bits_type(
            Boost_serial_port_options::Stop_bits_type::type::onepointfive);
    }
    else if(value == 2.0){
        return Boost_serial_port_options::Stop_bits_type(
            Boost_serial_port_options::Stop_bits_type::type::two);
    }
    else{
        throw Serial_port_config_file_parser_exception(
            "stop_bits must be one of [1.0, 1.5, 2.0]");
    }
    
}