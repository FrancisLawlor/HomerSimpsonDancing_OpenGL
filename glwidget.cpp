#include "glwidget.h"
#include "arcBall.h"
#include "point.h"
#include "vector.h"
#include <QDebug>

// the viewing area dimensions
int viewWidth;
int viewHeight;

// the zoom factor
double zoomFactor = 1.0;

float ortho = 1.0;
float orthoNear = -100.0;
float orthoFar = 100.0;

static GLfloat light_position[] = {0.0, 100.0, 100.0, 0.0};

// basic colours
static GLfloat black[] = {0.0, 0.0, 0.0, 1.0};
static GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
static GLfloat grey[] = {0.5, 0.5, 0.5, 1.0};

// primary colours
static GLfloat red[] = {1.0, 0.0, 0.0, 1.0};
static GLfloat green[] = {0.0, 1.0, 0.0, 1.0};
static GLfloat blue[] = {0.0, 0.0, 1.0, 1.0};

// secondary colours
static GLfloat yellow[] = {1.0, 1.0, 0.0, 1.0};
static GLfloat magenta[] = {1.0, 0.0, 1.0, 1.0};
static GLfloat cyan[] = {0.0, 1.0, 1.0, 1.0};

// other colours
static GLfloat orange[] = {1.0, 0.5, 0.0, 1.0};
static GLfloat brown[] = {0.5, 0.25, 0.0, 1.0};
static GLfloat dkgreen[] = {0.0, 0.5, 0.0, 1.0};
static GLfloat pink[] = {1.0, 0.6, 0.6, 1.0};

bool drag;
bool pull;
float pullX = 0.0;
float pullY = 0.0;
float moveX = 0.0;
float moveY = 0.0;

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent) {
  setFocusPolicy(Qt::StrongFocus);
  resetArcBall = false;
  axes = true;
  grid = true;
  wframe = false;
  animation = false;
  nGridlines = 3;
  arc = new arcBall();
  objects = new Objects(this);
  viewWidth = this->width();
  viewHeight = this->height();

  time = 0;
  nframes = 600;

  left_arm = 90.0f, right_arm = 90.0f;
  left_leg = 20.0f, right_leg = -20.0f;
  left_knee = -40.0f, right_knee = 0.0f;

  left_max = true;
  left_min = false;

  right_max = false;
  right_min = true;

  this->animate(time);

  QTimer *timer = new QTimer(this);
  timer->start(0);
  connect(timer, SIGNAL(timeout()), this, SLOT(idle()));
}

