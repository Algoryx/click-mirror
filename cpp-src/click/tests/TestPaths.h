#pragma once

#include<string>

inline std::string srcDir(const std::string file_path = __FILE__) {
    size_t idx = file_path.rfind("/");
    if (idx == std::string::npos)
        idx = file_path.rfind("\\");
    return file_path.substr(0, idx);
}

inline const std::string TESTDATA_DIR = srcDir() + "/../../../testdata/";
