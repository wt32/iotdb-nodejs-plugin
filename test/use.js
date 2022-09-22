const { Iotdb } = require("../build/Release/hello-world-native.node");

const iotdb = new Iotdb("127.0.0.1", 6667, "root", "root");

iotdb.query("SHOW STORAGE GROUP;");
