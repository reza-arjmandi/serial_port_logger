#pragma once

class File_ostream {

public:

    File_ostream(const fs::path& path):
        _file {std::make_shared<std::ofstream>(path)}
    {
    }

    File_ostream& operator<<(const std::string& data)
    {
        *_file << data;
        return *this;
    }

private:

    std::shared_ptr<std::ofstream> _file;

};