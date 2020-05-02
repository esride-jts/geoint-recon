
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
class FeatureTable;
class LayerListModel;
class Map;
class MapQuickView;
class MGRSGrid;
class MobileMapPackage;
}
}

class MobilePackageElement;
class OperationalLayerListModel;
class ThreatFactory;

#include <QObject>

class GEOINTRecon : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString mapCenter READ mapCenter)

    Q_PROPERTY(Esri::ArcGISRuntime::MapQuickView* mapView READ mapView WRITE setMapView NOTIFY mapViewChanged)
    Q_PROPERTY(MobilePackageElement* packageElement READ packageElement WRITE setPackageElement NOTIFY packageElementChanged)
    Q_PROPERTY(OperationalLayerListModel* layerListModel READ layerListModel NOTIFY layerListModelChanged)

public:
    explicit GEOINTRecon(QObject* parent = nullptr);
    ~GEOINTRecon() override;

    Q_INVOKABLE void centerMap(const QString &location, const QString &distance = "", const QString &linearUnit = "", const QString &direction = "");
    Q_INVOKABLE void showMap();

signals:
    void mapViewChanged();
    void packageElementChanged();
    void layerListModelChanged();

private:
    Esri::ArcGISRuntime::MapQuickView* mapView() const;
    void setMapView(Esri::ArcGISRuntime::MapQuickView* mapView);

    QString mapCenter() const;

    MobilePackageElement* packageElement() const;
    void setPackageElement(MobilePackageElement* packageElement);

    OperationalLayerListModel* layerListModel() const;

    void setupResultOverlay();
    void visitMap(Esri::ArcGISRuntime::Map* map) const;
    void visitFeatureTable(Esri::ArcGISRuntime::FeatureTable* table) const;

    Esri::ArcGISRuntime::Map* m_map = nullptr;
    Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;
    Esri::ArcGISRuntime::MGRSGrid* m_mgrsGrid = nullptr;
    Esri::ArcGISRuntime::MobileMapPackage* m_mobileMapPackage = nullptr;
    MobilePackageElement* m_packageElement = nullptr;
    OperationalLayerListModel* m_layerListModel = nullptr;
    ThreatFactory* m_threatFactory = nullptr;
};

#endif // GEOINTRECON_H
