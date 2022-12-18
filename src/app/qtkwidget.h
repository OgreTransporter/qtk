/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Main window for Qt6 OpenGL widget application                       ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/
#ifndef QTK_QTKWIDGET_H
#define QTK_QTKWIDGET_H

#include <iostream>

#include <QDockWidget>
#include <QMatrix4x4>
#include <QOpenGLDebugLogger>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QPlainTextEdit>

#include "qtk/qtkapi.h"
#include "qtk/scene.h"

namespace Qtk {
  class DebugConsole;

  /**
   * QtkWidget class to define required QOpenGLWidget functionality.
   *
   * This object has a Scene attached which manages the objects to render.
   * Client input is passed through this widget to control the camera view.
   */
  class QtkWidget : public QOpenGLWidget, protected QOpenGLFunctions {
      Q_OBJECT;

    public:
      /*************************************************************************
       * Contructors / Destructors
       ************************************************************************/

      /**
       * Qt Designer will call this ctor when creating this widget as a child.
       *
       * @param parent Pointer to a parent widget for this QtkWidget or nullptr.
       */
      explicit QtkWidget(QWidget * parent = nullptr) :
          QtkWidget(parent, "QtkWidget") {}

      /**
       * Default construct a QtkWidget.
       *
       * @param parent Pointer to a parent widget or nullptr if no parent.
       * @param name An objectName for the new QtkWidget.
       */
      explicit QtkWidget(QWidget * parent, const QString & name);

      /**
       * Construct a custom QtkWidget.
       *
       * @param parent Pointer to a parent widget or nullptr if no parent.
       * @param name An objectName for the new QtkWidget.
       * @param scene Pointer to a custom class inheriting from Qtk::Scene.
       */
      QtkWidget(QWidget * parent, const QString & name, Qtk::Scene * scene);

      ~QtkWidget() override;
      /*************************************************************************
       * Public Methods
       ************************************************************************/
      QAction * getActionToggleConsole();

    private:
      /*************************************************************************
       * Private Methods
       ************************************************************************/

      // clang-format off
    void teardownGL() { /* Nothing to teardown yet... */ }
      // clang-format on

    public:
      /*************************************************************************
       * Public Inherited Virtual Methods
       ************************************************************************/

      /**
       * Called when the widget is first constructed.
       */
      void initializeGL() override;

      /**
       * Called when the application window is resized.
       *
       * @param width The new width of the window.
       * @param height The new height of the window.
       */
      void resizeGL(int width, int height) override;

      /**
       * Called when OpenGL repaints the widget.
       */
      void paintGL() override;

      /*************************************************************************
       * Accessors
       ************************************************************************/

      inline Qtk::Scene * getScene() { return mScene; }

      /*************************************************************************
       * Setters
       ************************************************************************/

      void setScene(Qtk::Scene * scene);
    public slots:
      /**
       * Toggle visibility of the DebugConsole associated with this QtkWidget.
       */
      void toggleConsole();

    protected slots:
      /*************************************************************************
       * Qt Slots
       ************************************************************************/

      /**
       * Called when the `frameSwapped` signal is caught.
       * See definition of initializeGL()
       */
      void update();

#ifdef QTK_DEBUG
      /**
       * Called when the `messageLogged` signal is caught.
       * See definition of initializeGL()
       *
       * @param msg The message logged.
       */
      void messageLogged(const QOpenGLDebugMessage & msg);
#endif
    public:
    signals:
      void sendLog(
          const QString & message, Qtk::DebugContext context = Qtk::Status);

    protected:
      /*************************************************************************
       * Protected Methods
       ************************************************************************/

      void keyPressEvent(QKeyEvent * event) override;
      void keyReleaseEvent(QKeyEvent * event) override;
      void mousePressEvent(QMouseEvent * event) override;
      void mouseReleaseEvent(QMouseEvent * event) override;

    private:
      /*************************************************************************
       * Private Methods
       ************************************************************************/

      static void updateCameraInput();

#ifdef QTK_DEBUG
      void printContextInformation();
      QOpenGLDebugLogger * mDebugLogger;
#endif
      /*************************************************************************
       * Private Members
       ************************************************************************/

      bool mConsoleActive = false;
      Qtk::Scene * mScene;
      Qtk::DebugConsole * mConsole;
  };
}  // namespace Qtk

#endif  // QTK_QTKWIDGET_H
