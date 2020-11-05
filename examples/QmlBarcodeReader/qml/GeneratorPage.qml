import QtQuick 2.12
import QtQuick.Controls 2.12
import QtMultimedia 5.12
import QtQuick.Layouts 1.12
import com.scythestudio.sbarcodereader 1.0

Item {
    id: root
    anchors.fill: parent

    SBarcodeGenerator {
        id: barcodeGenerator
        onProcessFinished: {
            console.log(barcodeGenerator.filePath);
            image.source = "file:///" + barcodeGenerator.filePath
        }
    }

    Rectangle {
        anchors.fill: parent
        height: parent.height
        width: parent.width
        visible: true
        color: "white"

        Rectangle {
            id: inputRect
            z: 100
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 40
            TextField {
                id: textField
                placeholderText: qsTr("Input")
                anchors.fill: parent
                selectByMouse: true
                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.IBeamCursor
                    acceptedButtons: Qt.NoButton
                }
            }
        }

        Rectangle {
            id: imageRect
            anchors.top: inputRect.bottom
            anchors.bottom: buttonsRect.top
            anchors.left: parent.left
            anchors.right: parent.right
            Image {
                id: image
                anchors.centerIn: parent
                width: parent.width
                height: image.width
                cache: false
            }
        }

        Rectangle {
            id: buttonsRect
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 40
            RowLayout {
                id: buttonsLayout
                anchors.fill: parent
                Button {
                    id: settingsButton
                    anchors.margins: 5
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    visible: true
                    checkable: true
                    text: qsTr("Settings")
                    palette.buttonText: "#bdbdbd"
                    background: Rectangle {
                                    radius: 10
                                    color: "#218165"
                                }
                    onClicked: {
                        !checked ? settings.visible = false : settings.visible = true;
                    }
                }

                Button {
                    id: generateButton
                    anchors.margins: 5
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    visible: true
                    text: qsTr("Generate")
                    palette.buttonText: "#bdbdbd"
                    background: Rectangle {
                                    radius: 10
                                    color: "#218165"
                                }
                    onClicked: {
                        image.source = "";
                        barcodeGenerator.process(textField.text)
                    }
                }

                Button {
                    id: saveButton
                    anchors.margins: 5
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    visible: true
                    text: qsTr("Save")
                    palette.buttonText: "#bdbdbd"
                    background: Rectangle {
                                    radius: 10
                                    color: "#218165"
                                }
                    onClicked: {
                        barcodeGenerator.saveImage()
                    }
                }

                Button {
                    id: backButton
                    anchors.margins: 5
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    visible: true
                    text: qsTr("Back")
                    palette.buttonText: "#bdbdbd"
                    background: Rectangle {
                                    radius: 10
                                    color: "#218165"
                                }
                    onClicked: {
                        //StackView implementation
                        root.visible = false;
                    }
                }
            }
        }

        Rectangle {
            id: settings
            anchors.centerIn: parent
            width: parent.width * 0.6
            height: parent.height * 0.6
            visible: false
            border.color: "#333"
            border.width: 1
            Column {
                anchors.fill: parent
                spacing: 2

                TextField {
                    placeholderText: "Current width: " + barcodeGenerator.width
                    onEditingFinished: barcodeGenerator.width = text
                    background: Rectangle {
                                            radius: 2
                                            implicitWidth: settings.width
                                            implicitHeight: settings.height/8
                                            border.color: "#333"
                                            border.width: 1
                                        }
                }
                TextField {
                    placeholderText: "Current height: " + barcodeGenerator.height
                    onEditingFinished: barcodeGenerator.height = text
                    background: Rectangle {
                                            radius: 2
                                            implicitWidth: settings.width
                                            implicitHeight: settings.height/8
                                            border.color: "#333"
                                            border.width: 1
                                        }
                }
                TextField {
                    placeholderText: "Current margin: " + barcodeGenerator.margin
                    onEditingFinished: barcodeGenerator.margin = text
                    background: Rectangle {
                                            radius: 2
                                            implicitWidth: settings.width
                                            implicitHeight: settings.height/8
                                            border.color: "#333"
                                            border.width: 1
                                        }
                }
                TextField {
                    placeholderText: "Current ECC Level: " + barcodeGenerator.eccLevel
                    onEditingFinished: barcodeGenerator.eccLevel = text
                    background: Rectangle {
                                            radius: 2
                                            implicitWidth: settings.width
                                            implicitHeight: settings.height/8
                                            border.color: "#333"
                                            border.width: 1
                                        }
                }
                ComboBox {
                    model: ListModel {
                            id: formats
                            ListElement { text: "QR_CODE" }
                            ListElement { text: "DATA_MATRIX" }
                            ListElement { text: "CODE_128" }
                        }
                    onCurrentIndexChanged: barcodeGenerator.barcodeFormatFromQMLString(formats.get(currentIndex).text)
                    background: Rectangle {
                                            radius: 2
                                            implicitWidth: settings.width
                                            implicitHeight: settings.height/8
                                            border.color: "#333"
                                            border.width: 1
                                        }
                }
                ComboBox {
                    model: ListModel {
                            id: extensions
                            ListElement { text: "png" }
                            ListElement { text: "jpg" }
                        }
                    onCurrentIndexChanged: barcodeGenerator.extension = extensions.get(currentIndex).text
                    background: Rectangle {
                                            radius: 2
                                            implicitWidth: settings.width
                                            implicitHeight: settings.height/8
                                            border.color: "#333"
                                            border.width: 1
                                        }
                }
                TextField {
                    placeholderText: "Current file name: " + barcodeGenerator.fileName
                    selectByMouse: true
                    background: Rectangle {
                                            radius: 2
                                            implicitWidth: settings.width
                                            implicitHeight: settings.height/8
                                            border.color: "#333"
                                            border.width: 1
                                        }
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.IBeamCursor
                        acceptedButtons: Qt.NoButton
                    }
                    onEditingFinished: barcodeGenerator.fileName = text
                }
            }
        }
    }
}
