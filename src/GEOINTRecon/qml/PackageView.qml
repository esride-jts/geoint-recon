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

import Esri.MobilePackageElement 1.0
import Esri.MobilePackageStore 1.0

Item {
    id: packageViewRoot

    signal onShowMapPackage(MobilePackageElement element)

    Component.onCompleted: {
        appLabelAnimation.start();
    }

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

    ColumnLayout {
        anchors.fill: parent
        anchors.topMargin: 30

        Label {
            id: appLabel
            Layout.fillWidth: true
            horizontalAlignment: "AlignHCenter"
            text: "GEOINT Recon"
            font.bold: true
            font.pointSize: 32
            opacity: 0.3

            NumberAnimation {
                id: appLabelAnimation
                target: appLabel
                property: "opacity"
                from: 0.3
                to: 1.0
                duration: 2305
                easing.type: Easing.InBack
            }
        }

        SwipeView {
            id: swipeView
            Layout.fillHeight: true
            Layout.fillWidth: true

            Repeater {
                id: repeater
                model: mobileMapPackageStore

                Item {
                    id: pageItem

                    RowLayout {
                        anchors.fill: parent
                        spacing: 30

                        Button {
                            text: "<"
                            enabled: 0 < index
                            onClicked: swipeView.decrementCurrentIndex()
                            Layout.alignment: Qt.AlignLeft
                        }

                        ColumnLayout {
                            //anchors.horizontalCenter: parent.horizontalCenter
                            Layout.fillWidth: true
                            spacing: 30

                            Label {
                                Layout.fillWidth: true
                                text: model.title
                                horizontalAlignment: "AlignHCenter"
                                font.bold: true
                            }

                            Label {
                                Layout.fillWidth: true
                                text: model.description
                                wrapMode: Text.WordWrap
                                font.italic: true
                                font.pixelSize: font.pixelSize
                            }

                            Button {
                                Layout.fillWidth: true
                                text: "Show"
                                //width: parent.width

                                onClicked: {
                                    mobileMapPackageStore.setSelectedRowIndex(index);
                                    packageViewRoot.onShowMapPackage(mobileMapPackageStore.selectedPackageElement());
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


}
