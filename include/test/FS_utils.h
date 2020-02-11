#pragma once

class FS_utils {

public:

    void remove_if_exists(const fs::path& path_)
    {
        if(fs::exists(path_)){
            fs::remove(path_);
        }
    }

    void create_if_not_exist(const fs::path& path_)
    {
        if(!fs::exists(path_)){
            std::ofstream file;
            file.open(path_);
            file.close();
        }
    }

};