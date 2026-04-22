//
// Created by DingYong on 2026/4/21.
//

#ifndef FLIGHT_Y_MATRIX_HPP
#define FLIGHT_Y_MATRIX_HPP

namespace matrix
{
    // 三维向量类
    class Vector3f
    {
    public:
        float x,y,z;

        // 默认构造，初始化为0
        Vector3f()
        {
            x = 0;
            y = 0;
            z = 0;
        }

        // 带参数构造，直接赋值
        Vector3f(float px,float py,float pz)
        {
            x = px;
            y = py;
            z = pz;
        }

        // use v(i) to replace v.x
        // delete const，非只读
        float& operator()(int i)
        {
            if (i==0) return x;
            if (i==1) return y;
            if (i==2) return z;
        }

        // vector * num
        Vector3f operator*(float s)const{
            return Vector3f(x*s,y*s,z*s);
        }

        // vector / num
        Vector3f operator/(float s)const{
            return Vector3f(x/s,y/s,z/s);
        }

        // vector + vector
        Vector3f operator+(const Vector3f& v)const
        {
            return Vector3f{x+v.x,y+v.y,z+v.z};
        }

        Vector3f emult(const Vector3f& v) const
        {
            return Vector3f{x*v.x,y*v.y,z*v.z};
        }
    };

    class Quatf
    {
    public:
        float w,x,y,z; // 四元数格式

        Quatf()
        {
            w = 1;
            x = 0;
            y = 0;
            z = 0;
        }

        Quatf(float pw,float px,float py,float pz)
        {
            w = pw;
            x = px;
            y = py;
            z = pz;
        }

        // 下标访问
        float operator()(int i) const
        {
            if (i == 0) return w;
            if (i == 1) return x;
            if (i == 2) return y;
            if (i == 3) return z;
        }

        // 四元数求逆
        Quatf inversed() const
        {
            return Quatf(w,-x,-y,-z);
        }

        // Quaternion number multiplication
        Quatf operator*(const Quatf &q) const
        {
            float nw = w*q.w - x*q.x - y*q.y - z*q.z;
            float nx = w*q.x + x*q.w + y*q.z - z*q.y;
            float ny = w*q.y - x*q.z + y*q.w + z*q.x;
            float nz = w*q.z + x*q.y - y*q.x + z*q.w;
            return Quatf(nw, nx, ny, nz);
        }

        float fast_sqrt(float x)
        {
            float i;
            i = 0.5f * x;  // 初始值
            // 牛顿迭代 2 次，精度足够飞控
            i = i * (1.5f - (x * 0.5f) * i);
            i = i * (1.5f - (x * 0.5f) * i);
            return 1.0f / i;
        }

        // normalize
        void normalize()
        {
            float len = fast_sqrt(w*w + x*x + y*y + z*z);
            w /= len;
            x /= len;
            y /= len;
            z /= len;
        }

        // take the virtual part
        Vector3f imag() const
        {
            return Vector3f(x,y,z);
        }

        //
    };

}


#endif //FLIGHT_Y_MATRIX_HPP
