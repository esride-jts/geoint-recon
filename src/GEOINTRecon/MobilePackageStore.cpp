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

#include "MobileMapPackage.h"

#include "MobilePackageStore.h"
#include "MobilePackageElement.h"

#include <QProcessEnvironment>

using namespace Esri::ArcGISRuntime;

MobilePackageStore::MobilePackageStore(QObject *parent) : QAbstractListModel(parent)
{
    loadPackages();
}

QFileInfoList MobilePackageStore::packageInfos() const
{
    QString datapathKeyName = "geoint.datapath";
    QProcessEnvironment systemEnvironment = QProcessEnvironment::systemEnvironment();
    if (systemEnvironment.contains(datapathKeyName))
    {
        QString datapathValue = systemEnvironment.value(datapathKeyName);
        QDir dataDirectory(datapathValue);
        if (dataDirectory.exists())
        {
            dataDirectory.setFilter(QDir::Files);
            dataDirectory.setNameFilters(QStringList() << "*.mmpk");
            return dataDirectory.entryInfoList();
        }
        else
        {
            return QFileInfoList();
        }
    }
    else
    {
        QDir homeDirectory = QDir::home();
        homeDirectory.cd("geoint-recon");
        homeDirectory.cd("data");
        if (homeDirectory.exists())
        {
            homeDirectory.setFilter(QDir::Files);
            homeDirectory.setNameFilters(QStringList() << "*.mmpk");
            return homeDirectory.entryInfoList();
        }
        else
        {
            return QFileInfoList();
        }
    }
}

void MobilePackageStore::loadPackages()
{
    // Try loading the packages
    QFileInfoList packageInfos = this->packageInfos();
    int packageCount = packageInfos.size();

    // Begin loading into this model
    // from 0 to package count exclusive
    beginInsertRows(QModelIndex(), rowCount(), packageCount - 1);
    for (int index = 0; index < packageCount; index++)
    {
        QFileInfo packageInfo = packageInfos.at(index);
        if (packageInfo.exists())
        {
            MobileMapPackage* mobileMapPackage = new MobileMapPackage(packageInfo.filePath());
            if (mobileMapPackage
                && LoadStatus::NotLoaded == mobileMapPackage->loadStatus())
            {
                connect(mobileMapPackage, &MobileMapPackage::loadStatusChanged, [mobileMapPackage, this, packageCount](LoadStatus loadStatus)
                {
                    switch (loadStatus)
                    {
                        case LoadStatus::Loaded:
                            break;

                        default:
                            return;
                    }

                    MobilePackageElement* packageElement = new MobilePackageElement(mobileMapPackage);
                    this->m_model.append(packageElement);
                    if (packageCount == this->m_model.size())
                    {
                        // Emit all mobile map packages loaded
                        emit onLoaded();

                        // End loading into this model
                        endInsertRows();
                    }
                });

                // Start loading the map package
                mobileMapPackage->load();
            }
        }
    }
}

MobilePackageElement* MobilePackageStore::packageElement(int row) const
{
    if (row < 0 || m_model.size() <= row)
    {
        return nullptr;
    }

    return m_model.at(row);
}

void MobilePackageStore::setSelectedRowIndex(int row)
{
    if (row < 0 || m_model.size() <= row)
    {
        return;
    }

    m_selectedPackageElement = m_model.at(row);
}

MobilePackageElement* MobilePackageStore::selectedPackageElement() const
{
    return m_selectedPackageElement;
}

QHash<int, QByteArray> MobilePackageStore::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(RoleNames::TitleRole, QString("title").toUtf8());
    roles.insert(RoleNames::DescriptionRole, QString("description").toUtf8());
    return roles;
}

int MobilePackageStore::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_model.size();
}

QVariant MobilePackageStore::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || m_model.size() <= index.row())
    {
        return QVariant();
    }

    MobilePackageElement* packageElement = m_model.at(index.row());
    switch (role)
    {
        case TitleRole:
            return packageElement->title();

        case DescriptionRole:
            return packageElement->description();

        default:
            return QVariant();
    }
}
