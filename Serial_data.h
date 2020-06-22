#ifndef SERIAL_DATA_H
#define SERIAL_DATA_H

#include <wx/mathplot.h>
#include <boost/asio.hpp>
#include <vector>
#include <algorithm>

class Serial_data : public mpFX
{
    public:
        Serial_data();          // 默认构造函数
        //Ser1ial_data()
        virtual ~Serial_data(); // 默认析构函数
        virtual double GetY(double x);
        void Clear() { data.clear(); bufmax=0; } ;
        void addElement(double x) { data.push_back(x); bufmax=std::max(bufmax,x); }
        size_t getSize() { return data.size(); }
        double getMaxValue() { return bufmax; }

    protected:
        std::vector<double> data;
        void handle_read(uint8_t buf[]);

    private:
        double bufmax;
};

#endif // SERIAL_DATA_H
