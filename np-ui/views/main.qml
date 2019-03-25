import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import NP 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    Text {
        text: masterController.ui_welcomeMessage
    }
    ColumnLayout {
        id: foodCol
        property FoodSearch ui_foodSearch: masterController.ui_foodSearch
        //property FoodDB database
        //signal foodItemSelected(FoodItem item)

        function clear()
        {
            foodSearch.text = ''
            foodGroupsComboBox.currentIndex = 0
        }

        TextField {
            id: foodSearch
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            focus: true
            z: 1
            selectByMouse: true
            onTextChanged: {
                resultsList.model = foodCol.ui_foodSearch.searchFoods(foodGroupsComboBox.currentIndex, foodSearch.text)
            }
        }

        ComboBox {
            id: foodGroupsComboBox
            editable: true
            textRole: "ui_desc"
            model: foodCol.ui_foodSearch.ui_foodGrps
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            onCurrentIndexChanged: {
                resultsList.model = foodCol.ui_foodSearch.searchFoods(foodGroupsComboBox.currentIndex, foodSearch.text)
            }
            Component.onCompleted: {
                contentItem.selectByMouse = true
            }
            z: 1
        }

        ListView {
            id: resultsList
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignTop
            currentIndex: -1
            model: []
            z: 0
            delegate: ItemDelegate {
                width: parent.width
                text: modelData.shrtDesc
                contentItem: Label {
                    wrapMode: Text.WordWrap
                    text: parent.text
                }

                highlighted: { if(ListView.isCurrentItem) true; else false}
                onClicked: {
                    resultsList.currentItem.highlighted = false
                    resultsList.currentIndex = index
                }
                onHoveredChanged: {
                    if (!ListView.isCurrentItem)
                        highlighted = !highlighted
                }
            }

            //onCurrentItemChanged: foodCol.foodItemSelected(database.getFood(resultsList.model[resultsList.currentIndex]))

            ScrollIndicator.vertical: ScrollIndicator { }
        }
    }
}
