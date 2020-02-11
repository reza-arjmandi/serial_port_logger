# serial_port_logger

serial_port_logger is a smart single threaded console application that can log multiple serial port devices simultaneously.  
It's smart because if one of serial devices disconnect while it's running, or one of serial devices is not available when it run, it automatically reconnect to serial device after 10 seconds.  
It's can be compiled for windows, linux, mac and raspberry pi.  
Also you can download DEB packages for linux and raspberry pi in the [packages directory](packages).

You can run serial_port_logger like this:

```c++
serial_port_logger --CONFIG_FILE=config.json
```

And in the `config.json` file you can add multiple serial devices.  
Example:

```json
{
    "serial_ports": {
        "device1": {
            "log_file": "device1_log_file.txt",
            "driver": "/dev/ttyUSB0",
            "baud_rate": "9600",
            "flow_control": "none",
            "parity": "none",
            "stop_bits": "1",
            "character_size": "8"
        },
        "device2": {
            "log_file": "device2_log_file.txt",
            "driver": "/dev/ttyUSB1",
            "baud_rate": "9600",
            "flow_control": "none",
            "parity": "none",
            "stop_bits": "1",
            "character_size": "8"
        }
    }
}
```
