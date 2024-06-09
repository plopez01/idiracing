#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

class GameObject {

  public:
    vec3 position, rotation, velocity;
    float scale;

    GameObject();
    GameObject(vec3 position, vec3 rotation, float scale, vec3 centreModel);
    
    void modelTransform(mat4& TG);

    void localToGlobalPosition(vec3& position);

    virtual void update();

  private:
    vec3 centreModel;

};
