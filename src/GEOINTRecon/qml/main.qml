
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
            description: "The Incident Analysis template is an Esri Defense and Intelligence template containing maps and tools that can be used to perform pattern and trend analysis."
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

                RowLayout {
                    anchors.fill: parent
                    spacing: 10

                    Button {
                        text: "<"
                        enabled: 0 < index
                        onClicked: swipeView.decrementCurrentIndex()
                        anchors.left: parent.left
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
                    }

                    Button {
                        text: ">"
                        enabled: index < swipeView.count - 1
                        onClicked: swipeView.incrementCurrentIndex()
                        anchors.right: parent.right
                    }
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
