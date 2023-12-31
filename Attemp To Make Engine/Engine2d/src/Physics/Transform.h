#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Vector2d.h"

class Transform {
  public:

    float X,Y;

    Transform(float x=0,float y=0): X(x),Y(y) {}

    void Log(std::string msg = "") {
        std::cout<<msg<<"(X Y) = { "<<X<<' '<<Y<<" }"<<std::endl;
    }

    inline void TranslateX(float x) {
        X += x;
    }
    inline void TranslateY(float y) {
        Y += y;
    }
    inline void Translate(Vector2d v) {
        TranslateX(v.X);
        TranslateY(v.Y);
    }
};

#endif // TRANSFORM_H