void GLWidget::loadTextures() {
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  // load a texture
  glGenTextures(sizeof(textures) / sizeof(GLuint), textures);
  if (textures[0] == 0)
    qDebug() << "invalid texture names";

  QImage homermouth;
  homermouth.load(":/homermouth.png");
  QImage glFormattedImage = QGLWidget::convertToGLFormat(homermouth);
  if (!glFormattedImage.isNull()) {
    qDebug() << "Texture read correctly...";
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    // tell OpenGL which texture you want to use
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glFormattedImage.width(),
                 glFormattedImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 glFormattedImage.bits());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  } else {
    qDebug() << "Image 1 would not load";
  }

  QImage yellowtexture;
  yellowtexture.load(":/yellowtexture.png");
  glFormattedImage = QGLWidget::convertToGLFormat(yellowtexture);
  if (!glFormattedImage.isNull()) {
    qDebug() << "Texture read correctly...";
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    // tell OpenGL which texture you want to use
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glFormattedImage.width(),
                 glFormattedImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 glFormattedImage.bits());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  } else {
    qDebug() << "Image 2 would not load";
  }

  QImage whitetexture;
  whitetexture.load(":/whitetexture.png");
  glFormattedImage = QGLWidget::convertToGLFormat(whitetexture);
  if (!glFormattedImage.isNull()) {
    qDebug() << "Texture read correctly...";
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    // tell OpenGL which texture you want to use
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glFormattedImage.width(),
                 glFormattedImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 glFormattedImage.bits());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  } else {
    qDebug() << "Image 3 would not load";
  }

  QImage homereye;
  homereye.load(":/homereye.png");
  glFormattedImage = QGLWidget::convertToGLFormat(homereye);
  if (!glFormattedImage.isNull()) {
    qDebug() << "Texture read correctly...";
    glBindTexture(GL_TEXTURE_2D, textures[3]);
    // tell OpenGL which texture you want to use
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glFormattedImage.width(),
                 glFormattedImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 glFormattedImage.bits());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  } else {
    qDebug() << "Image 4 would not load";
  }

  QImage homerabdomen;
  homerabdomen.load(":/homerabdomen.png");
  glFormattedImage = QGLWidget::convertToGLFormat(homerabdomen);
  if (!glFormattedImage.isNull()) {
    qDebug() << "Texture read correctly...";
    glBindTexture(GL_TEXTURE_2D, textures[4]);
    // tell OpenGL which texture you want to use
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glFormattedImage.width(),
                 glFormattedImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 glFormattedImage.bits());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  } else {
    qDebug() << "Image 5 would not load";
  }

  QImage homerhair;
  homerhair.load(":/homerhair.png");
  glFormattedImage = QGLWidget::convertToGLFormat(homerhair);
  if (!glFormattedImage.isNull()) {
    qDebug() << "Texture read correctly...";
    glBindTexture(GL_TEXTURE_2D, textures[5]);
    // tell OpenGL which texture you want to use
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glFormattedImage.width(),
                 glFormattedImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 glFormattedImage.bits());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  } else {
    qDebug() << "Image 6 would not load";
  }


  QImage homerears;
  homerears.load(":/homerears.png");
  glFormattedImage = QGLWidget::convertToGLFormat(homerears);
  if (!glFormattedImage.isNull()) {
    qDebug() << "Texture read correctly...";
    glBindTexture(GL_TEXTURE_2D, textures[6]);
    // tell OpenGL which texture you want to use
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glFormattedImage.width(),
                 glFormattedImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 glFormattedImage.bits());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  } else {
    qDebug() << "Image 7 would not load";
  }

  QImage bluetexture;
  bluetexture.load(":/bluetexture.png");
  glFormattedImage = QGLWidget::convertToGLFormat(bluetexture);
  if (!glFormattedImage.isNull()) {
    qDebug() << "Texture read correctly...";
    glBindTexture(GL_TEXTURE_2D, textures[7]);
    // tell OpenGL which texture you want to use
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glFormattedImage.width(),
                 glFormattedImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 glFormattedImage.bits());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  } else {
    qDebug() << "Image 8 would not load";
  }

  QImage homerhand;
  homerhand.load(":/homerhand.png");
  glFormattedImage = QGLWidget::convertToGLFormat(homerhand);
  if (!glFormattedImage.isNull()) {
    qDebug() << "Texture read correctly...";
    glBindTexture(GL_TEXTURE_2D, textures[8]);
    // tell OpenGL which texture you want to use
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glFormattedImage.width(),
                 glFormattedImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 glFormattedImage.bits());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  } else {
    qDebug() << "Image 9 would not load";
  }

  QImage duff;
  duff.load(":/duff.jpg");
  glFormattedImage = QGLWidget::convertToGLFormat(duff);
  if (!glFormattedImage.isNull()) {
    qDebug() << "Texture read correctly...";
    glBindTexture(GL_TEXTURE_2D, textures[9]);
    // tell OpenGL which texture you want to use
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glFormattedImage.width(),
                 glFormattedImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 glFormattedImage.bits());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  } else {
    qDebug() << "Image 10 would not load";
  }

  QImage blacktexture;
  blacktexture.load(":/blacktexture.png");
  glFormattedImage = QGLWidget::convertToGLFormat(blacktexture);
  if (!glFormattedImage.isNull()) {
    qDebug() << "Texture read correctly...";
    glBindTexture(GL_TEXTURE_2D, textures[10]);
    // tell OpenGL which texture you want to use
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glFormattedImage.width(),
                 glFormattedImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 glFormattedImage.bits());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  } else {
    qDebug() << "Image 11 would not load";
  }

  QImage dancefloor;
  dancefloor.load(":/dancefloor.jpg");
  glFormattedImage = QGLWidget::convertToGLFormat(dancefloor);
  if (!glFormattedImage.isNull()) {
    qDebug() << "Texture read correctly...";
    glBindTexture(GL_TEXTURE_2D, textures[11]);
    // tell OpenGL which texture you want to use
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glFormattedImage.width(),
                 glFormattedImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 glFormattedImage.bits());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  } else {
    qDebug() << "Image 12 would not load";
  }

  QImage discoball;
  discoball.load(":/discoball.jpg");
  glFormattedImage = QGLWidget::convertToGLFormat(discoball);
  if (!glFormattedImage.isNull()) {
    qDebug() << "Texture read correctly...";
    glBindTexture(GL_TEXTURE_2D, textures[12]);
    // tell OpenGL which texture you want to use
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glFormattedImage.width(),
                 glFormattedImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 glFormattedImage.bits());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  } else {
    qDebug() << "Image 13 would not load";
  }


  QImage lightbeam;
  lightbeam.load(":/lightbeam.jpg");
  glFormattedImage = QGLWidget::convertToGLFormat(lightbeam);
  if (!glFormattedImage.isNull()) {
    qDebug() << "Texture read correctly...";
    glBindTexture(GL_TEXTURE_2D, textures[13]);
    // tell OpenGL which texture you want to use
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glFormattedImage.width(),
                 glFormattedImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 glFormattedImage.bits());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  } else {
    qDebug() << "Image 14 would not load";
  }

  //       QImage something;
  //       something.load("something.jpg");
  //       glFormattedImage = QGLWidget::convertToGLFormat(something);
  //       if(!glFormattedImage.isNull()){
  //           qDebug() << "Texture read correctly...";
  //           glBindTexture(GL_TEXTURE_2D, textures[0]);
  //           // tell OpenGL which texture you want to use
  //           glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA,
  //           glFormattedImage.width(),glFormattedImage.height(), 0, GL_RGBA,
  //           GL_UNSIGNED_BYTE, glFormattedImage.bits());
  //           glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  //           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  //           glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //           glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
  //           GL_LINEAR);
  //           glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
  //           GL_LINEAR);
  //           glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  //       }else{
  //           qDebug() << "Image 4 would not load";
  //       }

  glMatrixMode(GL_MODELVIEW);
}

