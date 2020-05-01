
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
#include "MGRSGrid.h"
#include "MobileMapPackage.h"
#include "FeatureLayer.h"
#include "FeatureQueryResult.h"
#include "FeatureTable.h"

#include "MobilePackageElement.h"
#include "MobilePackageStore.h"
#include "OperationalLayerListModel.h"
#include "RegularLocator.h"

#include <QException>
#include <QUrl>
#include <QUuid>

using namespace Esri::ArcGISRuntime;

GEOINTRecon::GEOINTRecon(QObject* parent /* = nullptr */):
    QObject(parent),
    m_map(new Map(Basemap::openStreetMap(this), this)),
    m_layerListModel(new OperationalLayerListModel(this))
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

    // Set the MGRS grid
    m_mgrsGrid = new MGRSGrid(this);
    m_mapView->setGrid(m_mgrsGrid);

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

OperationalLayerListModel* GEOINTRecon::layerListModel() const
{
    if (nullptr == m_mapView->map())
    {
        return nullptr;
    }

    return m_layerListModel;
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
    if (nullptr != m_packageElement)
    {
        Map* focusMap = m_packageElement->focusMap();
        if (nullptr != focusMap)
        {
            m_mapView->setMap(focusMap);

            // Update the layer list model
            LayerListModel* layerListModel = focusMap->operationalLayers();
            m_layerListModel->updateModel(layerListModel);

            // Iterate features
            //visitMap(focusMap);
        }
    }

    // Emit map view changed
    emit mapViewChanged();

    // Emit layer list model changed
    emit layerListModelChanged();
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
    // Create a new instance for managing lambda slots lifetime
    // This instance is deleted when the signal was fired once!
    QObject* context = new QObject();
    connect(table, &FeatureTable::queryFeaturesCompleted, context, [context, table](QUuid, FeatureQueryResult* rawQueryResult)
    {
        try
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
        }
        catch (QException& ex)
        {
            qCritical() << ex.what();
        }

        // Deletion of this instance destroys the registered lambda slot
        delete context;
    });

    QueryParameters parameters;
    table->queryFeatures(parameters);
}
