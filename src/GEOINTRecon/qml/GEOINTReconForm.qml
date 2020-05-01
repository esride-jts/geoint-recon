
// Copyright 2019 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

import QtQuick 2.6
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.2
import Esri.GEOINTRecon 1.0

import Esri.MobilePackageElement 1.0

Item {
    id: mapViewRoot

    signal onNavigateHome()

    function centerMap() {
        console.log(locationTextField.text, distanceField.text, linearUnitBox.currentText, directionBox.currentText);
        model.centerMap(locationTextField.text, distanceField.text, linearUnitBox.currentText, directionBox.currentText);
    }

    function showMobilePackageElement(packageElement) {
        model.packageElement = packageElement;
        model.showMap();
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Label {
            id: titleLabel
            text: "Map View of the GEOINT Recon"
            horizontalAlignment: "AlignHCenter"
            Layout.fillWidth: true
            font.bold: true
        }

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true

            ListModel {
                id: layerModel                
                ListElement { title: "1.jpg"; visible: true }
                ListElement { title: "2.jpg"; visible: true }
                ListElement { title: "3.jpg"; visible: true }
            }

            Component {
                id: layerComponent

                Item {
                    height: layerLabel.height + 20
                    width: parent.width

                    Component.onCompleted: {
                        // Initialize the checkbox using the layer visibility
                        layerCheckbox.checked = layerListView.model.layerVisibility(index);
                    }

                    RowLayout {
                        anchors.verticalCenter: parent.verticalCenter

                        CheckBox {
                            id: layerCheckbox

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    // The user affected click destroys the binding!
                                    layerCheckbox.checked = !layerCheckbox.checked;

                                    // Directly update the layer visibility using a discret invokable method
                                    layerListView.model.setLayerVisibility(index, layerCheckbox.checked);
                                }
                            }
                        }

                        Label {
                            id: layerLabel
                            text: title
                        }

                    }

                    MouseArea {
                        anchors.fill: parent
                        propagateComposedEvents: true
                        onClicked: {
                            if (index === layerListView.currentIndex) {
                                mouse.accepted = false;
                            }
                            else {
                                layerListView.currentIndex = index;
                            }
                        }
                    }
                }
            }

            ListView {
                id: layerListView
                //model: layerModel
                model: model.layerListModel
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.minimumWidth: 100
                highlight: Rectangle { color: "#616847"; radius: 5 }
                focus: true

                delegate: layerComponent
            }

            // Create MapQuickView here, and create its Map etc. in C++ code
            MapView {
                id: view
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.preferredHeight: 300
                Layout.preferredWidth: 300
                // set focus to enable keyboard navigation
                focus: true

                onViewpointChanged: {
                    // Update the location using the center of the current map extent
                    locationTextField.text = model.mapCenter;
                }
            }
        }

        RowLayout {
            spacing: 30

            TextField {
                id: locationTextField
                Layout.fillWidth: true
                text: "33UUT 10031 45848"

                Keys.onReturnPressed: {
                    model.centerMap(locationTextField.text);
                }
            }

            TextField {
                id: distanceField
                text: "0"
                validator: DoubleValidator {
                    bottom: 0
                    top: 10000
                }

                Keys.onReturnPressed: {
                    centerMap();
                }
            }

            ComboBox {
                id: linearUnitBox
                model: ListModel {
                   ListElement { text: "km" }
                   ListElement { text: "m" }
                   ListElement { text: "mi" }
                   ListElement { text: "nm" }
                   ListElement { text: "ft" }
                }
            }

            ComboBox {
                id: directionBox
                model: ListModel {
                   ListElement { text: "N" }
                   ListElement { text: "NNE" }
                   ListElement { text: "NE" }
                   ListElement { text: "ENE" }
                   ListElement { text: "E" }
                   ListElement { text: "ESE" }
                   ListElement { text: "SE" }
                   ListElement { text: "SSE" }
                   ListElement { text: "S" }
                   ListElement { text: "SSW" }
                   ListElement { text: "SW" }
                   ListElement { text: "WSW" }
                   ListElement { text: "W" }
                   ListElement { text: "WNW" }
                   ListElement { text: "NW" }
                   ListElement { text: "NNW" }
                }
            }

            Button {
                id: locateButton
                text: "Locate"
                Layout.fillWidth: true

                onClicked: {
                    centerMap();
                }

                Keys.onReturnPressed: {
                    centerMap();
                }
            }

        }

        Button {
            id: backButton
            text: "Back"
            Layout.fillWidth: true

            onClicked: mapViewRoot.onNavigateHome()
        }

        // Declare the C++ instance which creates the map etc. and supply the view
        GEOINTRecon {
            id: model
            mapView: view
        }
    }
}
