/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Collection of widget plugins for Qt Designer                        ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
################################################################################
*/

#ifndef QTK_WIDGETPLUGINCOLLECTION_H
#define QTK_WIDGETPLUGINCOLLECTION_H

#include <QDesignerCustomWidgetCollectionInterface>

class WidgetPluginCollection :
    public QObject,
    public QDesignerCustomWidgetCollectionInterface {
    Q_OBJECT
    // Since we're exporting a collection, this is the only plugin metadata
    // needed.
    Q_PLUGIN_METADATA(IID "com.Klips.WidgetPluginCollection")
    // Tell Qt Object system that we're implementing an interface.
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)

  public:
    explicit WidgetPluginCollection(QObject * parent = nullptr);
    [[nodiscard]] QList<QDesignerCustomWidgetInterface *> customWidgets() const;

  private:
    QList<QDesignerCustomWidgetInterface *> m_collection;
    QString m_collectionName;
};

#endif  // QTK_WIDGETPLUGINCOLLECTION_H
