#include "GameObject.h"
#include <glm/trigonometric.hpp>

GameObject::GameObject(){
  position = vec3(0);
  rotation = vec3(0);
  scale = 1;
  centreModel = vec3(0);
  velocity = vec3(0);
}

GameObject::GameObject(vec3 position, vec3 rotation, float scale, vec3 centreModel) {
  this->position = position;
  this->rotation = rotation;
  this->scale = scale;
  this->centreModel = centreModel;
  this->velocity = vec3(0);
}

void GameObject::localToGlobalPosition(vec3& pos) {
  mat3 globalMat = rotate(mat4(1.0), radians(rotation.y), vec3(0, 1, 0));
  pos = globalMat * pos;
}

void GameObject::modelTransform(mat4& TG)
{
  TG = mat4(1.0f);
  TG = glm::translate(TG, position);
  TG = glm::scale(TG, glm::vec3(scale));
  TG = glm::rotate(TG, glm::radians(rotation.x), glm::vec3(1,0,0));
  TG = glm::rotate(TG, glm::radians(rotation.y), glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreModel);
}

void GameObject::update(){
  position += velocity;

  velocity *= 0.95;
}
