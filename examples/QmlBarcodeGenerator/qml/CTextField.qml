import QtQuick 2.12
import QtQuick.Controls 2.12


/*!
  Field for setting width, height, margin & error correction code level parameters.
  */
TextField {
  id: root

  property bool inputIsValid: true

  selectByMouse: true

  leftPadding: 5

  background: Rectangle {
    radius: 2

    border {
      color: inputIsValid ? Theme.borderColor : Theme.invalidInputBorderColor
      width: 1
    }
  }
}
