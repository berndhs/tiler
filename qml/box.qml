import QtQuick 1.0

Rectangle {
  id: compexBox
  width: boxWidth
  height: boxHeight
  radius: 8
  rotation: 0
  objectName: "qmlBox"
  color: "yellow"
  opacity: 0.2
  border.color: "black"
  RotationAnimation on rotation { 
    id: turnit
    from: 0
    to: 360
    duration: 10000
    loops: 2 //Animation.Infinite
    direction: RotationAnimation.Clockwise 
    running: false;
  }
  
  signal finished (variant angle)

  function setShape (h, w) {
    console.log ("they want");
    console.log (w);
    console.log (h);
    width = w;
    height = h;
  }
  function readWidth () {
    return width;
  }
  function readHeight () {
    return height;
  }
  function rotate (degrees) {
    rotation = rotation + degrees;
  }
  function changeText (newText) {
    centralText.text = newText
    turnit.pause(); 
  }
  MouseArea {
    anchors.fill: parent
    onClicked: {
      cppBox.BigClicked (mouseX, mouseY);
    }
  }
  RoundButton {
    id: theButton
    objectName: "theButtonObject"
    anchors.left: compexBox.left
    anchors.leftMargin: 10
    anchors.top: compexBox.top
    anchors.topMargin: 20
    //color: "blue"
    MouseArea {
      id: wholeTheButton
      anchors.fill: parent
      onClicked: {      
        turnit.running = true;
        cppBox.Clicked ();
      }
    }
    Text {
      anchors.centerIn: parent
      text: "Start"
    }
  }
  RoundButton {
    id: finiButton
    objectName: "finiButtonObject"
    anchors.right: compexBox.right
    anchors.rightMargin: 4
    anchors.top: theButton.top
    //color: "green"
    MouseArea {
      id:finiArea
      anchors.fill: parent
      onClicked: {
        compexBox.finished (compexBox.rotation);
        theButton.shrink (2000);
      }
    }
    Text {
      anchors.centerIn: parent
      text: "Done"
    }
  }
  Text {
    id: centralText
    anchors.centerIn: parent
    text: "Hello Yellow World!"
  }
}
