#pragma once

class Serial_port_config_file_parser_exception : public std::exception {

public:

    Serial_port_config_file_parser_exception(const std::string& message);
    const char* what() const noexcept override;

private:

    std::string _message;

};