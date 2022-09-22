const { Iotdb } = require("../build/Release/hello-world-native.node");

const iotdb = new Iotdb("192.168.61.186", 6667, "root", "root");

iotdb.query("SHOW STORAGE GROUP;");