#include "Car.h"
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
using namespace std;

void Car::keyPressEvent(QKeyEvent* event) {
  if (arrows){
    switch (event->key()) {
      case Qt::Key_Up: {
        keys[UP] = true;
        break;
      }
      case Qt::Key_Down: {
        keys[DOWN] = true;
        break;
      }
      case Qt::Key_Left: {
        keys[LEFT] = true;
        break;
      }
      case Qt::Key_Right: {
        keys[RIGHT] = true;
        break;
      }
    }
  } else {
    switch (event->key()) { 
      case Qt::Key_W: {
        keys[UP] = true;
        break;
      }
      case Qt::Key_S: {
        keys[DOWN] = true;
        break;
      }
      case Qt::Key_A: {
        keys[LEFT] = true;
        break;
      }
      case Qt::Key_D: {
        keys[RIGHT] = true;
        break;
      }
  }
  }
}

void Car::keyReleaseEvent(QKeyEvent* event) {
  if (arrows){
    switch (event->key()) {
      case Qt::Key_Up: {
        keys[UP] = false;
        break;
      }
      case Qt::Key_Down: {
        keys[DOWN] = false;
        break;
      }
      case Qt::Key_Left: {
        keys[LEFT] = false;
        break;
      }
      case Qt::Key_Right: {
        keys[RIGHT] = false;
        break;
      }
    }
  } else {
    switch (event->key()) {
      case Qt::Key_W: {
        keys[UP] = false;
        break;
      }
      case Qt::Key_S: {
        keys[DOWN] = false;
        break;
      }
      case Qt::Key_A: {
        keys[LEFT] = false;
        break;
      }
      case Qt::Key_D: {
        keys[RIGHT] = false;
        break;
      }
    }
  }

}

void Car::printCarInfo(){
  std::cout << "Position: (" << position.x << ' ' << position.y << ' ' << position.z << ')' << '\n';
  std::cout << "Rotation: (" << rotation.x << ' ' << rotation.y << ' ' << rotation.z << ')' << std::endl;
}

void Car::update() {
  mat3 globalMat = rotate(mat4(1.0), radians(rotation.y), vec3(0, 1, 0));

  mat3 localMat = inverse(globalMat);

  vec3 fwdvec = vec3(0, 0, -0.02);
  vec3 backvec = vec3(0, 0, 0.01);

  if (keys[UP]) velocity += (mat3)globalMat * fwdvec;
  if (keys[DOWN]) velocity += (mat3) globalMat * backvec;
  if (keys[LEFT]) rotation += vec3(0, steerRate*length(velocity), 0);
  if (keys[RIGHT]) rotation += vec3(0, -steerRate*length(velocity), 0);
  
  position += velocity;
  velocity *= (1 - globalDrag);
  
  vec3 localVel = (mat3)localMat * velocity;

  //cout << "Vel: " << localVel.x << " " << localVel.z << endl;
  
  localVel.x *= (1 - lateralDrag);
  velocity = globalMat * localVel;
}
