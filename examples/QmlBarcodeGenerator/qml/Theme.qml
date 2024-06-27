pragma Singleton

import QtQuick 2.12


/**
  Contains common used colors
  */
QtObject {
  id: root

  readonly property color backgroundColor: "#218165"
  readonly property color textColor: "#bdbdbd"
  readonly property color borderColor: "#333"
  readonly property color invalidInputBorderColor: "#F00"
}
