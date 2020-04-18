
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
import Esri.GEOINTRecon 1.0

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

    ListModel {
        id: packageModel
        ListElement {
            title: "Incidents analysis"
            description: "..."
        }
        ListElement {
            title: "Hot-cold spot analysis"
            description: "..."
        }
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

    SwipeView {
        anchors.fill: parent

        id: swipeView

        Repeater {
            model: packageModel

            Item {
                id: pageItem

                Button {
                    text: "<"
                    enabled: 0 < index
                    onClicked: swipeView.decrementCurrentIndex()
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                }

                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: model.title
                }

                Button {
                    text: ">"
                    enabled: index < swipeView.count - 1
                    onClicked: swipeView.incrementCurrentIndex()
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }

    /*
    GEOINTReconForm {
        anchors.fill: parent
    }
    */
}
