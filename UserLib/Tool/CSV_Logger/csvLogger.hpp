//
// Created by DingYong on 2026/4/25.
//

#pragma once

class csvLogger
{
public:
    csvLogger();
    void open(const char* filename);
    void write(float time,float gx,float gy,float gz);
    void close();

private:
    bool _is_opened;
};
