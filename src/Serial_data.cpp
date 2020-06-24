#include "Serial_data.h"
#include <iostream>
#include <boost/asio.hpp>

Serial_data::Serial_data(): mpFX( wxT("Serial 1") ), bufmax(0)
{
    // 构造函数记得要在参数列表里给串口读写类做初始化
    //ctor
}

double Serial_data::GetY(double x) // 对接wxWidget的部分
{
    long xa = x;
    if(xa<0 || xa>=(int)data.size()-1){
        return -100;
    }
    else{
        return data[xa] + (data[xa+1]-data[xa]) * (x-xa);
    }
}




Serial_data::~Serial_data()
{
    //dtor
}
