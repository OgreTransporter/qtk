/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: MainWindow for Qtk application                                      ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <qtk/qtkapi.h>

#include "examplescene.h"
#include "qtkmainwindow.h"
#include "ui_qtkmainwindow.h"

MainWindow * MainWindow::mainWindow_ = Q_NULLPTR;

MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent) {
  ui_ = new Ui::MainWindow;
  setObjectName("MainWindow");
  // For use in design mode using Qt Creator
  // + We can use the `ui` member to access nested widgets by name
  ui_->setupUi(this);
  ui_->menuView->addAction(ui_->toolBar->toggleViewAction());

  // Initialize static container for all active QtkWidgets
  auto qtkWidgets = findChildren<Qtk::QtkWidget *>();
  for(auto & qtkWidget : qtkWidgets) {
    qtkWidget->setScene(new ExampleScene);
    views_.emplace(qtkWidget->getScene()->getSceneName(), qtkWidget);
    ui_->menuView->addAction(qtkWidget->getActionToggleConsole());
    connect(
        qtkWidget->getScene(), &Qtk::Scene::sceneUpdated, this,
        &MainWindow::refreshScene);
  }

  auto docks = findChildren<QDockWidget *>();
  for(auto & dock : docks) {
    addDockWidget(Qt::RightDockWidgetArea, dock);
    ui_->menuView->addAction(dock->toggleViewAction());
  }

  // Set the window icon used for Qtk.
  setWindowIcon(Qtk::getIcon());
}

MainWindow::~MainWindow() {
  delete ui_;
}

void MainWindow::refreshScene(QString sceneName) {
  ui_->qtk__TreeView->updateView(getQtkWidget(sceneName)->getScene());
}
