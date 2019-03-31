import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import NP 1.0;
import components 1.0;

Item {
    id: root
    property FoodItemList foodItemList
    property FoodItem foodItem: foodItemList ? foodItemList.foodTotalEq : null
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
            text: foodItemList ? foodItemList.ui_name : ''
            onTextChanged: foodItemList.ui_name = text
        }
        Label {
            visible: root.weight ? true : false
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            font.pointSize: 20
            text: "Recipe Weight (grams): " + (foodItemList ? foodItemList.ui_gmWgt : 0)
        }
        RowLayout {
            visible: root.weight ? true : false
            Layout.alignment: Qt.AlignHCenter

            Label {
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                font.pointSize: 20
                text: "Serving Size (grams)"
            }

            SpinBox {
                id: servingAmount
                from: 0
                value: foodItemList ? foodItemList.ui_amount : 0
                to: 100 * 10000
                stepSize: 10
                editable: true

                property int decimals: 2
                property real realValue: value

                validator: DoubleValidator {
                    bottom: Math.min(servingAmount.from, servingAmount.to)
                    top:  Math.max(servingAmount.from, servingAmount.to)
                }

                textFromValue: function(value, locale) {
                    return Number(value).toLocaleString(locale, 'f', servingAmount.decimals)
                }

                valueFromText: function(text, locale) {
                    return Number.fromLocaleString(locale, text)
                }

                onValueChanged: if(root.weight && foodItemList) foodItemList.ui_amount = servingAmount.value.toFixed(2)

                Component.onCompleted: {
                    contentItem.selectByMouse = true
                }
            }
        }

        /*FoodItemDisplay {
            Layout.fillHeight: true
            Layout.fillWidth: true
            foodItem: root.foodItemList ? root.foodItemList.foodAvgEq : null
            desc: false
        }*/

        FoodItemDisplay {
            Layout.fillHeight: true
            Layout.fillWidth: true
            foodItem: root.foodItemList ? root.foodItemList.foodTotalEq : null
            desc: false
        }
    }
}
