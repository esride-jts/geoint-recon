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

import QtQuick 2.2
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.2

import Esri.MobilePackageStore 1.0

Item {
    id: packageViewRoot

    signal onShowMapPackage(string mapPackagePath)

    ListModel {
        id: packageModel
        ListElement {
            title: "Incidents analysis"
            description: "The Incident Analysis template is an Esri Defense and Intelligence template containing maps and tools that can be used to perform pattern and trend analysis."
        }
        ListElement {
            title: "Hot-cold spot analysis"
            description: "..."
        }
    }

    MobilePackageStore {
        id: mobileMapPackageStore

        Component.onCompleted: {
            mobileMapPackageStore.onLoaded.connect(updatePackageModel);
        }

        function updatePackageModel() {
            console.log("Model loaded");
        }
    }

    SwipeView {
        anchors.fill: parent
        id: swipeView

        Repeater {
            id: repeater
            model: mobileMapPackageStore

            Item {
                id: pageItem

                RowLayout {
                    anchors.fill: parent
                    spacing: 10

                    Button {
                        text: "<"
                        enabled: 0 < index
                        onClicked: swipeView.decrementCurrentIndex()
                        Layout.alignment: Qt.AlignLeft
                    }

                    Column {
                        Layout.fillWidth: true
                        spacing: 10

                        Label {
                            text: model.title
                            horizontalAlignment: "AlignHCenter"
                            width: parent.width
                            font.bold: true
                        }
                        Label {
                            text: model.description
                            width: parent.width
                            wrapMode: Text.WordWrap
                            font.italic: true
                            font.pixelSize: font.pixelSize
                        }
                        Button {
                            text: "Show"
                            width: parent.width

                            onClicked: {
                                mobileMapPackageStore.setSelectedRowIndex(index);
                                packageViewRoot.onShowMapPackage("...");
                            }
                        }
                    }

                    Button {
                        text: ">"
                        enabled: index < swipeView.count - 1
                        onClicked: swipeView.incrementCurrentIndex()
                        Layout.alignment: Qt.AlignRight
                    }
                }
            }
        }
    }
}
