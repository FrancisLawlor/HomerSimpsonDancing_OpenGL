#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  QFile::copy(":/eye.jpg", QDir::homePath() + QString("/Desktop/eye.jpg"));
  QFile::copy(":/comics.jpg",
              QDir::homePath() + QString("/Desktop/comics.jpg"));
  QFile::copy(":/texture.jpg",
              QDir::homePath() + QString("/Desktop/texture.jpg"));

  opengl = new GLWidget();
  ui->setupUi(this);
  ui->glScrollArea->setWidgetResizable(true);
  ui->glScrollArea->setWidget(opengl);
  opengl->setFocus();

  QObject::connect(ui->arcButton, SIGNAL(clicked()), this,
                   SLOT(resetArcBall()));
  QObject::connect(ui->zoomButton, SIGNAL(clicked()), this, SLOT(resetZoom()));
  QObject::connect(ui->dragButton, SIGNAL(clicked()), this,
                   SLOT(resetPosition()));
  QObject::connect(ui->actionSave_Image, SIGNAL(triggered()), this,
                   SLOT(saveImage()));
  QObject::connect(ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

  Q_INIT_RESOURCE(resources);
}

void MainWindow::resetArcBall() { opengl->resetArc(); }

void MainWindow::resetPosition() { opengl->resetPos(); }

void MainWindow::resetZoom() { opengl->resetZoom(); }

void MainWindow::saveImage() { opengl->saveImage(); }

void MainWindow::closeEvent(QCloseEvent *event) { qApp->quit(); }

MainWindow::~MainWindow() {
  delete opengl;
  delete ui;
}
