/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Main program for practice using Qt6 widgets and OpenGL              ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <QApplication>

#include "qtkmainwindow.h"

int main(int argc, char * argv[]) {
  QApplication a(argc, argv);

  auto window = MainWindow::getMainWindow();
  window->show();

  return QApplication::exec();
}
