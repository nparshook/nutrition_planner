import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import NP 1.0;

Item {
    property FoodSearch ui_foodSearch: masterController.ui_foodSearch
    signal foodItemSelected(FoodItem item)

    function clear()
    {
        foodSearchText.text = ''
        foodGroupsComboBox.currentIndex = 0
    }

    ColumnLayout {
        anchors.fill: parent
        TextField {
            id: foodSearchText
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            focus: true
            z: 1
            selectByMouse: true
            onTextChanged: {
                resultsList.model = ui_foodSearch.searchFoods(foodGroupsComboBox.currentIndex, foodSearchText.text)
                console.log(resultsList.model)
            }
        }

        ComboBox {
            id: foodGroupsComboBox
            editable: true
            textRole: "ui_desc"
            model: ui_foodSearch.ui_foodGrps
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            onCurrentIndexChanged: {
                resultsList.model = ui_foodSearch.searchFoods(foodGroupsComboBox.currentIndex, foodSearchText.text)
                console.log(resultsList.model)
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
                text: modelData.ui_shrtDesc
                contentItem: Label {
                    wrapMode: Text.WordWrap
                    text: parent.text
                }

                highlighted: { if(ListView.isCurrentItem) true; else false}
                onClicked: {
                    resultsList.currentItem.highlighted = false
                    resultsList.currentIndex = index
                    resultsList.currentItem.highlighted = true
                }
                onHoveredChanged: {
                    if (!ListView.isCurrentItem)
                        highlighted = !highlighted
                }
            }

            onCurrentItemChanged: foodItemSelected(ui_foodSearch.getFood(resultsList.model[resultsList.currentIndex]))

            ScrollIndicator.vertical: ScrollIndicator { }
        }
    }
}
