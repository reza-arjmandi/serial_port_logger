#include "serial_port_logger/Serial_port_config_file_parser.h"
#include "serial_port_logger/Serial_port_config_file_parser_exception.h"

#include "test/Random_serial_port_config_generator.h"
#include "test/FS_utils.h"

class Serial_port_config_file_parser_tests : public Test {

public:

    void SetUp() override
    {
        _FS_utils.remove_if_exists(_config_file);

        _serial_port_options_matcher = 
        [&](const std::vector<Serial_port_config>& devices_info)
        {
            bool result {true};

            if(devices_info.size() != 
                _container.get_child("serial_ports").size()){
                return false;
            }

            int idx {0};
            for(const auto& elem : _container.get_child("serial_ports")){
                auto prefix = "serial_ports." + elem.first;
                result = result 
                && elem.first == devices_info[idx].name
                && _container.get<fs::path>(prefix +  ".log_file") == 
                    devices_info[idx].log_file 
                && _container.get<fs::path>(prefix +  ".driver") == 
                    devices_info[idx].driver
                &&  Boost_serial_port_options::Baud_rate_type(
                    _container.get<unsigned int>(
                        prefix + ".baud_rate")).value() == 
                        devices_info[idx].options.baud_rate.value()
                && str_to_flow_control(_container.get<std::string>(
                        prefix + ".flow_control")).value() == 
                        devices_info[idx].options.flow_control.value()
                && str_to_parity(_container.get<std::string>(
                        prefix + ".parity")).value() == 
                        devices_info[idx].options.parity.value()
                && float_to_stop_bits(_container.get<float>(
                        prefix + ".stop_bits")).value() == 
                        devices_info[idx].options.stop_bits.value()
                && Boost_serial_port_options::Character_size_type(
                    _container.get<unsigned int>(
                        prefix + ".character_size")).value() == 
                        devices_info[idx].options.character_size.value();
                idx++;
            }
            return result;
        };

    }

    void TearDown() override
    {
        _FS_utils.remove_if_exists(_config_file);
    }

    void generate_config_file()
    {
        _config_generator.generate_config_file(_config_file);
    }

    void generate_config_file_with_invalid_baud_rate()
    {
        _config_generator.generate_config_file(_config_file, false);
    }

    void generate_config_file_with_invalid_flow_control()
    {
        _config_generator.generate_config_file(
            _config_file, true, true, false);
    }

    void generate_config_file_with_invalid_parity()
    {
        _config_generator.generate_config_file(_config_file, true, false);
    }

    void generate_config_file_with_invalid_stop_bits()
    {
        _config_generator.generate_config_file(
            _config_file, true, true, true, false);
    }

    void generate_config_file_with_invalid_character_size()
    {
        _config_generator.generate_config_file(
            _config_file, true, true, true, true, false);
    }

    void read_config_file()
    {
        boost::property_tree::read_json(_config_file, _container);
    }

    Boost_serial_port_options::Flow_control_type str_to_flow_control(
        const std::string& str)
    {
        if(str == "software"){
            return Boost_serial_port_options::Flow_control_type(
                Boost_serial_port_options::Flow_control_type::type::software);
        }
        else if(str == "hardware"){
            return Boost_serial_port_options::Flow_control_type(
                Boost_serial_port_options::Flow_control_type::type::hardware);
        }
        else{
            return Boost_serial_port_options::Flow_control_type(
                Boost_serial_port_options::Flow_control_type::type::none);
        }
    } 

    Boost_serial_port_options::Parity_type str_to_parity(
        const std::string& str)
    {
        if(str == "odd"){
            return Boost_serial_port_options::Parity_type(
                Boost_serial_port_options::Parity_type::type::odd);
        }
        else if(str == "even"){
            return Boost_serial_port_options::Parity_type(
                Boost_serial_port_options::Parity_type::type::even);
        }
        else{
            return Boost_serial_port_options::Parity_type(
                Boost_serial_port_options::Parity_type::type::none);
        }
    } 

