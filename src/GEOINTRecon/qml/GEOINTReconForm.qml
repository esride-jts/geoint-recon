
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

    function showMobilePackageElement(packageElement) {
        model.packageElement = packageElement;
        model.showMap();
    }

    ColumnLayout {
        anchors.fill: parent
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
                Layout.maximumWidth: 300
                highlight: Rectangle { color: "#616847"; radius: 5 }
                focus: true

                delegate: layerComponent
            }

            // Create MapQuickView here, and create its Map etc. in C++ code
            MapView {
                id: view
                Layout.fillHeight: true
                Layout.fillWidth: true
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

            Button {
                id: locateButton
                text: "Locate"
                Layout.fillWidth: true

                onClicked: {
                    model.centerMap(locationTextField.text);
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