void GLWidget::setOrtho() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float aspectRatio =
      static_cast<float>(viewWidth) / static_cast<float>(viewHeight);
  if (aspectRatio > 1.0) {
    glOrtho(-ortho, ortho, -ortho / aspectRatio, ortho / aspectRatio, -100,
            100);
  } else {
    glOrtho(-ortho * aspectRatio, ortho * aspectRatio, -ortho, ortho, -100,
            100);
  }
  glMatrixMode(GL_MODELVIEW);
}

void GLWidget::initializeGL() {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  setOrtho();
  glShadeModel(GL_SMOOTH); // use flat lighting
  glLightfv(GL_LIGHT0, GL_POSITION,
            light_position); // specify the position of the light
  glEnable(GL_LIGHT0);       // switch light #0 on
  glEnable(GL_LIGHTING);     // switch lighting on
  glEnable(GL_DEPTH_TEST);   // make sure depth buffer is switched on
  glEnable(GL_NORMALIZE);    // normalize normal vectors for safety
  loadTextures();
}

void GLWidget::paintGL() {
  // clear all pixels
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GLfloat mNow[16];

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  // glPushMatrix();
  glTranslatef(pullX, pullY, 0.0);
  glGetFloatv(GL_MODELVIEW_MATRIX, mNow);
  arc->getMatrix(mNow);
  glLoadMatrixf(mNow);

  // draw dancefloor
  glPushMatrix();
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  glBindTexture(GL_TEXTURE_2D, textures[11]);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glNormal3f(0.0f, 0.0f, 0.0f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-3.0f, -3.0f, 0.0f);
  glTexCoord2f(3.0f, 0.0f);
  glVertex3f(3.0f, -3.0f, 0.0f);
  glTexCoord2f(3.0f, 3.0f);
  glVertex3f(3.0f, 3.0f, 0.0f);
  glTexCoord2f(0.0f, 3.0f);
  glVertex3f(-3.0f, 3.0f, 0.0f);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();

  drawDiscoBall();

  glPopMatrix();

  if (axes) {
    drawAxes();
  }
  if (grid) {
    drawGrid(magenta, nGridlines);
  }

  if (wframe) {             // switch for wireframe, this is a hack
    glDisable(GL_LIGHTING); // disable lighting
    glColor4fv(red);        // set a consistant colour for the model
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // switch to line mode
    glPushMatrix();
    drawHuman(); // draw the model
    glPopMatrix();
    glPolygonMode(GL_FRONT_AND_BACK,
                  GL_FILL); // fill triangles for the rest of rendering
    glEnable(GL_LIGHTING);  // enable lighting for the rest of the rendering
  } else {
    // now do the drawing
    glPushMatrix();
    drawHuman();
    glPopMatrix();
  }
}

