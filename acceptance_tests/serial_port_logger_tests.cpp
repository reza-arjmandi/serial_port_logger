#include "test/Random_generator.h"
#include "test/FS_utils.h"
#include "test/Virtual_serial_port.h"
#include "test/Logger.h"

class serial_port_logger_tests : public Test {

public:

    void SetUp() override
    {
        _gps_drivers = _gps_virtual_port.get_drivers();
        _compass_drivers = _compass_virtual_port.get_drivers();
        _FS_utils.remove_if_exists(_gps_report_file);
        _FS_utils.remove_if_exists(_compass_report_file);
    }

    void TearDown() override
    {
        _FS_utils.remove_if_exists(_gps_report_file);
        _FS_utils.remove_if_exists(_compass_report_file);
        _FS_utils.remove_if_exists(_config_file);
        terminate_logger_gps_compass();
    }

    void report(std::string_view message)
	{
		std::cout << "[  report  ] " << message << std::endl;
	}

    std::string generate_random_data()
    {
        auto number_of_lines = _random_generator.generate_integer(
            _minimum_number_of_lines, _maximum_number_of_lines);
        std::string result;
        for(auto i = 0; i < number_of_lines; i++) {
            auto data_size {_random_generator.generate_integer(
                _minimum_random_sentence_size, _maximum_random_sentence_size)};
            result += _random_generator.generate_string(data_size) + '\n';
        } 
        return result;
    }

    void write_data(const std::string& device, std::string_view data)
    {
        std::ofstream {device} << data;
    }

    std::string read_data(const fs::path& path_)
    {
        std::string line;
        std::string result;
        std::ifstream file {path_};

        while(std::getline(file, line)){
            result += line + '\n';
        }
        return result;
    }

    void wait_to_process_stable()
    {
        std::this_thread::sleep_for(_delay_duration_after_run_a_process);
    }

    void run_gps()
    {
        _gps_virtual_port.run();
        wait_to_process_stable();
    }

    void run_compass()
    {
        _compass_virtual_port.run();
        wait_to_process_stable();
    }

    void run_gps_compass_logger()
    {
        run_gps();
        run_compass();
        run_logger();
    }

    void run_gps_compass()
    {
        run_gps();
        run_compass();
    }

    void write_data_to_gps()
    {
        auto _random_data {generate_random_data()};
        _gps_random_data += _random_data;
        write_data(_gps_drivers.output, _random_data);
    }

    void write_data_to_compass()
    {
        auto _random_data {generate_random_data()};
        _compass_random_data += _random_data;
        write_data(_compass_drivers.output, _random_data);
    }

    void write_data_to_gps_and_compass()
    {
        write_data_to_gps();
        write_data_to_compass();
    }

    void terminate_gps()
    {
        if(_gps_virtual_port.running()){
            _gps_virtual_port.terminate();
        }
    }

    void terminate_compass()
    {
        if(_compass_virtual_port.running()){
            _compass_virtual_port.terminate();
        }
    }

    void terminate_logger_gps_compass()
    {
        terminate_logger();
        terminate_gps();
        terminate_compass();
    }

    void terminate_gps_compass()
    {
        terminate_gps();
        terminate_compass();
    }

    void run_logger(std::string argument = "")
    {
        generate_config_file();
        _logger.Command_line_options.config_file = _config_file;
        _logger.run(argument);
    }

    void wait_for_logger()
    {
        _logger.wait();
    }

    void generate_config_file()
    {
        boost::property_tree::ptree tree;

        tree.add("serial_ports." + _device1 + ".log_file", 
            _gps_report_file.string());
        tree.add("serial_ports." + _device1 + ".driver", 
            _gps_drivers.input.string());
        tree.add("serial_ports." + _device1 + ".baud_rate", 9600);
        tree.add("serial_ports." + _device1 + ".flow_control", "none");
        tree.add("serial_ports." + _device1 + ".parity", "none");
        tree.add("serial_ports." + _device1 + ".stop_bits", 1.0);
        tree.add("serial_ports." + _device1 + ".character_size", 8);

        tree.add("serial_ports." + _device2 + ".log_file", 
            _compass_report_file.string());
        tree.add("serial_ports." + _device2 + ".driver", 
            _compass_drivers.input.string());
        tree.add("serial_ports." + _device2 + ".baud_rate", 9600);
        tree.add("serial_ports." + _device2 + ".flow_control", "none");
        tree.add("serial_ports." + _device2 + ".parity", "none");
        tree.add("serial_ports." + _device2 + ".stop_bits", 1.0);
        tree.add("serial_ports." + _device2 + ".character_size", 8);

        boost::property_tree::write_json(_config_file, tree);
    }

