import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import NP 1.0;
import components 1.0;

Item {
    id: root
    property Day day
    property FoodItem foodItem: day ? day.foodTotalEq : null
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
            text: day ? day.ui_name : ''
            onTextChanged: day.ui_name = text
        }

        FoodItemDisplay {
            Layout.fillHeight: true
            Layout.fillWidth: true
            foodItem: root.day ? root.day.foodTotalEq : null
            desc: false
        }
    }
}
