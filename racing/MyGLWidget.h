// MyGLWidget.h
#include "LL4GLWidget.h"
#include <QtMultimedia/QMediaPlayer>
#include <QTimer>

#include "Car.h"

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent=0);
    ~MyGLWidget();
   

  public slots:
    void semaphoreTick();

  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent (QKeyEvent *event);
   
    virtual void initializeGL();
    virtual void iniEscena();
    virtual void iniCamera();
    virtual void iniMaterialTerra();

    virtual void modelTransformTerra();
    virtual void viewTransform(int view);
    virtual void projectTransform();

    virtual void paintGL();

    void modelTransform(mat4& TG);
    void setTint(vec3 tint);
  private:
    float angleX;
    
    bool topView = false;

    vec3 camPos[2];

    GameObject pipes[2];
    
    GameObject roads[8];

    GLuint tintLoc;

    Car cars[2];
   
    QMediaPlayer musicPlayer;
    QMediaPlayer sfxPlayer;

    QTimer semaphore;
    int semaphoreCountdown = 3;

    int printOglError(const char file[], int line, const char func[]);
};
