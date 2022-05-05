import QtQuick 2.12
import QtQuick.Controls 2.12


/*!
  Field for setting width, height, margin & error correction code level parameters.
  */
TextField {
  id: root

  selectByMouse: true

  background: Rectangle {
    radius: 2

    border {
      color: Theme.borderColor
      width: 1
    }
  }
}
