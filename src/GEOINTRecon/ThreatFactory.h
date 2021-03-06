#ifndef THREATFACTORY_H
#define THREATFACTORY_H

namespace Esri
{
namespace ArcGISRuntime
{
class GraphicsOverlay;
class MapView;
class Point;
}
}

#include <QMap>
#include <QObject>

class ThreatFactory : public QObject
{
    Q_OBJECT
public:
    explicit ThreatFactory(QObject *parent = nullptr);

    void setupOverlays(const Esri::ArcGISRuntime::MapView &mapView);

    void calculateThreats(const Esri::ArcGISRuntime::Point &location);

signals:

private:
    Esri::ArcGISRuntime::GraphicsOverlay* m_resultOverlay = nullptr;
    QMap<QString, double> m_threatEvacuationDistances;
};

#endif // THREATFACTORY_H
