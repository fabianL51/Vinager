#include <sys/stat.h> // to check file exists or not
#pragma once // prevent errors when some libraries are called in another scripts
#include <iostream>
#include <math.h>
#include <string>
#include "ctime"

inline bool file_exists (const std::string& name) {
    /* This functions check if a file exists */
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0); 
}