import QtQuick 2.0
import QtQuick.Controls 2.12

TextField {
  id: root

  background: Rectangle {
    implicitWidth: settings.width
    implicitHeight: settings.height / 8

    radius: 2

    border {
      color: "#333"
      width: 1
    }
  }
}
