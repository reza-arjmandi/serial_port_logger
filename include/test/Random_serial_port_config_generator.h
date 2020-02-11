#include "test/Random_generator.h"
#include "serial_port_logger/Serial_port_config.h"

class Random_serial_port_config_generator {

public:

    using Options_type = Serial_port_config::Serial_port_options_type;


    void generate_config_file(
        const fs::path& config_file,
        bool valid_baud_rate = true,
        bool valid_parity = true,
        bool valid_flow_control = true,
        bool valid_stop_bits = true,
        bool valid_character_size = true)
    {
        auto number_of_devices {_random_generator.generate_integer(1, 1000)};
        
        boost::property_tree::ptree container;
        for(auto i = 0; i < number_of_devices; i++){
            auto device {generate_random_str()};
            auto prefix {"serial_ports." + device};
            container.add(prefix + ".log_file", generate_random_path());
            container.add(prefix + ".driver", generate_random_path());
            container.add(prefix + ".baud_rate", 
                generate_baud_rate_str(valid_baud_rate));
            container.add(prefix + ".parity", generate_parity_str(valid_parity));
            container.add(prefix + ".flow_control", 
                generate_flow_control_str(valid_flow_control));
            container.add(prefix + ".stop_bits", 
                generate_stop_bits_float(valid_stop_bits));
            container.add(prefix + ".character_size", 
                generate_character_size_str(valid_character_size));
        }
        boost::property_tree::write_json(config_file, container);
    }

    Serial_port_config generate_config()
    {
        Serial_port_config result;
        result.log_file = generate_random_path();
        result.driver = generate_random_path();
        result.options = generate_serial_port_options();
        return result;
    }

private:

    Options_type generate_serial_port_options()
    {
        Options_type result;
        result.baud_rate = generate_baud_rate();
        result.character_size = generate_character_size();
        result.flow_control = generate_flow_control();
        result.parity = generate_parity();
        result.stop_bits = generate_stop_bits();
        return result;
    }

    int generate_random_integer()
    {
        return _random_generator.generate_integer(1000,100000);
    }

    std::string generate_baud_rate_str(bool valid = true)
    {
        if(valid){
            return std::to_string(generate_random_integer());
        }
        return generate_random_str();
    }

    Options_type::Baud_rate_type generate_baud_rate()
    {
        return Options_type::Baud_rate_type(generate_random_integer());
    }

    std::string generate_character_size_str(bool valid = true)
    {
        if(valid){
            return std::to_string(8);
        }
        return generate_random_str();
    }

    Options_type::Character_size_type generate_character_size()
    {
        return Options_type::Character_size_type();
    }

    std::string generate_random_str()
    {
        return _random_generator.generate_string(
            _random_generator.generate_integer(1, 1000));
    }

    fs::path generate_random_path()
    {
        return _random_generator.generate_path(
                    _random_generator.generate_integer(1,10));
    }

    std::string generate_flow_control_str(bool valid = true)
    {
        if(!valid){
            return generate_random_str();
        }

        switch (_random_generator.generate_integer(0, 3)){
        case 0:
            return {"none"};
            break;
        case 1:
            return {"software"};
            break;
        case 2:
            return {"hardware"};
            break;
        default:
            return {"none"};
            break;
        }
    }

    Options_type::Flow_control_type generate_flow_control()
    {
        switch (_random_generator.generate_integer(0, 3)){
        case 0:
            return Options_type::Flow_control_type(
                Options_type::Flow_control_type::none);
            break;
        case 1:
            return Options_type::Flow_control_type(
                Options_type::Flow_control_type::software);
            break;
        case 2:
            return Options_type::Flow_control_type(
                    Options_type::Flow_control_type::hardware);
            break;
        default:
            return Options_type::Flow_control_type(
                Options_type::Flow_control_type::none);
            break;
        }
    }

    std::string generate_parity_str(bool valid = true)
    {
        if(!valid){
            return generate_random_str();
        }

        switch (_random_generator.generate_integer(0, 3)){
        case 0:
            return {"none"};
            break;
        case 1:
            return {"odd"};
            break;
        case 2:
            return {"even"};
            break;
        default:
            return {"none"};
            break;
        }
    }

    Options_type::Parity_type generate_parity()
    {
        switch (_random_generator.generate_integer(0, 3)){
        case 0:
            return Options_type::Parity_type(Options_type::Parity_type::none);
            break;
        case 1:
            return Options_type::Parity_type(Options_type::Parity_type::odd);
            break;
        case 2:
            return Options_type::Parity_type(Options_type::Parity_type::even);
            break;
        default:
            return Options_type::Parity_type(Options_type::Parity_type::none);
            break;
        }
    }

    float generate_stop_bits_float(bool valid = true)
    {
        if(!valid){
            return 6.5;
        }

        switch (_random_generator.generate_integer(0, 3)){
        case 0:
            return 1.0;
            break;
        case 1:
            return 1.5;
            break;
        case 2:
            return 2.0;
            break;
        default:
            return 1.0;
            break;
        }
    }

    Options_type::Stop_bits_type generate_stop_bits()
    {
        switch (_random_generator.generate_integer(0, 3)){
        case 0:
            return Options_type::Stop_bits_type(
                Options_type::Stop_bits_type::one);
            break;
        case 1:
            return Options_type::Stop_bits_type(
                    Options_type::Stop_bits_type::onepointfive);
            break;
        case 2:
            return Options_type::Stop_bits_type(
                        Options_type::Stop_bits_type::two);
            break;
        default:
            return Options_type::Stop_bits_type(
                Options_type::Stop_bits_type::one);
            break;
        }
    }

    Random_generator _random_generator;

};