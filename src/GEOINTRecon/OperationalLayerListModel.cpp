#include "OperationalLayerListModel.h"

#include "Layer.h"
#include "LayerListModel.h"

#include <QDebug>

using namespace Esri::ArcGISRuntime;

OperationalLayerListModel::OperationalLayerListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

bool OperationalLayerListModel::layerVisibility(int index) const
{
    if (index < 0 || rowCount() <= index)
    {
        return false;
    }

    return m_layerListModel->at(index)->isVisible();
}

void OperationalLayerListModel::setLayerVisibility(int index, bool visible)
{
    if (index < 0 || rowCount() <= index)
    {
        return;
    }

    m_layerListModel->at(index)->setVisible(visible);
}

void OperationalLayerListModel::updateModel(Esri::ArcGISRuntime::LayerListModel *layerListModel)
{
    // Emit reset of this model instance
    beginResetModel();
    m_layerListModel = layerListModel;
    endResetModel();
}

QHash<int, QByteArray> OperationalLayerListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(RoleNames::TitleRole, QString("title").toUtf8());
    roles.insert(RoleNames::VisibleRole, QString("visible").toUtf8());
    return roles;
}

int OperationalLayerListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    /*if (parent.isValid())
        return 0;*/

    if (nullptr == m_layerListModel)
    {
        return 0;
    }

    return m_layerListModel->size();
}

QVariant OperationalLayerListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() < 0 || m_layerListModel->size() <= index.row())
    {
        return QVariant();
    }

    qDebug() << "Model Index: (" << index.row() << ", " << index.column() << ")";
    qDebug() << "Role: " << role;

    switch (role)
    {
        case TitleRole:
            return m_layerListModel->at(index.row())->name();

        case VisibleRole:
            return m_layerListModel->at(index.row())->isVisible();

        default:
            return QVariant();
    }
}
