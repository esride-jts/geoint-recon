
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

import QtQuick 2.2
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.2
import Esri.GEOINTRecon 1.0

import Esri.MobilePackageElement 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 600

    Material.theme: Material.Dark
    Material.accent: "#a7ad6d"      // BW Hellgrün
    //Material.accent: "#616847"      // BW Helloliv
    Material.background: "#312d2a"  // BW Schwarz
    Material.foreground: "#d3c2a6"  // BW Beige
    Material.primary: "#434a39"     // BW Dunkelgrün

    StackView {
        id: stackView
        initialItem: packageView
        anchors.fill: parent
    }

    PackageView {
        id: packageView

        Component.onCompleted: {
            packageView.onShowMapPackage.connect(showMapPackage);
            mapView.onNavigateHome.connect(navigateHome);
        }

        function navigateHome() {
            stackView.pop();
        }

        function showMapPackage(packageElement) {
            mapView.showMobilePackageElement(packageElement);
            stackView.push(mapView);
        }
    }

    GEOINTReconForm {
        id: mapView
        visible: false
    }

    /*
    Column {
        anchors.centerIn: parent

        RadioButton { text: qsTr("Small") }
        RadioButton { text: qsTr("Medium");  checked: true }
        RadioButton { text: qsTr("Large") }
        TextField { text: qsTr("Panzer Hurra!") }
    }
    */

    /*
    GEOINTReconForm {
        anchors.fill: parent
    }
    */
}
