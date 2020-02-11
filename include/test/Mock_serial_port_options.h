#pragma once

struct Mock_serial_port_options {

    enum class Flow_control {
        none,
        software,
        hardware
    };

    enum class Parity {
        none,
        odd,
        even
    };

    enum class Stop_bits {
        one,
        onepointfive,
        two
    };

    unsigned int baud_rate = 0;
    Flow_control flow_control = Flow_control::none;
    Parity parity = Parity::none;
    Stop_bits stop_bits = Stop_bits::one;
    unsigned int character_size = 8;
};