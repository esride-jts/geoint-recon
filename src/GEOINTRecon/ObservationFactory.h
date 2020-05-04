#ifndef OBSERVATIONFACTORY_H
#define OBSERVATIONFACTORY_H

namespace Esri
{
namespace ArcGISRuntime
{
class GraphicsOverlay;
class MapView;
class Point;
}
}

class RegularLocator;

#include <QObject>

class ObservationFactory : public QObject
{
    Q_OBJECT
public:
    explicit ObservationFactory(QObject *parent = nullptr);

    void setupOverlays(const Esri::ArcGISRuntime::MapView &mapView);

    void addNewObservation(const QString &location, const QString &minDistance = "", const QString &maxDistance = "", const QString &linearUnit = "", const QString &direction = "");

signals:


private:
    Esri::ArcGISRuntime::GraphicsOverlay* m_observationOverlay = nullptr;
    RegularLocator* m_regularLocator = nullptr;
};

#endif // OBSERVATIONFACTORY_H