void GLWidget::resizeGL(int w, int h) {
  viewWidth = w;
  viewHeight = h;
  glViewport(0, 0, viewWidth, viewHeight);
  setOrtho();
  this->updateGL();
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {

  case Qt::Key_1:
    axes = !axes;
    this->updateGL();
    break;
  case Qt::Key_2:
    grid = !grid;
    this->updateGL();
    break;
  case Qt::Key_3:
    wframe = !wframe;
    this->updateGL();
    break;
  case Qt::Key_4:
    animation = !animation;
    this->updateGL();
    break;
  case '[':
    if (nGridlines > 1)
      nGridlines--;
    this->updateGL();
    break;
  case ']':
    nGridlines++;
    this->updateGL();
    break;
  // case Qt::Key_T: this->dumpTexture(); break;

  case Qt::Key_Q:
    qApp->exit();
    break;
  default:
    event->ignore();
    break;
  }
}

void GLWidget::wheelEvent(QWheelEvent *event) {
  float numDegrees = (static_cast<float>(event->delta()) / 8.0);
  float numSteps = numDegrees / (180.0 * (1.0 / ortho));
  ortho -= numSteps;
  if (ortho <= 0.0) {
    ortho = 0.0001;
  }
  viewWidth = this->width();
  viewHeight = this->height();
  setOrtho();
  this->updateGL();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    arc->startBall(event->x(), event->y(), this->width(), this->height());
    drag = true;
  }
  if (event->button() == Qt::RightButton) {
    moveX = static_cast<float>(event->x());
    moveY = static_cast<float>(event->y());
    pull = true;
  }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton) {
    if (drag == true) {
      arc->updateBall(event->x(), event->y(), this->width(), this->height());
      this->updateGL();
    }
  }
  if (pull == true) {
    viewHeight = this->height();
    viewWidth = this->width();
    float aspectRatio =
        static_cast<float>(viewWidth) / static_cast<float>(viewHeight);
    if (aspectRatio < 1.0) {
      pullX += (static_cast<float>(event->x()) - moveX) /
               (static_cast<float>(this->width()) /
                ((ortho * 2.0) * aspectRatio)); //	set the vector
      pullY -= (static_cast<float>(event->y()) - moveY) /
               (static_cast<float>(this->height()) / (ortho * 2.0));
    } else {
      pullX += (static_cast<float>(event->x()) - moveX) /
               (static_cast<float>(this->width()) /
                (ortho * 2.0)); //	set the vector
      pullY -=
          (static_cast<float>(event->y()) - moveY) /
          (static_cast<float>(this->height()) / ((ortho * 2.0) / aspectRatio));
    }
    moveX = static_cast<float>(event->x());
    moveY = static_cast<float>(event->y());
    this->updateGL();
  }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    drag = false;
    this->updateGL();
  }
  if (event->button() == Qt::RightButton) {
    pull = false;
    this->updateGL();
  }
}

void GLWidget::resetArc() {
  resetArcBall = !resetArcBall;
  arc->reset();
  this->updateGL();
}

void GLWidget::resetPos() {
  pullX = 0.0;
  pullY = 0.0;
  this->updateGL();
}

void GLWidget::resetZoom() {
  viewWidth = this->width();
  viewHeight = this->height();
  ortho = zoomFactor;
  setOrtho();
  this->updateGL();
}

