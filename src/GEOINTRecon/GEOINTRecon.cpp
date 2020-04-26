
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

#include "MobilePackageStore.h"
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

    // Try loading the packages
    MobilePackageStore store;
    QFileInfoList packageInfos = store.packageInfos();
    for (int index = 0; index < packageInfos.size(); index++)
    {
        QFileInfo packageInfo = packageInfos.at(index);
        if (packageInfo.exists())
        {
            m_mobileMapPackage = new MobileMapPackage(packageInfo.filePath(), this);
            if (m_mobileMapPackage
                && LoadStatus::NotLoaded == m_mobileMapPackage->loadStatus())
            {
                connect(m_mobileMapPackage, &MobileMapPackage::loadStatusChanged, [this](LoadStatus loadStatus)
                {
                    switch (loadStatus)
                    {
                        case LoadStatus::Loaded:
                            break;

                        default:
                            return;
                    }

                    // Update the map views map
                    // and emit map view changed
                    Map* firstMap = m_mobileMapPackage->maps().at(0);
                    m_mapView->setMap(firstMap);
                    emit mapViewChanged();
                });

                // Start loading the map package
                m_mobileMapPackage->load();
            }
            break;
        }
    }


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
