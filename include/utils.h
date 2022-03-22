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

int get_current_time(std::string time_type){
    

    int time_data;  // year since 1900
    time_t current_time = std::time(0);
    tm *ltm = localtime(&current_time);
    if (time_type == "year"){
        time_data = 1900 + ltm->tm_year;
    }
    
    return time_data;
}