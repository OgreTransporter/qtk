/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Toolbox plugin for object details and options                       ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
################################################################################
*/

#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QDesignerExportWidget>
#include <QDockWidget>

namespace Ui {
  class ToolBox;
}

namespace Qtk {
  class QDESIGNER_WIDGET_EXPORT ToolBox : public QDockWidget {
      Q_OBJECT

    public:
      explicit ToolBox(QWidget * parent = nullptr);
      ~ToolBox();

    private:
      Ui::ToolBox * ui;
  };
}  // namespace Qtk

#endif  // TOOLBOX_H
