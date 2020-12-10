#pragma once

class File_ostream {

public:

    File_ostream(const fs::path& path):
        _file {std::make_shared<std::ofstream>(path, std::ios::app)}
    {
    }

    File_ostream& operator<<(const std::string& data)
    {
        *_file << data;
        return *this;
    }

    File_ostream& operator<<(const std::size_t& data)
    {
        *_file << data;
        return *this;
    }

    void close()
    {
        _file->close();
    }

private:

    std::shared_ptr<std::ofstream> _file;

};