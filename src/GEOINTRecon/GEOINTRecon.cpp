
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

#include "RegularLocator.h"

#include <QUrl>

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