void GLWidget::drawAxes() {
  glMatrixMode(GL_MODELVIEW); // switch to modelview
  glPushMatrix();             // push a matrix on the stack

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, grey); // set colour
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, grey); // but they do emit

  glBegin(GL_LINES); // start drawing lines
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(1.0, 0.0, 0.0); // x-axis
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 1.0, 0.0); // y-axis
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0, 1.0); // z-axis
  glEnd();

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
               black); // set reflective colour

  glPushMatrix();                 // push a matrix on the stack
  glTranslatef(0.0, 0.0, 1.0);    // move to end of z axis
  solidCone(0.02, 0.1, 32, 32);   // draw a solid cone for the z-axis
  glRotatef(90.0, 1.0, 0.0, 0.0); // rotate to draw in z=0 plane
  glTranslatef(0.1, 0.0, 0.0);    // move away slightly
  glScalef(0.001, 0.001, 0.001);  // scale down
  renderText(0, 0, 0, "Z");
  glPopMatrix(); // pop matrix off stack

  glPushMatrix();              // push a matrix on the stack
  glTranslatef(0.0, 1.0, 0.0); // move to end of y axis
  glRotatef(-90.0, 1.0, 0.0, 0.0);
  solidCone(0.02, 0.1, 32, 32);   // draw a solid cone for the y-axis
  glRotatef(90.0, 1.0, 0.0, 0.0); // rotate to draw in y=0 plane
  glTranslatef(0.1, 0.0, 0.0);    // move away slightly
  glScalef(0.001, 0.001, 0.001);  // scale down
  renderText(0, 0, 0, "Y");
  glPopMatrix(); // pop matrix off stack

  glPushMatrix();              // push a matrix on the stack
  glTranslatef(1.0, 0.0, 0.0); // move to end of x axis
  glRotatef(90.0, 0.0, 1.0, 0.0);
  solidCone(0.02, 0.1, 32, 32);   // draw a solid cone for the x-axis
  glRotatef(90.0, 0.0, 1.0, 0.0); // rotate to draw in x=0 plane
  glTranslatef(0.0, 0.1, 0.0);    // move away slightly
  glScalef(0.001, 0.001, 0.001);  // scale down
  renderText(0, 0, 0, "X");
  glPopMatrix(); // pop matrix off stack

  glMatrixMode(GL_MODELVIEW); // switch to modelview
  glPopMatrix();              // pop matrix off stack
} // drawAxes()

// function to draw grid on z = 0 plane
void GLWidget::drawGrid(GLfloat *colour, int nGridlines) {
  int x, z;
  // edges don't reflect
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, black);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, colour); // but they do emit
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  for (x = -nGridlines; x <= nGridlines; x++) { // for each x
    if (x % 12)
      glLineWidth(1.0);
    else
      glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex3i(x, 0, -nGridlines);
    glVertex3i(x, 0, +nGridlines);
    glEnd();
  }                                             // for each x
  for (z = -nGridlines; z <= nGridlines; z++) { // for each y
    if (z % 12)
      glLineWidth(1.0);
    else
      glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex3i(-nGridlines, 0, z);
    glVertex3i(+nGridlines, 0, z);
    glEnd();
  } // for each y
  glLineWidth(1.0);
  glPopMatrix();
  // stop emitting, otherwise other objects will emit the same colour
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
} // drawGrid()

void GLWidget::solidCone(GLdouble base, GLdouble height, GLint slices,
                         GLint stacks) {
  glBegin(GL_LINE_LOOP);
  GLUquadricObj *quadric = gluNewQuadric();
  gluQuadricDrawStyle(quadric, GLU_FILL);
  gluCylinder(quadric, base, 0, height, slices, stacks);

  gluDeleteQuadric(quadric);
  glEnd();
}

void GLWidget::idle() {
  if (animation == true) {
    if (time < nframes)
      time++;
    else
      time = 0;
    animate(time);
  }
  this->updateGL();
}

