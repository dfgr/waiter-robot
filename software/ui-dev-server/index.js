const express = require("express");
const app = express()
app.use(express.static('public'))
let r=125
let g =200;
let b = 100;

app.get("/", (req,res) =>{ 
    res.sendFile("./public/index.html")
})
app.get("/rgb",(req,res)=>{
    if (req.query.r != undefined && req.query.g != undefined && req.query.b != undefined){
        console.log("buraya dustu")
        r = req.query.r
        g = req.query.g
        b = req.query.b
        res.json({ r: r, g: g, b: b }).status(200);
    }
    res.json({ r: r, g: g, b: b }).status(200);
})
app.get("/rightmotor",(req,res)=>{
    
    console.log("sagmotordegeri"+req.query.s)
    res.json({ state: 1 }).status(200);
})
app.get("/leftmotor", (req, res) => {
    console.log("solmotordegeri:" + req.query.s)
    res.json({ state: 1 }).status(200);
})
app.get("/horn",(req,res)=>{
    
    if(req.query.i == "1"){
        console.log("kornaya sinyal gitti")
        res.json({state:1}).status(200);
    }
    else if (req.query.i == "0") {
        res.json({ state: 1 }).status(200);
        console.log("kornadan sinyal kesildi")
    }
    else {
        console.log("degisik seyler geldi kornaya")
    }
})
app.listen(8080);