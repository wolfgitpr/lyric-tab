import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 800
    height: 600

    property int itemCount: 10
    property int itemWidth: 80
    property bool wrap: true

    Column {
        width: parent.width
        spacing: 10
        anchors.centerIn: parent

        ScrollView {
            id: scrollView
            width: parent.width
            height: 400  // 设置容器高度

            Column {
                width: parent.width
                spacing: 10
                anchors.centerIn: parent

                // 使用 Flow 作为 ScrollView 的内容
                Flow {
                    id: flow
                    width: wrap ? scrollView.width : Math.max(scrollView.width, itemCount * itemWidth)
                    flow: wrap ? Flow.Wrap : Flow.LeftToRight
                    spacing: 10

                    Repeater {
                        model: itemCount
                        delegate: Item {
                            width: itemWidth
                            height: 30
                            Rectangle {
                                width: itemWidth
                                height: 30
                                color: "lightgreen"
                                Text {
                                    anchors.centerIn: parent
                                    text: "Item " + (index + 1)
                                }
                            }
                        }
                    }
                }
            }
        }

        CheckBox {
            text: "自动换行"
            checked: wrap
            onCheckedChanged: {
                wrap = checked
            }
        }

        Button {
            text: "增加项目"
            onClicked: {
                itemCount += 1
            }
        }

        Button {
            text: "减少项目"
            onClicked: {
                if (itemCount > 1) itemCount -= 1
            }
        }
    }

    // 动态调整 Flow 的宽度
    onWidthChanged: {
        flow.width = wrap ? scrollView.width : Math.max(scrollView.width, itemCount * itemWidth * 1.2)
    }
}
