#pragma once
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/mat4x3.hpp"

class CurveMover
{
protected:
    float t = 0;
    int direction = 1;
    float speed = 0.005f;
    glm::vec3 final = glm::vec3(1);
    glm::mat4x3 points = {
            0, 5, 0,
            5, 10, 0,
            10, 5, 0,
            5, 0, 0
    };
public:
    virtual void moveOnCurve() {
        t += speed * direction;
        if (t > 1)
            direction = -1;
        if (t < 0)
            direction = 1;


        glm::vec4 tMat = glm::vec4(t * t * t, t * t, t, 1);

        glm::mat4 bezier = {
            -1,  3, -3, 1,
             3, -6,  3, 0,
            -3,  3,  0, 0,
             1,  0,  0, 0
        };


        final = (points * bezier) * tMat;
    }

    void setSpeed(float _speed) {
        speed = _speed;
    }

    void setPoints(glm::mat4x3 _points) {
        points = _points;
    }
};

