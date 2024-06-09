#include "GameObject.h"
#include <QKeyEvent>
#include "keys.h"

class Car : public GameObject {
  public:
    bool keys[4] = {0};
    bool arrows = true;

    float globalDrag = 0.05;
    float lateralDrag = 0.5;
    float steerRate = 3;
    vec3 color = vec3(1);

    Car() : GameObject() {}
    Car(vec3 position, vec3 rotation, float scale, vec3 centreModel) : GameObject(position, rotation, scale, centreModel) {}

    Car(vec3 position, vec3 rotation, float scale, vec3 centreModel, bool arrows, vec3 color) : GameObject(position, rotation, scale, centreModel) {
      this->arrows = arrows;
      this->color = color;
    }

    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    void printCarInfo();

    virtual void update() override;
};
