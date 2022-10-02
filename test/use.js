const {Iotdb} = require("./mac/iotdb.node");

const iotdb = new Iotdb("127.0.0.1", 6667, "root", "root");

// let storage = iotdb.query("SHOW STORAGE GROUP;");
// console.log(storage)
let res = iotdb.query("show timeseries;");
console.log("res")
console.log(res)
console.log("res")
// iotdb.query("select * from root.sg_test_01;");
