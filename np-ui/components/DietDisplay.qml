import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import NP 1.0;
import components 1.0;

Item {
    id: root
    property Diet diet
    property FoodItem foodItem: diet ? diet.foodTotalEq : null
    property bool weight: false

    ColumnLayout {
        anchors.fill: parent
        TextField {
            id: title
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            font.pointSize: 28
            text: diet ? diet.ui_name : ''
            onTextChanged: diet.ui_name = text
        }

        FoodItemDisplay {
            Layout.fillHeight: true
            Layout.fillWidth: true
            foodItem: root.diet ? root.diet.foodTotalEq : null
            desc: false
        }
    }
}
