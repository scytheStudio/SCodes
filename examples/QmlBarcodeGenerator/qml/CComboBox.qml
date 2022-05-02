import QtQuick 2.12
import QtQuick.Controls 2.12


/*!
  Drop-down menu for barcode format selection & save file format extension
  */
ComboBox {
  id: root

  implicitWidth: parent.width
  implicitHeight: parent.height / 8

  background: Rectangle {
    radius: 2

    border {
      color: "#333"
      width: 1
    }
  }
}
