#include "MobilePackageElement.h"

#include "Map.h"
#include "MobileMapPackage.h"

using namespace Esri::ArcGISRuntime;

MobilePackageElement::MobilePackageElement(QObject *parent) : QObject(parent)
{
    m_mobileMapPackage = dynamic_cast<Esri::ArcGISRuntime::MobileMapPackage*>(parent);
}

QString MobilePackageElement::title() const
{
    if (nullptr == m_mobileMapPackage)
    {
        return QString("");
    }

    return m_mobileMapPackage->item()->title();
}

QString MobilePackageElement::description() const
{
    if (nullptr == m_mobileMapPackage)
    {
        return QString("");
    }

    return m_mobileMapPackage->item()->description();
}

Esri::ArcGISRuntime::Map* MobilePackageElement::focusMap() const
{
    if (m_mobileMapPackage->maps().empty())
    {
        return nullptr;
    }

    return m_mobileMapPackage->maps().at(0);
}
