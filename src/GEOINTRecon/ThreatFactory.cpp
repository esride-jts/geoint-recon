#include "ThreatFactory.h"

#include "GeometryEngine.h"
#include "GraphicsOverlay.h"
#include "MapView.h"
#include "SimpleFillSymbol.h"
#include "SimpleLineSymbol.h"
#include "SimpleRenderer.h"

#include <QList>

using namespace Esri::ArcGISRuntime;

ThreatFactory::ThreatFactory(QObject *parent) : QObject(parent)
{
    m_threatEvacuationDistances.insert("Pipe Bomb", 850);
    m_threatEvacuationDistances.insert("Suicide Belt", 1080);
    m_threatEvacuationDistances.insert("Suicide Vest", 1360);
    m_threatEvacuationDistances.insert("Suitcase Bomb", 1850);
    m_threatEvacuationDistances.insert("Compact Sedan", 1500);
    m_threatEvacuationDistances.insert("Sedan", 1750);
    m_threatEvacuationDistances.insert("Cargo Van", 2750);
    m_threatEvacuationDistances.insert("Delivery Truck", 3750);
    m_threatEvacuationDistances.insert("Water Truck", 6500);
    m_threatEvacuationDistances.insert("Semitrailer", 7000);
}

void ThreatFactory::setupOverlays(const MapView &mapView)
{
    if (nullptr != m_resultOverlay)
    {
        delete m_resultOverlay;
    }

    m_resultOverlay = new GraphicsOverlay(this);
    SimpleRenderer* renderer = new SimpleRenderer(m_resultOverlay);
    SimpleFillSymbol* symbol = new SimpleFillSymbol(SimpleFillSymbolStyle::Null, QColorConstants::Red, m_resultOverlay);
    const float outlineWidth = 5;
    SimpleLineSymbol* outlineSymbol = new SimpleLineSymbol(SimpleLineSymbolStyle::DashDot, QColorConstants::Red, outlineWidth, m_resultOverlay);
    symbol->setOutline(outlineSymbol);
    renderer->setSymbol(symbol);
    m_resultOverlay->setRenderer(renderer);

    mapView.graphicsOverlays()->append(m_resultOverlay);
}

void ThreatFactory::calculateThreats(const Esri::ArcGISRuntime::Point &location)
{
    //QList<Geometry> evacuationAreas;
    const double maxDeviation = 1e-5;
    QList<double> distances = m_threatEvacuationDistances.values();
    std::sort(distances.begin(), distances.end());
    for (double evacuationDistance : distances)
    {
        Geometry evacuationArea = GeometryEngine::bufferGeodetic(location, evacuationDistance, LinearUnit::feet(), maxDeviation, GeodeticCurveType::Geodesic);
        /*
        if (!evacuationAreas.empty())
        {
            // TODO: Difference constructs a geometry which cannot be displayed by the runtime!
            // Calculate the difference to the last evacuation area
            //evacuationArea = GeometryEngine::difference(evacuationAreas.last(), evacuationArea);
            //evacuationArea = GeometryEngine::simplify(evacuationArea);
        }
        //evacuationAreas.append(evacuationArea);
        */

        Graphic* evacuationGraphic = new Graphic(evacuationArea, m_resultOverlay);
        m_resultOverlay->graphics()->append(evacuationGraphic);
    }
}
