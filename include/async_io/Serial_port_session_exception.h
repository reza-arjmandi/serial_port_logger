#pragma once

class Serial_port_session_exception : public std::exception {

public:

    Serial_port_session_exception(
        const std::string& device,const std::string& message);

    const char* what() const noexcept final;

    std::string device() const;

private:

    std::string _device;
    std::string _message;

};