    Boost_serial_port_options::Stop_bits_type float_to_stop_bits(float value)
    {
        if(value == 1.5){
            return Boost_serial_port_options::Stop_bits_type(
                Boost_serial_port_options::Stop_bits_type::type::onepointfive);
        }
        else if(value == 2.0){
            return Boost_serial_port_options::Stop_bits_type(
                Boost_serial_port_options::Stop_bits_type::type::two);
        }
        else{
            return Boost_serial_port_options::Stop_bits_type(
                Boost_serial_port_options::Stop_bits_type::type::one);
        }
    } 

    std::function<
        bool(const std::vector<Serial_port_config>& devices_info)>
            _serial_port_options_matcher;
    Serial_port_config_file_parser _parser;
    boost::property_tree::ptree _container;
    Random_generator _random_generator;
    fs::path _config_file {_random_generator.generate_string(10)};
    Random_serial_port_config_generator _config_generator;
    FS_utils _FS_utils;

};

TEST_F(Serial_port_config_file_parser_tests, 
When_get_devices_info_is_called_\
Then_list_of_devices_info_must_return)
{
    generate_config_file();
    read_config_file();
    auto parsed_configs {_parser.get_devices_info(_config_file)};
    ASSERT_THAT(parsed_configs, Truly(_serial_port_options_matcher));
}

TEST_F(Serial_port_config_file_parser_tests, 
When_get_devices_info_is_called_with_a_config_file_that_has_invalid_baud_rate\
Then_an_exception_must_be_thrown)
{
    generate_config_file_with_invalid_baud_rate();
    read_config_file();

    try{
        auto parsed_configs {_parser.get_devices_info(_config_file)};
        ASSERT_TRUE(false);
    }
    catch(Serial_port_config_file_parser_exception& ex){
        ASSERT_TRUE(true);
    }
}

TEST_F(Serial_port_config_file_parser_tests, 
When_get_devices_info_is_called_\
with_a_config_file_that_has_invalid_flow_control\
Then_an_exception_must_be_thrown)
{
    generate_config_file_with_invalid_flow_control();
    read_config_file();

    try{
        auto parsed_configs {_parser.get_devices_info(_config_file)};
        ASSERT_TRUE(false);
    }
    catch(Serial_port_config_file_parser_exception& ex){
        ASSERT_TRUE(true);
    }
}

TEST_F(Serial_port_config_file_parser_tests, 
When_get_devices_info_is_called_with_a_config_file_that_has_invalid_parity\
Then_an_exception_must_be_thrown)
{
    generate_config_file_with_invalid_parity();
    read_config_file();

    try{
        auto parsed_configs {_parser.get_devices_info(_config_file)};
        ASSERT_TRUE(false);
    }
    catch(Serial_port_config_file_parser_exception& ex){
        ASSERT_TRUE(true);
    }
}

TEST_F(Serial_port_config_file_parser_tests, 
When_get_devices_info_is_called_with_a_config_file_that_has_invalid_stop_bits\
Then_an_exception_must_be_thrown)
{
    generate_config_file_with_invalid_stop_bits();
    read_config_file();

    try{
        auto parsed_configs {_parser.get_devices_info(_config_file)};
        ASSERT_TRUE(false);
    }
    catch(Serial_port_config_file_parser_exception& ex){
        ASSERT_TRUE(true);
    }
}

TEST_F(Serial_port_config_file_parser_tests, 
When_get_devices_info_is_called_\
with_a_config_file_that_has_invalid_character_size\
Then_an_exception_must_be_thrown)
{
    generate_config_file_with_invalid_character_size();
    read_config_file();

    try{
        auto parsed_configs {_parser.get_devices_info(_config_file)};
        ASSERT_TRUE(false);
    }
    catch(Serial_port_config_file_parser_exception& ex){
        ASSERT_TRUE(true);
    }
}