void GLWidget::drawHuman() { // drawHuman()
  glPushMatrix();

  glTranslatef(posn_x, 2.0f, posn_z);
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  glRotatef(bodyTheta, 0.0f, 1.0f, 0.0f);

  // PELVIS BEGINNING

  // pelvis
  glPushMatrix();

  glEnable(GL_TEXTURE_2D);
  glRotatef(270, 1.0, 0.0, 0.0);
  glBindTexture(GL_TEXTURE_2D, textures[4]);
  objects->drawSphere(0.7f, 32, 32);
  glDisable(GL_TEXTURE_2D);

  // CHEST BEGINNING

  //  chest
  glPushMatrix();

  glTranslatef(0.0f, 0.0f, 0.5f);
  glRotatef(90, 1.0, 0.0, 0.0);
  glBindTexture(GL_TEXTURE_2D, textures[2]);
  glEnable(GL_TEXTURE_2D);
  objects->drawSphere(0.5f, 32, 32);
  glDisable(GL_TEXTURE_2D);

  // LEFT ARM BEGINNING

  // left shoulder
  glPushMatrix();

  glTranslatef(0.5f, 0.4f, 0.0f);
  glBindTexture(GL_TEXTURE_2D, textures[2]);
  glEnable(GL_TEXTURE_2D);
  objects->drawSphere(0.25f, 32, 32);
  glDisable(GL_TEXTURE_2D);

  // left upper arm
  glPushMatrix();

  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glEnable(GL_TEXTURE_2D);
  objects->drawCylinder(0.15f, 0.7f, 32);
  glDisable(GL_TEXTURE_2D);

  // left elbow joint

  glPushMatrix();

  glTranslatef(0.0f, 0.0f, 0.75f);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glEnable(GL_TEXTURE_2D);
  objects->drawSphere(0.15f, 32, 32);
  glDisable(GL_TEXTURE_2D);

  // left forearm
  glPushMatrix();

  glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glEnable(GL_TEXTURE_2D);
  objects->drawCylinder(0.15f, 0.7f, 32);
  glDisable(GL_TEXTURE_2D);

  // left hand
  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();

  glTranslatef(0.0f, 0.0f, 0.75f);
  glTranslatef(0.15f, -0.15f, 0.0f);

  glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);

  GLUquadricObj *righthand = gluNewQuadric();
  glBindTexture(GL_TEXTURE_2D, textures[8]);
  glEnable(GL_TEXTURE_2D);
  gluQuadricTexture(righthand, GL_TRUE);
  gluQuadricDrawStyle(righthand, GLU_FILL);
  glPolygonMode(GL_FRONT, GL_FILL);
  gluQuadricNormals(righthand, GLU_SMOOTH);
  gluCylinder(righthand, 0.15f, 0.15f, 0.4f, 32, 100);
  glDisable(GL_TEXTURE_2D);

  glPopMatrix();

  glPopMatrix();

  glPopMatrix();

  glPopMatrix();

  glPopMatrix();

  // LEFT ARM END

  // RIGHT ARM Beginning

  // right shoulder
  glPushMatrix();

  glTranslatef(-0.5f, 0.4f, 0.0f);
  glBindTexture(GL_TEXTURE_2D, textures[2]);
  glEnable(GL_TEXTURE_2D);
  objects->drawSphere(0.25f, 32, 32);
  glDisable(GL_TEXTURE_2D);

  //  upper arm
  glPushMatrix();

  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(315.0f, 0.0f, 1.0f, 0.0f);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glEnable(GL_TEXTURE_2D);
  objects->drawCylinder(0.15f, 0.7f, 32);
  glDisable(GL_TEXTURE_2D);

  // left elbow joint

  glPushMatrix();

  glTranslatef(0.0f, 0.0f, 0.75f);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glEnable(GL_TEXTURE_2D);
  objects->drawSphere(0.15f, 32, 32);
  glDisable(GL_TEXTURE_2D);

  // left forearm
  glPushMatrix();

  glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glEnable(GL_TEXTURE_2D);
  objects->drawCylinder(0.15f, 0.7f, 32);
  glDisable(GL_TEXTURE_2D);

  // left hand
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);

  glTranslatef(0.0f, 0.0f, 0.75f);
  glTranslatef(0.15f, -0.15f, 0.0f);

  glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);

  GLUquadricObj *lefthand = gluNewQuadric();
  glBindTexture(GL_TEXTURE_2D, textures[9]);
  glEnable(GL_TEXTURE_2D);
  gluQuadricTexture(lefthand, GL_TRUE);
  gluQuadricDrawStyle(lefthand, GLU_FILL);
  glPolygonMode(GL_FRONT, GL_FILL);
  gluQuadricNormals(lefthand, GLU_SMOOTH);
  gluCylinder(lefthand, 0.15f, 0.15f, 0.4f, 32, 100);
  glDisable(GL_TEXTURE_2D);

  glPopMatrix();

  glPopMatrix();

  glPopMatrix();

  glPopMatrix();

  glPopMatrix();

  // RIGHT ARM END

  // HEAD BEGINNING

  // neck
  glPushMatrix();
  glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glEnable(GL_TEXTURE_2D);
  objects->drawCylinder(0.2f, 0.7f, 32);
  glDisable(GL_TEXTURE_2D);

  // head
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);

  glTranslatef(0.0f, 0.0f, 0.70f);
  glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
  GLUquadricObj *cylinder = gluNewQuadric();
  glBindTexture(GL_TEXTURE_2D, textures[6]);
  glEnable(GL_TEXTURE_2D);
  gluQuadricTexture(cylinder, GL_TRUE);
  gluQuadricDrawStyle(cylinder, GLU_FILL);
  glPolygonMode(GL_FRONT, GL_FILL);
  gluQuadricNormals(cylinder, GLU_SMOOTH);
  gluCylinder(cylinder, 0.3f, 0.3f, 0.7f, 32, 100);
  glDisable(GL_TEXTURE_2D);

  // jaw
  glPushMatrix();
  glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glEnable(GL_TEXTURE_2D);
  objects->drawSphere(0.3f, 32, 32);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();

  // scalp
  glPushMatrix();
  glTranslatef(0.0f, 0.0f, 0.65f);
  glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  glBindTexture(GL_TEXTURE_2D, textures[5]);
  glEnable(GL_TEXTURE_2D);
  objects->drawSphere(0.3f, 32, 32);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();

  // nose
  glPushMatrix();
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glTranslatef(0.0, 0.5, 0.25);
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  glEnable(GL_TEXTURE_2D);
  objects->drawCylinder(0.05f, 0.2f, 32);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();

  // left eye
  glPushMatrix();
  glTranslatef(-0.1f, 0.3f, 0.35f);
  glRotatef(100.0f, 0.0, 1.0, 1.0);
  glBindTexture(GL_TEXTURE_2D, textures[3]);
  glEnable(GL_TEXTURE_2D);
  objects->drawSphere(0.1f, 32, 32);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();

  // right eye
  glPushMatrix();
  glTranslatef(0.1f, 0.3f, 0.35f);
  glRotatef(80.0f, 0.0, 1.0, 1.0);
  glBindTexture(GL_TEXTURE_2D, textures[3]);
  glEnable(GL_TEXTURE_2D);
  objects->drawSphere(0.1f, 32, 32);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();

  // mouth
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glPushMatrix();
  glTranslatef(0.0f, 0.3f, 0.055f);
  glEnable(GL_TEXTURE_2D);
  glRotatef(90, 0.0f, 0.0f, 1.0f);
  objects->drawSphere(0.175f, 32, 32);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();

  glPopMatrix();

  glPopMatrix();

  // HEAD END

  glPopMatrix();

  // CHEST END

  // LEFT LEG BEGINNING

  // left thigh
  glPushMatrix();
  glRotatef(left_leg, 1.0f, 0.0f, 0.0f);
  glBindTexture(GL_TEXTURE_2D, textures[7]);
  glTranslatef(0.3f, 0.0f, -1.0f);
  glEnable(GL_TEXTURE_2D);
  objects->drawCylinder(0.3f, 0.7f, 32);
  glDisable(GL_TEXTURE_2D);

  // left knee
  glPushMatrix();
  glRotatef(-left_knee, 1.0f, 0.0f, 0.0f);
  glBindTexture(GL_TEXTURE_2D, textures[7]);
  glEnable(GL_TEXTURE_2D);
  objects->drawSphere(0.3f, 32, 32);
  glDisable(GL_TEXTURE_2D);

  // left shin
  glPushMatrix();
  glBindTexture(GL_TEXTURE_2D, textures[7]);
  glTranslatef(0.0f, 0.0f, -0.75f);
  glEnable(GL_TEXTURE_2D);
  objects->drawCylinder(0.3f, 0.7f, 32);
  glDisable(GL_TEXTURE_2D);

  // left foot

  glPushMatrix();
  glTranslatef(0.0f, 0.0f, -0.15f);
  glTranslatef(0.0f, -0.55f, 0.0f);

  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

  glBindTexture(GL_TEXTURE_2D, textures[10]);
  glTranslatef(0.0f, 0.0f, -0.75f);
  glEnable(GL_TEXTURE_2D);
  objects->drawCylinder(0.2f, 0.75f, 32);
  glDisable(GL_TEXTURE_2D);

  glPopMatrix();

  glPopMatrix();

  glPopMatrix();

  glPopMatrix();

  // LEFT LEG END

  // RIGHT LEG BEGINNING

  glPushMatrix();
  glRotatef(right_leg, 1.0f, 0.0f, 0.0f);

  glBindTexture(GL_TEXTURE_2D, textures[7]);
  glTranslatef(-0.3f, 0.0f, -1.0f);
  glEnable(GL_TEXTURE_2D);
  objects->drawCylinder(0.3f, 0.7f, 32);
  glDisable(GL_TEXTURE_2D);

  // right knee
  glPushMatrix();
  glRotatef(-right_knee, 1.0f, 0.0f, 0.0f);

  glBindTexture(GL_TEXTURE_2D, textures[7]);
  glEnable(GL_TEXTURE_2D);
  objects->drawSphere(0.3f, 32, 32);
  glDisable(GL_TEXTURE_2D);

  // left shin
  glPushMatrix();
  glBindTexture(GL_TEXTURE_2D, textures[7]);
  glTranslatef(0.0f, 0.0f, -0.75f);
  glEnable(GL_TEXTURE_2D);
  objects->drawCylinder(0.3f, 0.7f, 32);
  glDisable(GL_TEXTURE_2D);

  // right foot

  glPushMatrix();
  glTranslatef(0.0f, 0.0f, -0.15f);
  glTranslatef(0.0f, -0.55f, 0.0f);

  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

  glBindTexture(GL_TEXTURE_2D, textures[10]);
  glTranslatef(0.0f, 0.0f, -0.75f);
  glEnable(GL_TEXTURE_2D);
  objects->drawCylinder(0.2f, 0.75f, 32);
  glDisable(GL_TEXTURE_2D);

  glPopMatrix();

  glPopMatrix();

  glPopMatrix();

  glPopMatrix();

  //RIGHT LEG END

  glPopMatrix();

  // PELVIS END

  glPopMatrix();

} // drawHuman()

