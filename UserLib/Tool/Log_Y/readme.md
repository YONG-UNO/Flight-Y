# WHAT
```mermaid
this is a CSV LOGGER SYSTEM to record 
the data in the running term
You can use python scripts to perform mathematical 
engineering analysis of the recorded data, eg. FFT

Or as a drone black box that records your every crash...(QAQ)
```

# HOW
```c++
#include "csvLogger.hpp"

// creat a object
csvLogger logger;

void init()
{
    // open the specialed file and meanwhile open the file 
    logger.open("gyro_log.csv");
}



```