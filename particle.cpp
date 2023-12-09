#include "particle.h"

particle::particle(bool l, float m, float s, vec4 c, vec2 p, vec2 sp) {
    locked = l;
    edgesReset = false;
    maxSpeed = 0.05f;
    mass = m;
    size = s;
    color = c;
    position = p;
    oldPosi = position;
    speed = sp;
    acceleration = vec2(0,0);
}

particle::particle() {
}

void particle::update(){
    if (locked == false){
        oldPosi = position;
        speed += acceleration;
        if (sqrt(speed.x*speed.x + speed.y*speed.y) > maxSpeed) {
	        speed = normalize(speed) * maxSpeed;
        }
        position += speed;
        acceleration = vec2(0,0);
        if (edgesReset == true){edges();}
    }
}

void particle::applyForce(vec2& force){
    acceleration += force/mass;
}

void particle::edges(){
    if (position.y > 1){ position.y = -1;oldPosi = position;}
    if (position.y < -1){ position.y = 1;oldPosi = position;}
    if (position.x > 1){ position.x = -1;oldPosi = position;}
    if (position.x < -1){ position.x = 1;oldPosi = position;}
}