void GLWidget::drawDiscoBall() {
  glPushMatrix();
  glRotatef(bodyTheta, 0.0f, 1.0f, 0.0f);
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  glTranslatef(0.0f, 0.0f, -7.0f);
  glBindTexture(GL_TEXTURE_2D, textures[12]);
  glEnable(GL_TEXTURE_2D);
  objects->drawSphere(1.0f, 32, 100);
  glDisable(GL_TEXTURE_2D);

  // draw beam of light occastionally.
  if (time % 2 == 0) {
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.70f);
    glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);

    GLUquadricObj *lightbeam = gluNewQuadric();
    glBindTexture(GL_TEXTURE_2D, textures[13]);
    glEnable(GL_TEXTURE_2D);
    gluQuadricTexture(lightbeam, GL_TRUE);
    gluQuadricDrawStyle(lightbeam, GLU_FILL);
    glPolygonMode(GL_FRONT, GL_FILL);
    gluQuadricNormals(lightbeam, GLU_SMOOTH);
    gluCylinder(lightbeam, 0.0f, 0.3f, 7.0f, 32, 100);
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
  }

  glPopMatrix();
}

void GLWidget::animate(int time) {
  /* animate() */
  for (int timeStep = time; timeStep < nframes; timeStep += 5) {
      this->time = time;
      computePosition(timeStep);
      computeBodyTheta();
      computeLegs();
      drawHuman();
      drawDiscoBall();
      QWidget::update();
      updateGL();
  }

} /* animate() */

