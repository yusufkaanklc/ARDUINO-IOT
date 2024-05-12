import express from "express";
import mongoose from "mongoose";
import StateModel from "./Model.js";
import { dirname } from "node:path";
import { fileURLToPath } from "node:url";
const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);
import path from "path";

const app = express();
const port = 5000;
let stateID;

mongoose
  .connect("mongodb://127.0.0.1:27017/arduino-test")
  .then(async () => {
    const existState = await StateModel.findOne();
    if (!existState) {
      const createState = new StateModel({
        lightState1: false,
        lightState2: false,
        LDRState1: false,
        LDRState2: false,
      });
      const newState = await createState.save();
      stateID = newState._id;
    } else {
      stateID = existState._id;
    }
  })
  .catch((err) => console.error("MongoDB bağlantı hatası:", err));

app.use(express.json());
app.use(express.static(path.join(__dirname, "public")));

app.get("/get-state", async (req, res) => {
  try {
    const findState = await StateModel.findById(stateID);
    if (!findState) {
      return res.status(404).json({ message: "Durum bulunamadı" });
    }
    res.status(200).json(findState);
  } catch (error) {
    res.status(500).json({ message: "Internal server error" });
  }
});

app.post("/update-state", async (req, res) => {
  try {
    const { lightState1, lightState2 } = req.body;
    const updatedFields = {};
    if (lightState1 !== undefined) updatedFields.lightState1 = lightState1;
    if (lightState2 !== undefined) updatedFields.lightState2 = lightState2;

    const updatedState = await StateModel.findByIdAndUpdate(
      stateID,
      updatedFields,
      { new: true }
    );

    if (!updatedState) {
      return res.status(404).json({ message: "Durum bulunamadı" });
    }

    res.status(200).json(updatedState);
  } catch (error) {
    res.status(500).json({ message: "Internal server error" });
  }
});

app.get("*", (req, res) => {
  res.sendFile(path.join(__dirname, "public", "index.html"));
});

app.listen(port, () => {
  console.log(`Server started at http://localhost:${port}`);
});
