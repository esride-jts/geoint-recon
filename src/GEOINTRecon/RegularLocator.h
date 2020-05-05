// GEOINT Recon is a sample native desktop application for geospatial intelligence workflows.
// Copyright (C) 2020 Esri Deutschland GmbH
// Jan Tschada (j.tschada@esri.de)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Additional permission under GNU LGPL version 3 section 4 and 5
// If you modify this Program, or any covered work, by linking or combining
// it with ArcGIS Runtime for Qt (or a modified version of that library),
// containing parts covered by the terms of ArcGIS Runtime for Qt,
// the licensors of this Program grant you additional permission to convey the resulting work.
// See <https://developers.arcgis.com/qt/> for further information.
//

#ifndef REGULARLOCATOR_H
#define REGULARLOCATOR_H

namespace Esri
{
namespace ArcGISRuntime
{
class Geometry;
class LinearUnit;
class LocatorTask;
}
}

#include <QObject>

#include "GeointApi.h"

class RegularLocator : public QObject
{
    Q_OBJECT
public:
    explicit RegularLocator(QObject *parent = nullptr);

    void setupLocatorTask(Esri::ArcGISRuntime::LocatorTask *locatorTask);

    WGS84Location locate(const QString &location, const QString &distance = "", const QString &linearUnit = "", const QString &direction = "");
    Esri::ArcGISRuntime::Geometry locateGeometry(const QString &location, const QString &minDistance = "", const QString &maxDistance = "", const QString &linearUnit = "", const QString &direction = "");

signals:
    void onLocated(WGS84Location newLocation);

private:
    double toDegrees(const QString &direction) const;
    Esri::ArcGISRuntime::LinearUnit toLinearUnit(const QString &linearUnit, double &conversionFactor) const;

    QObject* m_locatorContext;
    Esri::ArcGISRuntime::LocatorTask* m_locatorTask = nullptr;
};

#endif // REGULARLOCATOR_H
