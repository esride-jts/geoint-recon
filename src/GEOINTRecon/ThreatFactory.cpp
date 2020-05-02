#include "ThreatFactory.h"

#include "GeometryEngine.h"
#include "GraphicsOverlay.h"
#include "MapView.h"
#include "SimpleFillSymbol.h"
#include "SimpleRenderer.h"

using namespace Esri::ArcGISRuntime;

ThreatFactory::ThreatFactory(QObject *parent) : QObject(parent)
{

}

void ThreatFactory::setupOverlays(const MapView &mapView)
{
    if (nullptr != m_resultOverlay)
    {
        delete m_resultOverlay;
    }

    m_resultOverlay = new GraphicsOverlay(this);
    SimpleFillSymbol* symbol = new SimpleFillSymbol(SimpleFillSymbolStyle::Solid, QColorConstants::Red, m_resultOverlay);
    SimpleRenderer* renderer = new SimpleRenderer(symbol, m_resultOverlay);
    renderer->setSymbol(symbol);
    m_resultOverlay->setOpacity(0.5);
    m_resultOverlay->setRenderer(renderer);

    mapView.graphicsOverlays()->append(m_resultOverlay);
}

void ThreatFactory::calculateThreats(const Esri::ArcGISRuntime::Point &location)
{
    const double maxDeviation = 1e-5;
    Geometry bufferGeometry = GeometryEngine::bufferGeodetic(location, 1500, LinearUnit::feet(), maxDeviation, GeodeticCurveType::Geodesic);
    Graphic* bufferGraphic = new Graphic(bufferGeometry, m_resultOverlay);
    m_resultOverlay->graphics()->append(bufferGraphic);
}
