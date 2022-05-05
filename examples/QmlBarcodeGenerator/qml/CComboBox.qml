import QtQuick 2.12
import QtQuick.Controls 2.12


/*!
  Drop-down menu for barcode format selection & save file format extension
  */
ComboBox {
  id: root

  background: Rectangle {
    radius: 2

    border {
      color: theme.borderColor
      width: 1
    }
  }
}
