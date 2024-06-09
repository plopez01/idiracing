// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

MyGLWidget::MyGLWidget(QWidget *parent) : LL4GLWidget(parent) {

}

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  
  carregaShaders();
  
  creaBuffersModels();
  creaBuffersTerra();


  tintLoc = glGetUniformLocation (program->programId(), "tint");
  setTint(vec3(1));

  iniEscena();
  iniCamera();

  connect(&semaphore, SIGNAL(timeout()), this, SLOT(semaphoreTick()));

  sfxPlayer.setMedia(QUrl::fromLocalFile("/home/plopez/idi/racing/pitido.ogg"));
  sfxPlayer.play(); 
  musicPlayer.setMedia(QUrl::fromLocalFile("/home/plopez/idi/racing/music.ogg"));
  musicPlayer.setVolume(50);

  semaphore.start(4000);
}

void MyGLWidget::semaphoreTick(){
    semaphore.stop();
    musicPlayer.play();
}

void MyGLWidget::iniEscena ()
{
  centreEsc = glm::vec3(5,0,5);
  radiEsc = 40;
  pipes[0] = GameObject(vec3(radiEsc, 0, radiEsc), vec3(0, 0, 0), escalaModels[PIPE]*10, centreBaseModels[PIPE]);
  pipes[1] = GameObject(vec3(radiEsc*2.4f, 0, -0.4*radiEsc), vec3(0, 0, 0), escalaModels[PIPE]*10, centreBaseModels[PIPE]);

  cars[0] = Car(vec3(72, 0, 46), vec3(0, 350, 0), escalaModels[CAR], centreBaseModels[CAR], true, vec3(1, 0, 0));
  cars[1] = Car(vec3(64, 0, 44.5), vec3(0, 350, 0), escalaModels[CAR], centreBaseModels[CAR], false, vec3(0, 0, 1));
  
  float roadSize = radiEsc;

  vec3 roadPos[] = {
    vec3(0, 0.01, 0),
    vec3(1, 0.01, -1.1/2.0),
    vec3(1.54, 0.01, -1.5),
    vec3(2.5, 0.01, -1.5),
    vec3(2.5, 0.01, -1.1/2.0),
    vec3(1.5, 0.01, 0),
    vec3(0.95, 0.01, 1),
    vec3(0, 0.01, 1)
  };

  vec3 roadRots[] = {
    vec3(0, 90, 0),
    vec3(0, 270, 0),
    vec3(0, 90, 0),
    vec3(0, 0, 0),
    vec3(0, -90, 0),
    vec3(0, 90, 0),
    vec3(0, -90, 0),
    vec3(0, 180, 0)
  };

  for (int i = 0; i < 8; i++) {
    roads[i] = GameObject(
        vec3(
          (roadPos[i].x + 1/2.0) * roadSize, 
          roadPos[i].y, 
          (roadPos[i].z + 1/2.0) * roadSize), 
        roadRots[i], 
        escalaModels[ROAD]*roadSize/2.0, 
        centreBaseModels[ROAD]);
  }
}

void MyGLWidget::iniCamera ()
{
  angleX = 0.0;
  angleY = 0.0;
  viewTransform (0);
}

void MyGLWidget::iniMaterialTerra ()
{
  // Donem valors al material del terra
  amb = glm::vec3(0.5, 1, 0.4);
  diff = glm::vec3(0, 0, 0);
  spec = glm::vec3(0,0,0);
  shin = 100;
}

void MyGLWidget::modelTransformTerra ()
{
  glm::mat4 TG(1.0f);
  TG = translate(TG, vec3(-1*radiEsc, 0, -3*radiEsc));
  TG = scale(TG, vec3(6.0*radiEsc/10));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform (int view)
{
  // EULER
  //View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
 //View = glm::translate(View, -centreEsc);
  //View = glm::rotate(View, angleX, glm::vec3(1, 0, 0));
  //View = glm::rotate(View, angleY, glm::vec3(0, 1, 0));
  
  if (topView) {
    // Top view
    View = glm::lookAt(vec3(radiEsc, radiEsc, radiEsc), cars[view].position, vec3(0, 1, 0));
  } else {
    // Follow
    vec3 followPos = vec3(0, 5, 10);
    cars[view].localToGlobalPosition(followPos);
    
    camPos[view] = mix(camPos[view], followPos + cars[view].position, 1/30.0);
    View = glm::lookAt(camPos[view], cars[view].position, vec3(0, 1, 0));
  }
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj;  // Matriu de projecció
  Proj = glm::perspective(float(M_PI/3.0), ra, 1.0f, 10.0f*radiEsc);

  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::modelTransform(mat4& TG){
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::setTint(vec3 tint) {
  glUniform3f(tintLoc, tint.r, tint.g, tint.b);
}

void MyGLWidget::paintGL () 
{
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  vec4 viewports[] = {
    vec4(0, 0, ample, alt),
    vec4(ample, 0, ample, alt)
  };

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  for (int view = 0; view < 2; view++) {
    glViewport (
        viewports[view].x,
        viewports[view].y,
        viewports[view].z,
        viewports[view].w
    );

    // Esborrem el frame-buffer i el depth-buffer
    mat4 TG(1.0);

    // TERRA
    glBindVertexArray (VAO_Terra);
    modelTransformTerra ();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    // Road
    glBindVertexArray (VAO_models[ROAD]);
    for (int i = 0; i < 8; i++) {
      roads[i].modelTransform(TG);
      modelTransform(TG);
      glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size()*3);
    }
    // Car
    glBindVertexArray (VAO_models[CAR]);
    
    for (int i = 0; i < 2; i++) {
      cars[i].modelTransform(TG);
      modelTransform(TG);
      setTint(cars[i].color);
      glDrawArrays(GL_TRIANGLES, 0, models[CAR].faces().size()*3);
      setTint(vec3(1));
    }

    // Pipe
    glBindVertexArray (VAO_models[PIPE]);
    for (int i = 0; i < 2; i++) {
      pipes[i].modelTransform(TG);
      modelTransform(TG);
      glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
    }
    glBindVertexArray(0);
    
    cars[view].update( );
    
    viewTransform(view);
  }

  cars[0].printCarInfo();

  update();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleX += (e->y() - yClick) * M_PI / ample;
    angleY += (e->x() - xClick) * M_PI / ample;
    viewTransform (0);
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  
  for (int i = 0; i < 2; i++) {
    cars[i].keyPressEvent(event);
  }

  switch (event->key()) {
  case Qt::Key_Up: {
      // ...
    break;
	}
  case Qt::Key_C: {
      topView = !topView;
    break;
	}
  case Qt::Key_S: {
      // ...
    break;
	}	
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::keyReleaseEvent(QKeyEvent* event) {
  makeCurrent();
  for (int i = 0; i < 2; i++) {
    cars[i].keyReleaseEvent(event);
  }

  update();
}
