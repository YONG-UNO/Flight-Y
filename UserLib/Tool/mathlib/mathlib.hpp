/**
* @file mathlib
 * @brief 通用math库
 * @author DingYong
 * @version V1.0.0
 * @date 2026-04-14
 *
 * @history
 * 版本      日期        作者       变更内容
 * V1.0.0    2026-04-14  DingYong   初版创建
 * V1.0.1    2026-04-28  DingYong   从Functions改名为mathlib
 */

#ifndef FLIGHT_Y_FUNCTIONS_H
#define FLIGHT_Y_FUNCTIONS_H
class math
{
public:
    static float constrain(float val ,float min_val,float max_val)
    {
        return (val < min_val) ? min_val : ((val > max_val) ? max_val : val);
    }
};

#endif //FLIGHT_Y_FUNCTIONS_H
