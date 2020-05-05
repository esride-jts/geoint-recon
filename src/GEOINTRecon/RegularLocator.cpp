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

#include "RegularLocator.h"

#include "CoordinateFormatter.h"
#include "GeometryEngine.h"
#include "LocatorTask.h"

#include <QtMath>
#include <QRegularExpression>

using namespace Esri::ArcGISRuntime;

RegularLocator::RegularLocator(QObject *parent) : QObject(parent)
{

}

void RegularLocator::setupLocatorTask(Esri::ArcGISRuntime::LocatorTask *locatorTask)
{
    if (nullptr != m_locatorTask)
    {
        delete m_locatorContext;
    }

    m_locatorTask = locatorTask;

    if (nullptr != m_locatorTask)
    {
        m_locatorContext = new QObject();
        connect(m_locatorTask, &LocatorTask::geocodeCompleted, m_locatorContext, [this](QUuid, const QList<GeocodeResult>& results)
        {
            if (results.empty())
            {
                return;
            }

            GeocodeResult geocodeResult = results.first();
            Point targetLocation = geocodeResult.displayLocation();
            if (targetLocation.isValid())
            {
                emit onLocated(WGS84Location(targetLocation.y(), targetLocation.x()));
            }
        });
    }
}

WGS84Location RegularLocator::locate(const QString &location, const QString &distance, const QString &linearUnit, const QString &direction)
{
    QRegularExpression expression("(\\d{1,2})\\s*([a-zA-Z]{1,3})\\s*(\\d*)\\s*(\\d*)");
    QRegularExpressionMatch match = expression.match(location);
    if (match.hasMatch())
    {
        Point mgrsLocation = CoordinateFormatter::fromMgrs(location, SpatialReference::wgs84(), MgrsConversionMode::Automatic);
        if (mgrsLocation.isValid())
        {
            QList<Point> mgrsLocations({mgrsLocation});
            double distanceAsDouble = distance.toDouble();
            double conversionFactor = 0.0;
            LinearUnit linearUnitAsUnit = toLinearUnit(linearUnit, conversionFactor);
            distanceAsDouble *= conversionFactor;
            double azimuth = toDegrees(direction);
            AngularUnit angularUnit = AngularUnit::degrees();
            GeodeticCurveType curveType = GeodeticCurveType::Geodesic;
            QList<Point> targetLocations = GeometryEngine::moveGeodetic(mgrsLocations, distanceAsDouble, linearUnitAsUnit, azimuth, angularUnit, curveType);
            Point targetLocation = targetLocations.first();
            if (targetLocation.isValid())
            {
                return WGS84Location(targetLocation.y(), targetLocation.x());
            }
            else
            {
                return WGS84Location(mgrsLocation.y(), mgrsLocation.x());
            }
        }
    }
    else if (nullptr != m_locatorTask)
    {
        GeocodeParameters geocodeParameters;
        geocodeParameters.setOutputSpatialReference(SpatialReference::wgs84());
        m_locatorTask->geocodeWithParameters(location, geocodeParameters);
    }

    return WGS84Location();
}

