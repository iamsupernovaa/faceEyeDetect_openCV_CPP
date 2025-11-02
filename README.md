# Real-Time Face & Eye Detection using OpenCV (C++)

### 👁 Overview
This project implements **real-time face and eye detection** using **Haar Cascade Classifiers** in **C++ with OpenCV**.  
It goes a step further by analysing **eye brightness ratios** to estimate whether the driver’s eyes are **open or closed** — making it a foundation for **Driver Drowsiness Detection Systems** used in modern **ADAS (Advanced Driver Assistance Systems)**.

---

### ⚙️ Features
- Real-time **face and eye detection** using OpenCV’s Haar Cascades  
- **Brightness ratio analysis** for open vs closed eye detection  
- **Dynamic waveform plotting** of eye brightness history  
- Visual alert overlay when eyes remain closed for several frames  
- Lightweight and modular — ready for integration with other automotive safety systems  

---

### 🧩 Tech Stack
- **Language:** C++  
- **Library:** OpenCV (>=4.0)  
- **Algorithms:** Haar Cascade Detection, Thresholding, Brightness Ratio Calculation  

---

### 🧠 How It Works
1. The camera captures live frames using `VideoCapture`.  
2. The face and eyes are detected using Haar Cascade models.  
3. Each eye region is converted to grayscale → blurred → histogram equalized → thresholded.  
4. The **brightness ratio** (white pixel ratio) determines whether the eye is open or closed.  
5. A **waveform** of the ratio is plotted dynamically for visualization.  
6. If eyes remain undetected for multiple frames, a **“Eyes Closed” alert** is displayed.

---

### 📂 File Structure
├── detect.cpp
├── Resources/
│   ├── haarcascade_frontalface_default.xml
│   ├── haarcascade_eye_tree_eyeglasses.xml
└── README.md

---

### 🧪 Setup & Usage
**1️⃣ Install OpenCV**  
```bash
brew install opencv      # macOS
sudo apt-get install libopencv-dev  # Ubuntu
