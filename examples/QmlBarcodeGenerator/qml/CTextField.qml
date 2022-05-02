import QtQuick 2.12
import QtQuick.Controls 2.12


/*!
  Field for setting width, height, margin & error correction code level parameters.
  */
TextField {
  id: root

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
