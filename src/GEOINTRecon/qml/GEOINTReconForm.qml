
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
        id: rootColumn
        anchors.fill: parent
        spacing: 10

        Label {
            id: titleLabel
            text: "Map View of the GEOINT Recon"
            horizontalAlignment: "AlignHCenter"
            Layout.fillWidth: true
            font.bold: true
        }

        // Create MapQuickView here, and create its Map etc. in C++ code
        MapView {
            id: view
            Layout.fillHeight: true
            Layout.fillWidth: true
            // set focus to enable keyboard navigation
            focus: true
        }

        Button {
            id: locateButton
            text: "Locate"
            Layout.fillWidth: true

            onClicked: model.centerMap("33UUT 10031 45848")
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