void GLWidget::computePosition(int time) {
  float thetaDeg = ((float) time / 600.0) * 360;
  float thetaRad = ((float) time / 600.0) * 2.0 * M_PI;
  posn_x = cos(thetaRad);
  posn_z = sin(thetaRad);

  glPushMatrix();
  glRotatef(thetaRad, 0.0, 0.0, 1.0);
  glTranslatef(posn_x, posn_z, 0.0);
  glPopMatrix();
}

void GLWidget::computeLegs() {
  if (left_leg == 20.0) {
    left_max = true;
    left_min = false;

    right_min = true;
    right_max = false;
    }

  if (left_leg == -20.0) {
    left_min = true;      this->time = time;

    left_max = false;

    right_min = false;
    right_max = true;
  }

  if (left_max) {
    left_leg--;
    left_knee++;

    right_leg++;
    right_knee--;
  } else if (left_min) {
    left_leg++;
    left_knee--;

    right_leg--;
    right_knee++;
  }
}

void GLWidget::computeBodyTheta() {
  float angle = atan(posn_x / posn_z) * 180 / M_PI;

  if (posn_z > 0) {
    angle += 180.0f;
  }

  bodyTheta = angle;
}

void GLWidget::saveImage() {
  QImage image = this->grabFrameBuffer();
  image.save("your_image.jpeg", "jpeg");
  if (!image.isNull()) {
    qDebug() << "Image saved successfully";
  }
}

GLWidget::~GLWidget() {
  delete objects;
  delete arc;
}
