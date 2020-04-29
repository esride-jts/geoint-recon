#include "OperationalLayerListModel.h"

#include "LayerListModel.h"

#include <QDebug>

using namespace Esri::ArcGISRuntime;

OperationalLayerListModel::OperationalLayerListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void OperationalLayerListModel::updateModel(Esri::ArcGISRuntime::LayerListModel *layerListModel)
{
    beginInsertRows(QModelIndex(), 0, layerListModel->size() - 1);
    m_layerListModel = layerListModel;
    endInsertRows();
}

QHash<int, QByteArray> OperationalLayerListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(RoleNames::TitleRole, QString("title").toUtf8());
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

    switch (role)
    {
        case TitleRole:
            return m_layerListModel->at(index.row())->name();

        default:
            return QVariant();
    }
}