Esri::ArcGISRuntime::Geometry RegularLocator::locateGeometry(const QString &location, const QString &minDistance, const QString &maxDistance, const QString &linearUnit, const QString &direction)
{
    QRegularExpression expression("(\\d{1,2})\\s*([a-zA-Z]{1,3})\\s*(\\d*)\\s*(\\d*)");
    QRegularExpressionMatch match = expression.match(location);
    if (match.hasMatch())
    {
        Point mgrsLocation = CoordinateFormatter::fromMgrs(location, SpatialReference::wgs84(), MgrsConversionMode::Automatic);
        if (mgrsLocation.isValid())
        {
            QList<Point> mgrsLocations({mgrsLocation});
            double distanceAsDouble = 0.5 * (minDistance.toDouble() + maxDistance.toDouble());
            double conversionFactor = 0.0;
            LinearUnit linearUnitAsUnit = toLinearUnit(linearUnit, conversionFactor);
            distanceAsDouble *= conversionFactor;
            double azimuth = toDegrees(direction);
            AngularUnit angularUnit = AngularUnit::degrees();
            GeodeticCurveType curveType = GeodeticCurveType::Geodesic;
            QList<Point> targetLocations = GeometryEngine::moveGeodetic(mgrsLocations, distanceAsDouble, linearUnitAsUnit, azimuth, angularUnit, curveType);
            Point targetLocation = targetLocations.first();
            if (targetLocation.isValid())
            {
                // Calculation of the field of view
                /*
                const double maxDeviation = 1e-5;
                double fieldOfView = 214.0;
                double fieldOfViewHeight = 140.0;
                double viewDiameter = 2 * distanceAsDouble * tan(0.5 * fieldOfView);
                if (1 < viewDiameter)
                {
                    viewDiameter = distanceAsDouble;
                }
                */

                double targetHeight = 0.5 * (maxDistance.toDouble() - minDistance.toDouble());
                double targetWidth = 2.0 * targetHeight;
                if (50 < linearUnitAsUnit.convertToMeters(targetHeight))
                {
                    GeodesicEllipseParameters targetParameters(targetLocation, targetWidth, targetHeight);
                    double axisDirection = -azimuth;
                    targetParameters.setAxisDirection(axisDirection);
                    targetParameters.setLinearUnit(linearUnitAsUnit);
                    targetParameters.setAngularUnit(AngularUnit::degrees());
                    return GeometryEngine::ellipseGeodesic(targetParameters);
                }
                else
                {
                    targetHeight = 50;
                    targetWidth = 100;
                    GeodesicEllipseParameters targetParameters(targetLocation, targetWidth, targetHeight);
                    double axisDirection = -azimuth;
                    targetParameters.setAxisDirection(axisDirection);
                    targetParameters.setLinearUnit(LinearUnit::meters());
                    targetParameters.setAngularUnit(AngularUnit::degrees());
                    return GeometryEngine::ellipseGeodesic(targetParameters);
                }
            }
        }
    }

    return Point();
}

double RegularLocator::toDegrees(const QString &direction) const
{
    QList<QString> directions({
        "N", "NNE", "NE", "ENE", "E", "ESE", "SE", "SSE", "S", "SSW", "SW", "WSW", "W", "WNW", "NW", "NNW"
    });

    double degreePrecision = 360.0 / directions.size();
    for (int index = 0; index < directions.size(); index++)
    {
        QString knownDirection = directions.at(index);
        if (0 == knownDirection.compare(direction, Qt::CaseInsensitive))
        {
            return index * degreePrecision;
        }
    }

    return 0.0;
}

LinearUnit RegularLocator::toLinearUnit(const QString &linearUnit, double &conversionFactor) const
{
    QString linearUnitAsLowerCase = linearUnit.toLower();
    QMap<QString, LinearUnit> linearUnits({
        { "ft", LinearUnit::feet() },
        { "km", LinearUnit::kilometers() },
        { "m", LinearUnit::meters() },
        { "mi", LinearUnit::miles() },
        { "nm", LinearUnit::miles() }
    });

    QMap<QString, LinearUnit>::iterator knownLinearUnit = linearUnits.find(linearUnitAsLowerCase);
    if (linearUnits.end() != knownLinearUnit)
    {
        if (0 == linearUnitAsLowerCase.compare("nm"))
        {
            conversionFactor = 1.15;
        }
        else
        {
            conversionFactor = 1.0;
        }

        return knownLinearUnit.value();
    }

    // Return kilometers default and set the conversion factor 0.0
    // the calller should multipy this factor with the numeric distance.
    // in this case the distance should lead to 0.0.
    conversionFactor = 0.0;
    return LinearUnit::kilometers();
}
