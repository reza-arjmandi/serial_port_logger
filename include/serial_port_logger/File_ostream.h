#pragma once

class File_ostream {

public:

    File_ostream(const fs::path& path):
        _path {path}
    {
    }

    File_ostream& operator<<(const std::string& data)
    {
        std::ofstream {_path, std::ios::app} << data;
        return *this;
    }

private:

    fs::path _path;

};