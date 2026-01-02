#include"funcs.h"
#include<iostream>
#include<string>
#include<random>
#include<sstream>

std::string random_id(int lenght){
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 9);
    std::stringstream ss;

    for(int pos = 0; pos < lenght; pos++){
        ss << distrib(gen);
    }

    return ss.str();
}