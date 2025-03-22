from flask import Flask, request, jsonify
import joblib  # For loading AI model
import numpy as np

app = Flask(__name__)

# Load trained AI model (modify with actual path)
model = joblib.load("stroke_model.pkl")

def predict_stroke(heart_rate, emg, motion, bp):
    input_data = np.array([[heart_rate, emg, motion, bp]])
    prediction = model.predict(input_data)
    return "High Stroke Risk" if prediction[0] == 1 else "Low Stroke Risk"

@app.route("/sensor-data", methods=["POST"])
def receive_data():
    data = request.get_json()
    heart_rate = int(data["heartRate"])
    emg = int(data["emg"])
    motion = int(data["motion"])
    bp = int(data["bp"])
    
    risk_level = predict_stroke(heart_rate, emg, motion, bp)
    response = {"stroke_risk": risk_level}
    return jsonify(response)

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)
