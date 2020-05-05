#ifndef MOBILEPACKAGEELEMENT_H
#define MOBILEPACKAGEELEMENT_H

#include <QObject>

namespace Esri
{
namespace ArcGISRuntime
{
class LocatorTask;
class Map;
class MobileMapPackage;
}
}

class MobilePackageElement : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title NOTIFY onTitleChanged)

    Q_PROPERTY(QString description READ description NOTIFY onDescriptionChanged)

public:
    explicit MobilePackageElement(QObject *parent = nullptr);

    QString title() const;
    QString description() const;

    Esri::ArcGISRuntime::Map* focusMap() const;
    Esri::ArcGISRuntime::LocatorTask* locatorTask() const;

signals:
    void onTitleChanged();
    void onDescriptionChanged();


private:
    Esri::ArcGISRuntime::MobileMapPackage* m_mobileMapPackage = nullptr;
    QString m_title;
    QString m_description;
};

#endif // MOBILEPACKAGEELEMENT_H
