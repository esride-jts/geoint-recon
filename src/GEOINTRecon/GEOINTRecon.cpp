
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
#include "CoordinateFormatter.h"
#include "FeatureLayer.h"
#include "FeatureQueryResult.h"
#include "FeatureTable.h"
#include "GeometryEngine.h"
#include "Map.h"
#include "MapQuickView.h"
#include "MGRSGrid.h"
#include "MobileMapPackage.h"
#include "SimpleFillSymbol.h"
#include "SimpleRenderer.h"

#include "MobilePackageElement.h"
#include "MobilePackageStore.h"
#include "OperationalLayerListModel.h"
#include "ObservationFactory.h"
#include "RegularLocator.h"
#include "ThreatFactory.h"

#include <QException>
#include <QUrl>
#include <QUuid>

using namespace Esri::ArcGISRuntime;

GEOINTRecon::GEOINTRecon(QObject* parent /* = nullptr */):
    QObject(parent),
    m_map(new Map(Basemap::openStreetMap(this), this)),
    m_layerListModel(new OperationalLayerListModel(this)),
    m_observationFactory(new ObservationFactory(this)),
    m_regularLocator(new RegularLocator(this)),
    m_threatFactory(new ThreatFactory(this))
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

    // Set the background color
    BackgroundGrid backgroundGrid;
    backgroundGrid.setColor(QColor("#d3c2a6"));
    backgroundGrid.setGridLineWidth(0);
    m_mapView->setBackgroundGrid(backgroundGrid);

    // Set the MGRS grid
    m_mgrsGrid = new MGRSGrid(this);
    m_mapView->setGrid(m_mgrsGrid);

    // Handle map clicked
    connect(m_mapView, &MapQuickView::mouseClicked, this, [this](QMouseEvent& mouseEvent) {
        Point mapPoint(m_mapView->screenToLocation(mouseEvent.x(), mouseEvent.y()));
        m_mapView->setViewpointCenter(mapPoint);
    });

    connect(m_regularLocator, &RegularLocator::onLocated, this, [this](WGS84Location wgs84Location)
    {
        Point mapCenter(wgs84Location.longitude(), wgs84Location.latitude(), SpatialReference::wgs84());
        m_mapView->setViewpointCenter(mapCenter);
    });

    m_mapView->setMap(m_map);
    emit mapViewChanged();
}

QString GEOINTRecon::mapCenter() const
{
    if (nullptr == m_mapView)
    {
        return QString("");
    }

    Geometry mapCenter = m_mapView->currentViewpoint(ViewpointType::CenterAndScale).targetGeometry();
    switch (mapCenter.geometryType())
    {
    case GeometryType::Point:
        {
            Point location = static_cast<Point>(mapCenter);
            const int mgrsPrecision = 5;
            return CoordinateFormatter::toMgrs(location, MgrsConversionMode::Automatic, mgrsPrecision, true);
        }
        break;

    default:
        return QString("");
    }
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

void GEOINTRecon::addObservation(const QString &location, const QString &minDistance, const QString &maxDistance, const QString &linearUnit, const QString &direction)
{
    m_observationFactory->addNewObservation(location, minDistance, maxDistance, linearUnit, direction);
}

void GEOINTRecon::calculateThreats()
{
    Geometry mapCenter = m_mapView->currentViewpoint(ViewpointType::CenterAndScale).targetGeometry();
    switch (mapCenter.geometryType())
    {
    case GeometryType::Point:
        {
            Point location = static_cast<Point>(mapCenter);

            // Calculate threats
            m_threatFactory->calculateThreats(location);
        }
        break;
    default:
        return;
    }
}

void GEOINTRecon::centerMap(const QString &location, const QString &distance, const QString &linearUnit, const QString &direction)
{
    WGS84Location wgs84Location = m_regularLocator->locate(location, distance, linearUnit, direction);
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

            // Setup the overlays
            setupOverlays();

            // Setup tasks
            setupTasks();

            // Iterate features
            //visitMap(focusMap);
        }
    }

    // Emit map view changed
    emit mapViewChanged();

    // Emit layer list model changed
    emit layerListModelChanged();
}

void GEOINTRecon::setupOverlays()
{
    m_mapView->graphicsOverlays()->clear();

    m_observationFactory->setupOverlays(*m_mapView);
    m_threatFactory->setupOverlays(*m_mapView);
}

void GEOINTRecon::setupTasks()
{
    if (nullptr != m_packageElement)
    {
        m_regularLocator->setupLocatorTask(m_packageElement->locatorTask());
    }
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
                    Geometry geometry = feature->geometry();
                    if (!geometry.isEmpty())
                    {
                        geometryCount++;

                        switch (geometry.geometryType())
                        {
                        case GeometryType::Point:
                            {
                            }
                            break;

                        default:
                            break;
                        }
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