    void terminate_logger()
    {
        if(_logger.running()){
            _logger.terminate();
        }
    }

    void wait_for_log()
    {
        std::this_thread::sleep_for(_delay_duration_for_logger);
    }

    std::vector<std::string> read_lines_from_logger_stdout(int number_of_lines)
    {
        std::vector<std::string> lines(number_of_lines);
        for(int i = 0; i < number_of_lines; i++) {
            lines[i] = _logger.read_a_line_from_stdout();
        }
        return lines;
    }

    void wait_for_reconnect()
    {
        std::this_thread::sleep_for(_delay_duration_for_reconnect);
    }

    void assert_report_file_equal_to_data_which_is_written_to_gps()
    {
        ASSERT_THAT(read_data(_gps_report_file), Eq(_gps_random_data));
    }

    void assert_report_file_equal_to_data_which_is_written_to_compass()
    {
        ASSERT_THAT(read_data(_compass_report_file), Eq(_compass_random_data));
    }

    void 
        assert_report_files_equal_to_data_which_is_written_to_gps_and_compass()
    {
        assert_report_file_equal_to_data_which_is_written_to_gps();
        assert_report_file_equal_to_data_which_is_written_to_compass();
    }

    std::vector<int> extract_sync_tags(const fs::path& file)
    {
        auto contents = read_data(file);
        std::vector<std::string> lines;
        boost::algorithm::split(
            lines, contents, [](char& delim){return delim == '\n';});

        std::vector<int> sync_tags;
        std::regex pattern("^(\\d*),.*$");
        
        for(const auto& line : lines) {
            std::smatch captured_match;
            if(std::regex_match(line, captured_match, pattern)) {
                sync_tags.push_back(
                    std::atoi(captured_match[1].str().c_str()));
            }
        }

        return sync_tags;
    }

    void 
        assert_report_files_has_sync_tags_and_equal_to_data_which_is_written_to_gps_and_compass()
    {
        auto gps_sync_tags = extract_sync_tags(_gps_report_file);
        auto compass_sync_tags = extract_sync_tags(_compass_report_file);

        ASSERT_THAT(gps_sync_tags.empty(), Not(true));
        ASSERT_THAT(compass_sync_tags.empty(), Not(true));
    }

    Logger _logger;
    Virtual_serial_port _gps_virtual_port;
    Virtual_serial_port _compass_virtual_port;
    Random_generator _random_generator;
    fs::path _gps_report_file {"gps_report_file.txt"};
    fs::path _compass_report_file {"compass_report_file.txt"};
    fs::path _config_file {_random_generator.generate_string(10)};
    Virtual_serial_port::Drivers _gps_drivers;
    Virtual_serial_port::Drivers _compass_drivers;
    std::string _gps_random_data {""};
    std::string _compass_random_data {""};
    std::chrono::seconds _delay_duration_after_run_a_process {1};
    std::chrono::seconds _delay_duration_for_logger {5};
    std::chrono::seconds _delay_duration_for_reconnect {9};
    FS_utils _FS_utils;
    const std::string _device1 = "GPS";
    const std::string _device2 = "compass";
    const int _minimum_random_sentence_size {5000};
    const int _maximum_random_sentence_size {10000};
    const int _minimum_number_of_lines {2};
    const int _maximum_number_of_lines {10};
    const int _sync_every_param {_random_generator.generate_integer(10, 5000)};

};

TEST_F(serial_port_logger_tests, When_gps_compass_logger_is_run_\
Then_it_must_log_gps_and_compass_data_to_file)
{
    run_gps_compass_logger();
    write_data_to_gps_and_compass();
    wait_for_log();
    terminate_logger_gps_compass();
    assert_report_files_equal_to_data_which_is_written_to_gps_and_compass();
}

