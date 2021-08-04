var zlib = require("zlib");

let data = [
  0x08, 0xd7, 0x63, 0xf8, 0xcf, 0xc0, 0x00, 0x00, 0x03, 0x01, 0x01, 0x00,
];

var arr = new Uint8Array(data);
// console.log(arr.toString())

zlib.inflate(arr, (err, buf) => {
  // console.log(buf)
    // console.log(buf.join(","));
    console.log("#" + buf.toString('hex').substr(2))
});

let newArr = new Uint8Array([0,255,0,0]);

zlib.deflate(newArr, (err, buf) => {
  let str = "";
  buf.forEach(num => {
    str += "0x" + num.toString(16) + " "
  })
  console.log(str)


  let str2 = "";
  data.forEach(num => {
    str2 += "0x" + num.toString(16) + " "
  })
  console.log(" "+str2)

  // zlib.inflate(buf, (err, buf) => {
  //   console.log("#" + buf.toString('hex').substr(2))
  //   // console.log(buf.join(","))
  // })
});

