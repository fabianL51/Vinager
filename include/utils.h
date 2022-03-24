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

int get_current_date_data(std::string time_type){
    

    int date_data;  // year since 1900
    time_t current_time = std::time(0);
    tm *ltm = localtime(&current_time);
    if (time_type == "year"){
        date_data = 1900 + ltm->tm_year;
    }
    if (time_type == "month"){
        date_data = ltm->tm_mon;
    }
    if (time_type == "day"){
        date_data = ltm->tm_mday;
    }
    
    return date_data;
}

std::string get_current_date(){
    
    std::string current_date;
    time_t current_time = std::time(0);
    tm *ltm = localtime(&current_time);

    current_date = std::to_string(ltm->tm_mday) + "." + std::to_string(ltm->tm_mon) + "." + std::to_string(1900 + ltm->tm_year);

    return current_date;

}