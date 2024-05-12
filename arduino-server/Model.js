import mongoose from "mongoose";

const { Schema } = mongoose;

const stateSchema = new Schema({
  lightState1: {
    type: Boolean,
    default: false,
  },
  lightState2: {
    type: Boolean,
    default: false,
  },
  LDRState1: {
    type: Boolean,
    default: false,
  },
  LDRState2: {
    type: Boolean,
    default: false,
  },
});

const StateModel = mongoose.model("State", stateSchema);

export default StateModel;
