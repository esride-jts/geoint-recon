
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

#include "GEOINTRecon.h"

#include "Basemap.h"
#include "Map.h"
#include "MapQuickView.h"
#include "MobileMapPackage.h"
#include "FeatureLayer.h"
#include "FeatureQueryResult.h"
#include "FeatureTable.h"

#include "MobilePackageElement.h"
#include "MobilePackageStore.h"
#include "RegularLocator.h"

#include <QUrl>
#include <QUuid>

using namespace Esri::ArcGISRuntime;

GEOINTRecon::GEOINTRecon(QObject* parent /* = nullptr */):
    QObject(parent),
    m_map(new Map(Basemap::openStreetMap(this), this))
{
}

GEOINTRecon::~GEOINTRecon()
{
}

MapQuickView* GEOINTRecon::mapView() const
{
    return m_mapView;
}

// Set the view (created in QML)
void GEOINTRecon::setMapView(MapQuickView* mapView)
{
    if (!mapView || mapView == m_mapView)
    {
        return;
    }

    m_mapView = mapView;

    m_mapView->setMap(m_map);
    emit mapViewChanged();
}

MobilePackageElement* GEOINTRecon::packageElement() const
{
    return m_packageElement;
}

void GEOINTRecon::setPackageElement(MobilePackageElement *packageElement)
{
    m_packageElement = packageElement;
    emit packageElementChanged();
}

void GEOINTRecon::centerMap(const QString &location)
{
    RegularLocator locator;
    WGS84Location wgs84Location = locator.locate(location);
    if (wgs84Location.empty())
    {
        return;
    }

    Point mapCenter(wgs84Location.longitude(), wgs84Location.latitude(), SpatialReference::wgs84());
    m_mapView->setViewpointCenter(mapCenter);
}

void GEOINTRecon::showMap()
{
    // Update the map views map
    // and emit map view changed
    if (nullptr != m_packageElement)
    {
        Map* focusMap = m_packageElement->focusMap();
        if (nullptr != focusMap)
        {
            m_mapView->setMap(focusMap);

            // Iterate features
            visitMap(focusMap);
        }
    }

    emit mapViewChanged();
}

void GEOINTRecon::visitMap(Esri::ArcGISRuntime::Map *map) const
{
    LayerListModel* layerListModel = map->operationalLayers();
    for (int index = 0; index < layerListModel->size(); index++)
    {
        Layer* layer = layerListModel->at(index);
        FeatureLayer* featureLayer = dynamic_cast<FeatureLayer*>(layer);
        if (nullptr != featureLayer)
        {
            qDebug() << "Feature Layer: " << featureLayer->name();
            FeatureTable* table = featureLayer->featureTable();
            visitFeatureTable(table);
        }
    }
}

void GEOINTRecon::visitFeatureTable(Esri::ArcGISRuntime::FeatureTable *table) const
{
    connect(table, &FeatureTable::queryFeaturesCompleted, this, [table](QUuid, FeatureQueryResult* rawQueryResult)
    {
        qlonglong geometryCount = 0;
        QScopedPointer<FeatureQueryResult> queryResult(rawQueryResult);
        if (queryResult)
        {
            FeatureIterator featureIterator = queryResult->iterator();
            while (featureIterator.hasNext())
            {
                QScopedPointer<Feature> feature(featureIterator.next());
                if (!feature->geometry().isEmpty())
                {
                    geometryCount++;
                }
            }
        }

        qDebug() << table->displayName() << " with " << geometryCount << " geometries.";
    });

    QueryParameters parameters;
    table->queryFeatures(parameters);
}
