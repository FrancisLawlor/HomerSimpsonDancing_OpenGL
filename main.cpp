#include "mainwindow.h"
#include <QApplication>
#include <QtOpenGL>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MainWindow w;
  w.setWindowTitle("COMP30020-Lab7");
  w.show();

  return app.exec();
}
