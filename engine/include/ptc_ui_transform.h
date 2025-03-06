#ifndef PTC_UI_TRANSFORM_H
#define PTC_UI_TRANSFORM_H

#include <ptc_transform.h>

class UITransform : public Transform
{
public:
    int width;
    int height;
    UITransform(Object* _object, int _width = 200, int _height = 50);
};

#endif