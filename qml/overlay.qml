import QtQuick 1.0

Rectangle {
  width: 64
  height: 32
  radius: 4
  //color: "blue"
  gradient: Gradient {
    GradientStop { position: 0.0; color: "red" }
    GradientStop { position: 1.0; color: "magenta" }
  }
  PropertyAnimation on scale { 
    id: shrinkIt
    running: false
    to: 0
    duration: 1000
  }
  PropertyAnimation on width { 
    id: narrowIt
    running: false
    to: 0
    duration: 1200
  }
  function shrink (lag) {
    narrowIt.duration = lag;
    narrowIt.running = true;
    shrinkIt.running = true; 
  }
}