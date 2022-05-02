import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Button {
  id: root

  anchors.margins: 5

  Layout.alignment: Qt.AlignHCenter
  Layout.fillHeight: true
  Layout.fillWidth: true

  checkable: true

  palette.buttonText: "#bdbdbd"

  background: Rectangle {
    radius: 10
    color: "#218165"
  }
}
