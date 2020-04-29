#ifndef OPERATIONALLAYERLISTMODEL_H
#define OPERATIONALLAYERLISTMODEL_H

#include <QAbstractListModel>

namespace Esri
{
namespace ArcGISRuntime
{
class LayerListModel;
}
}

class OperationalLayerListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit OperationalLayerListModel(QObject *parent = nullptr);

    void updateModel(Esri::ArcGISRuntime::LayerListModel* layerListModel);

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    enum RoleNames {
        TitleRole = Qt::UserRole + 1
    };

    Esri::ArcGISRuntime::LayerListModel* m_layerListModel = nullptr;
};

#endif // OPERATIONALLAYERLISTMODEL_H
