
// Copyright 2019 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

#ifndef GEOINTRECON_H
#define GEOINTRECON_H

namespace Esri
{
namespace ArcGISRuntime
{
class Map;
class MapQuickView;
class MobileMapPackage;
}
}

class MobilePackageElement;

#include <QObject>

class GEOINTRecon : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Esri::ArcGISRuntime::MapQuickView* mapView READ mapView WRITE setMapView NOTIFY mapViewChanged)
    Q_PROPERTY(MobilePackageElement* packageElement READ packageElement WRITE setPackageElement NOTIFY packageElementChanged)

public:
    explicit GEOINTRecon(QObject* parent = nullptr);
    ~GEOINTRecon() override;

    Q_INVOKABLE void centerMap(const QString &location);
    Q_INVOKABLE void showMap();

signals:
    void mapViewChanged();
    void packageElementChanged();

private:
    Esri::ArcGISRuntime::MapQuickView* mapView() const;
    void setMapView(Esri::ArcGISRuntime::MapQuickView* mapView);

    MobilePackageElement* packageElement() const;
    void setPackageElement(MobilePackageElement* packageElement);

    Esri::ArcGISRuntime::Map* m_map = nullptr;
    Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;
    Esri::ArcGISRuntime::MobileMapPackage* m_mobileMapPackage = nullptr;
    MobilePackageElement* m_packageElement = nullptr;
};

#endif // GEOINTRECON_H
