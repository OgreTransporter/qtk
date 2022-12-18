/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: MainWindow for creating an example Qt application                   ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <QMainWindow>
#include <QWindow>

#include "debugconsole.h"
#include "ui_debugconsole.h"

using namespace Qtk;

DebugConsole::DebugConsole(QWidget * owner, const QString & key) :
    DebugConsole(owner, key, key + "Debugger") {}

DebugConsole::DebugConsole(
    QWidget * owner, const QString & key, const QString & name) {
  ui_ = new Ui::DebugConsole;
  ui_->setupUi(this);
  setObjectName(name);
  mConsole = ui_->textEdit;
  setWidget(mConsole);
  setWindowTitle(name + " Debug Console");

  auto qtkWidget = dynamic_cast<QtkWidget *>(owner);
  if(qtkWidget) {
    connect(qtkWidget, &QtkWidget::sendLog, this, &DebugConsole::sendLog);
    sendLog(
        "Debug console (" + name + ") attached to QtkWidget: '"
        + qtkWidget->objectName() + "'");
    sendLog("Test\nLogging\t\n\tStuff", Status);
    sendLog("Test\nLogging\t\n\tStuff", Debug);
    sendLog("Test\nLogging\t\n\tStuff", Warn);
    sendLog("Test\nLogging\t\n\tStuff", Error);
    sendLog(
        "Test\nLogging\t\n\tStuff that is really long and will wrap around but "
        "it might not you don't know until you try",
        Fatal);
  }
}
