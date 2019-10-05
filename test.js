const test = require("./build/Release/native.node")
const pth = "../dam/test/2018/2018-11-16/DSC01283.ARW"



;(async ()=>{
    console.log((await test.base64(pth,5)).length) 
})()

