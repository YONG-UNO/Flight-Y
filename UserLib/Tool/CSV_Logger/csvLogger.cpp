//
// Created by DingYong on 2026/4/25.
//

#include "csvLogger.hpp"
#include "cstdio"

static FILE* file = nullptr;

csvLogger::csvLogger()
{
    _is_opened = false;
}

void csvLogger::open(const char* filename)
{
    file = fopen(filename,"w");

    if (file != nullptr)
    {
        fprintf(file,"time,gx,gy,gz\n");
        _is_opened = true;
    }
}

void csvLogger::write(float time, float gx, float gy, float gz)
{
    if (!_is_opened || file == nullptr)
    {
        return;
    }

    // write a line context
    fprintf(file,"%.2f,%.2f,%.2f,%.2f\n",time,gx,gy,gz);
}

void csvLogger::close()
{
    if (file != nullptr)
    {
        fclose(file);
        _is_opened = false;
    }
}
