#include <sys/stat.h> // to check file exists or not
#pragma once // prevent errors when some libraries are called in another scripts
#include <iostream>
#include <math.h>
#include <string>
#include <xlnt/xlnt.hpp> // include xlnt for excel handling

inline bool file_exists (const std::string& name) {
    /* This functions check if a file exists */
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0); 
}

xlnt::border create_data_border (){
    
    xlnt::border border;
    xlnt::border::border_property border_prop;
    border_prop.style(xlnt::border_style::thin);
    border.side(xlnt::border_side::start, border_prop);
    border.side(xlnt::border_side::end, border_prop);
    border.side(xlnt::border_side::top, border_prop);
    border.side(xlnt::border_side::bottom, border_prop);

    return border;
}