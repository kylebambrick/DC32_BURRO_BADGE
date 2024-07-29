#include "burro_mesh.h"
#include <Arduino.h>

Scheduler userScheduler;
painlessMesh mesh;

int points = 0;
int modified = 1000;

Task taskSendMessage(TASK_SECOND, TASK_FOREVER, &sendMessage);

void sendMessage() {
  String msg = "pt: " + String(pattern) + ", ct: " + String(counter) + ", in: " + String(previousMillis) + ", lv: " + String(points);
  //Serial.printf("Sent     from %u msg=%s\n", mesh.getNodeId(), msg.c_str());
  mesh.sendBroadcast(msg);
}

void receivedCallback(uint32_t from, String &msg) {
  //Serial.printf("Received from %u msg=%s\n", from, msg.c_str());
  uint32_t msgPattern = 9999;
  uint32_t msgCounter = 9999;
  uint32_t msgInterval = 9999;
  uint32_t msgPoints = 9999;

  if (msg.startsWith("pt:")) {
    // Find the start and end positions of the Pattern value
    int ptStart = msg.indexOf("pt: ") + 4;
    int ptEnd = msg.indexOf(", ct: ");
    if (ptStart >= 4 && ptEnd > ptStart) {
      msgPattern = msg.substring(ptStart, ptEnd).toInt();
    }

    // Find the start and end positions of the Chase Step Counter value
    int ctStart = msg.indexOf("ct: ") + 4;
    int ctEnd = msg.indexOf(", in: ");
    if (ctStart >= 4 && ctEnd > ctStart) {
      msgCounter = msg.substring(ctStart, ctEnd).toInt();
    }

    // Find the start and end positions of the Chase Step Counter value
    int inStart = msg.indexOf("in: ") + 4;
    int inEnd = msg.indexOf(", lv: ");
    if (inStart >= 4 && inEnd > inStart) {
      msgInterval= msg.substring(inStart, inEnd).toInt();
    }

    // Find the start and end positions of the flag points
    int lvStart = msg.indexOf("lv: ") + 4;
    int lvEnd = msg.length();
    if (lvStart >= 4 && lvEnd > lvStart) {
      msgPoints = msg.substring(lvStart, lvEnd).toInt();
    }
  }

  //Serial.printf("msgPattern: %u, msgCounter: %u, msgPoints: %u\n", msgPattern, msgCounter, msgPoints);

  if (msgPattern != 9999 && msgCounter != 9999 && msgInterval != 9999 && msgPoints != 9999) {
    wink();
    //Serial.print("Checking for update\n");
    if (msgPoints > points || (msgPoints == points && from > mesh.getNodeId())) {
      //Serial.print("Found node with more points or higher id\n");
      meshCounter = 0;
      if (pattern != msgPattern) {
        //Serial.print("Updating pattern\n");
        pattern = msgPattern;
        forceChange = true;
        forceNotChange = true;
        okToChange = true;
        patternChangeMillis = 0;
        counter = msgCounter;
        previousMillis = msgInterval;
      }
      if (counter != msgCounter) {
        counter = msgCounter;
        previousMillis = msgInterval;
      }
    }
  }
}

void newConnectionCallback(uint32_t nodeId) {
  //Serial.printf("New Connection, nodeId = %u\n", nodeId);
  printNodeList();
}

void printNodeList() {
  auto nodeList = mesh.getNodeList();
  //Serial.println("Current Nodes in the Mesh Network:");
  for (auto const& node : nodeList) {
    //Serial.println(node);
  }
}

void changedConnectionCallback() {
  //Serial.printf("Changed connections\n");
  printNodeList();
}

void nodeTimeAdjustedCallback(int32_t offset) {
  //Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void initMesh() {
  //mesh.setDebugMsgTypes(ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE); // all types on

  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask(taskSendMessage);
  taskSendMessage.enable();

  //Serial.print("Mesh IP address: ");
  //Serial.println(WiFi.softAPIP());

  setPoints();
}

uint32_t updateMesh() {
  mesh.update();
  return mesh.getNodeTime();
}

void setPoints() {
  
  points = 0;

  if (flags.flagSGS)
    points += 1;

  if (flags.flagKyle)
    points += 9;

  if (flags.flagUser)
    points += 2;

  if (flags.flagPass)
    points += 2;

}