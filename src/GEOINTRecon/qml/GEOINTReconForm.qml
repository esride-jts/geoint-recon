
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

    Column {
        id: rootColumn
        anchors.fill: parent
        spacing: 10

        Label {
            id: titleLabel
            text: "Map View of the GEOINT Recon"
            horizontalAlignment: "AlignHCenter"
            width: parent.width
            font.bold: true
        }

        // Create MapQuickView here, and create its Map etc. in C++ code
        MapView {
            id: view
            height: parent.height - titleLabel.height - locateButton.height - backButton.height - 3 * rootColumn.spacing
            width: parent.width
            // set focus to enable keyboard navigation
            focus: true
        }

        Button {
            id: locateButton
            text: "Locate"
            width: parent.width

            onClicked: model.centerMap("33UUT 10031 45848")
        }

        Button {
            id: backButton
            text: "Back"
            width: parent.width

            onClicked: mapViewRoot.onNavigateHome()
        }

        // Declare the C++ instance which creates the map etc. and supply the view
        GEOINTRecon {
            id: model
            mapView: view
        }
    }
}
