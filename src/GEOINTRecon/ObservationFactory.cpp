#include "ObservationFactory.h"

#include "GeometryEngine.h"
#include "GraphicsOverlay.h"
#include "LinearUnit.h"
#include "MapView.h"
#include "SimpleFillSymbol.h"
#include "SimpleRenderer.h"

#include "RegularLocator.h"

using namespace Esri::ArcGISRuntime;

ObservationFactory::ObservationFactory(QObject *parent) :
    QObject(parent),
    m_regularLocator(new RegularLocator(this))
{

}

void ObservationFactory::setupOverlays(const Esri::ArcGISRuntime::MapView &mapView)
{
    if (nullptr != m_observationOverlay)
    {
        delete m_observationOverlay;
    }

    m_observationOverlay = new GraphicsOverlay(this);
    SimpleRenderer* renderer = new SimpleRenderer(m_observationOverlay);
    SimpleFillSymbol* symbol = new SimpleFillSymbol(SimpleFillSymbolStyle::Solid, QColorConstants::Red, m_observationOverlay);
    renderer->setSymbol(symbol);
    m_observationOverlay->setRenderer(renderer);
    m_observationOverlay->setOpacity(0.35);

    mapView.graphicsOverlays()->append(m_observationOverlay);
}

void ObservationFactory::addNewObservation(const QString &location, const QString &distance, const QString &linearUnit, const QString &direction)
{
    Geometry observationArea = m_regularLocator->locateGeometry(location, distance, linearUnit, direction);
    Graphic* observationGraphic = new Graphic(observationArea, m_observationOverlay);
    m_observationOverlay->graphics()->append(observationGraphic);
}
