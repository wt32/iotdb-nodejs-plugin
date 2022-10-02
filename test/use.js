const {Iotdb} = require("./mac/iotdb.node");

const iotdb = new Iotdb("127.0.0.1", 6667, "root", "root");

let res = iotdb.query("show timeseries;");
console.log(res)
