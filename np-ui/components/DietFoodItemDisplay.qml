import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import NP 1.0;

Item {
    id: root
    property Diet diet
    property FoodItem foodItem
    property bool servings: false
    property bool desc: true
    function scaleNutr(nutrVal)
    {
        return Math.round((foodItem.ui_amount * foodItem.ui_scaleFactor * nutrVal / 100) * 100) / 100
    }

    function calories()
    {
        if(foodItem.ui_protein && foodItem.ui_carbs && foodItem.ui_fat) {
            return root.scaleNutr(foodItem.ui_protein.ui_nutrVal) * 4 + root.scaleNutr(foodItem.ui_carbs.ui_nutrVal) * 4 + root.scaleNutr(foodItem.ui_fat.ui_nutrVal) * 9
        }
        return 0
    }

    onFoodItemChanged: if(root.foodItem) {vitaminList.model = foodItem.ui_vitamins;mineralList.model = foodItem.ui_minerals} else {vitaminList.model = []; mineralList.model = []}

    Layout.alignment: Qt.AlignHCenter
    ColumnLayout {
        anchors.fill: parent

        Label {
            visible: root.desc
            id: longDescLabel
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            font.pointSize: 28
            text: foodItem ? foodItem.ui_foodID.ui_longDesc : ''
        }
        RowLayout {
            visible: root.servings ? true : false
            Layout.alignment: Qt.AlignHCenter

            Label {
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                font.pointSize: 20
                text: "Serving Size"
            }

            SpinBox {
                id: servingAmount
                from: 0
                value: foodItem ? foodItem.ui_amount : 0
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

                onValueChanged: if(foodItem) foodItem.ui_amount = servingAmount.value.toFixed(2)

                Component.onCompleted: {
                    contentItem.selectByMouse = true
                }
            }

            ComboBox {
                id: weightSelector
                textRole: "ui_msreDesc"
                model: if (foodItem) foodItem.ui_weights
                currentIndex: foodItem ? foodItem.ui_weightIdx : 0
                onCurrentIndexChanged: if(foodItem) foodItem.ui_weightIdx = weightSelector.currentIndex
            }
        }

        Label {
            id: calories
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            font.pointSize: 28
            text: if(foodItem) {'Calories ' + root.calories()} else {''}
        }
        Label {
            id: protein
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            font.pointSize: 28
            text: if(foodItem && foodItem.ui_protein) {foodItem.ui_protein.ui_nutrDesc + ' ' + root.scaleNutr(foodItem.ui_protein.ui_nutrVal)} else {''}
        }
        Label {
            id: carbs
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            font.pointSize: 28
            text: if(foodItem && foodItem.ui_carbs) {foodItem.ui_carbs.ui_nutrDesc + ' ' + root.scaleNutr(foodItem.ui_carbs.ui_nutrVal)} else {''}
        }
        Label {
            id: fat
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            font.pointSize: 28
            text: if(foodItem && foodItem.ui_fat) {foodItem.ui_fat.ui_nutrDesc + ' ' + root.scaleNutr(foodItem.ui_fat.ui_nutrVal)} else {''}
        }
        RowLayout {
            ListView {
                id: vitaminList
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                delegate: ItemDelegate {
                    visible: diet.getNutrReq(model.ui_nutrNo) >= 0
                    width: parent.width
                    text: model.ui_nutrDesc + " " + model.ui_nutrVal + model.ui_units + " --- " + diet.getNutrReq(model.ui_nutrNo)
                }

                model: []

                ScrollIndicator.vertical: ScrollIndicator {}

                onModelChanged: vitaminList.currentIndex = -1
            }
            ListView {
                id: mineralList
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                delegate: ItemDelegate {
                    width: parent.width
                    text: model.ui_nutrDesc + " " + model.ui_nutrVal + model.ui_units
                }

                model: []

                ScrollIndicator.vertical: ScrollIndicator {}

                onModelChanged: mineralList.currentIndex = -1
            }
        }
    }
}
