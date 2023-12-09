#include "glm/glm.hpp"
using namespace glm;



class particle{
    public:
        particle();
        particle(bool, float, float, vec4, vec2, vec2);
        bool locked = false;
        bool edgesReset = true;
        float maxSpeed = 0.005f;
        float mass = 1.0f;
        float size = 1.0f;
        vec4 color = vec4(1,1,1,1);
        vec2 position = vec2(cos(rand()), sin(rand()));
        vec2 oldPosi = position;
        vec2 speed = vec2(0,0);
        vec2 acceleration = vec2(0,0);
        void update();
        void applyForce(vec2&);
        void edges();
};

