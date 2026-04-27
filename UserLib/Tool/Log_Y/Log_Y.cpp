//
// Created by DingYong on 2026/4/25.
//

#include "Log_Y.hpp"
#include "cstdio"
#include "stm32f1xx_hal.h"

float Log_Y::getSystemTimeMs()
{
    return (float)HAL_GetTick();
}

Log_Y::Log_Y()
{
    _is_opened = false;
    _max_lines = 0;
    _current_line = 0;
    _file = nullptr;
}

void Log_Y::open(const char* filename,int max_lines)
{

    // "w" overwrite "a" continue to write
    _file = fopen(filename,"w");

    if (_file != nullptr)
    {
        // init the params
        _is_opened = true;
        _max_lines = max_lines;
        _current_line = 0;

        // write CSV first line
        fprintf(_file,"time,gx,gy,gz\n");
    }
}

void Log_Y::write(float gx, float gy, float gz)
{
    // check : is open / file is empty or full
    if (!_is_opened || _file == nullptr || isFull())
    {
        return;
    }

    // get system timestamp
    float time_ms = getSystemTimeMs();

    // write a line context
    fprintf(_file,"%.2f,%.2f,%.2f,%.2f\n",time_ms,gx,gy,gz);

    _current_line++;

    // auto close when the lines get the max_lines
    if (isFull())
    {
        close();
    }
}

bool Log_Y::isFull() const
{
    return _current_line >= _max_lines;
}

void Log_Y::close()
{
    if (_file != nullptr)
    {
        fclose(_file);
        _file = nullptr;
    }

    _is_opened = false;
}