TEST_F(serial_port_logger_tests, 
When_gps_module_is_disconnected_and_after_a_while_connected_\
Then_gps_compass_logger_must_reconnect_to_it_and_log_its_data_to_file)
{
    run_gps_compass_logger();
    write_data_to_gps_and_compass();
    wait_for_log();
    terminate_gps();
    wait_for_reconnect();
    run_gps();
    write_data_to_gps();
    wait_for_log();
    terminate_logger_gps_compass();
    assert_report_files_equal_to_data_which_is_written_to_gps_and_compass();
}

TEST_F(serial_port_logger_tests, 
When_compass_module_is_disconnected_and_after_a_while_connected_\
Then_gps_compass_logger_must_reconnect_to_it_and_log_its_data_to_file)
{
    run_gps_compass_logger();
    write_data_to_gps_and_compass();
    wait_for_log();
    terminate_compass();
    wait_for_reconnect();
    run_compass();
    write_data_to_compass();
    wait_for_log();
    terminate_logger_gps_compass();
    assert_report_files_equal_to_data_which_is_written_to_gps_and_compass();
}

TEST_F(serial_port_logger_tests, 
When_gps_module_is_not_available_\
Then_gps_compass_logger_must_reconnect_to_it_after_a_while)
{
    run_compass();
    run_logger();
    write_data_to_compass();
    wait_for_reconnect();
    run_gps();
    write_data_to_gps();
    wait_for_log();
    terminate_logger_gps_compass();
    assert_report_files_equal_to_data_which_is_written_to_gps_and_compass();
}

TEST_F(serial_port_logger_tests, 
When_compass_module_is_not_available_\
Then_gps_compass_logger_must_reconnect_to_it_after_a_while)
{
    run_gps();
    run_logger();
    write_data_to_gps();
    wait_for_reconnect();
    run_compass();
    write_data_to_compass();
    wait_for_log();
    terminate_logger_gps_compass();
    assert_report_files_equal_to_data_which_is_written_to_gps_and_compass();
}

TEST_F(serial_port_logger_tests, 
When_compass_module_is_not_available_\
Then_gps_compass_logger_must_log_gps_data)
{
    run_gps();
    run_logger();
    write_data_to_gps();
    wait_for_log();
    terminate_logger();
    terminate_gps();
    assert_report_file_equal_to_data_which_is_written_to_gps();
}

TEST_F(serial_port_logger_tests, 
When_gps_module_is_not_available_\
Then_gps_compass_logger_must_log_compass_data)
{
    run_compass();
    run_logger();
    write_data_to_compass();
    wait_for_log();
    terminate_logger();
    terminate_compass();
    assert_report_file_equal_to_data_which_is_written_to_compass();
}

TEST_F(serial_port_logger_tests, 
Given_gps_and_compass_is_connected_\
When_logger_is_run_with_is_connected_argument_\
Then_logger_must_print_they_are_connected_successfully)
{
    run_gps_compass();
    run_logger("--IS-CONNECTED");
    auto lines = read_lines_from_logger_stdout(2);
    terminate_gps_compass();

    std::vector<std::string> expectedLines({
        _device1 + " is connected successfully",
        _device2 + " is connected successfully"
    });

    ASSERT_THAT(lines, UnorderedElementsAreArray(expectedLines));

    wait_for_logger();
}


TEST_F(serial_port_logger_tests, 
Given_gps_and_compass_is_not_connected_\
When_logger_is_run_with_is_connected_argument_\
Then_logger_must_print_they_are_not_connected)
{
    run_logger("--IS-CONNECTED");
    auto lines = read_lines_from_logger_stdout(2);

    std::vector<std::string> expectedLines({
        _device1 + " is not connected",
        _device2 + " is not connected"
    });

    ASSERT_THAT(lines, UnorderedElementsAreArray(expectedLines));

    wait_for_logger();
}

TEST_F(serial_port_logger_tests, 
When_logger_is_run_with_SYNC_EVERY_argument_\
Then_logger_must_tag_every_sentene_with_time_label)
{
    run_gps_compass();
    auto cmd_params = "--SYNC-EVERY=" + std::to_string(_sync_every_param);
    run_logger(cmd_params);
    write_data_to_gps_and_compass();
    wait_for_log();
    terminate_logger_gps_compass();
    assert_report_files_has_sync_tags_and_equal_to_data_which_is_written_to_gps_and_compass();
}