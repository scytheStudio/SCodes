import QtQuick 2.0
import QtQuick.Controls 2.12

TextField {
  implicitWidth: parent.width
  implicitHeight: parent.height / 8
  selectByMouse: true
  background: Rectangle {
    radius: 2
    border {
      color: "#333"
      width: 1
    }
  }
}
