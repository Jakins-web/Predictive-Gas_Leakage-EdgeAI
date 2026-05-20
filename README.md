# Intelligent Edge-AI Gas Leakage Detection System

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Python 3.12](https://img.shields.io/badge/Python-3.12-blue.svg)](https://www.python.org/)
[![Framework: Scikit-Learn](https://img.shields.io/badge/Framework-Scikit--Learn-orange.svg)](https://scikit-learn.org/)

This repository bridges the gap between passive IoT monitoring and predictive Edge AI. Developed originally as an IoT hardware solution at the Federal University of Technology, Akure (FUTA), this project evolved into an investigation of machine learning architectures optimized for zero-fail life-safety environments. 

By transitioning from static hardware threshold limits to a Random Forest Ensemble model, this system handles environmental noise and sensor cross-sensitivity to achieve a **99.998% classification accuracy**, effectively mitigating catastrophic industrial and residential false-negative risks.

---

## 📌 Project Architecture Overview

1. **Data Acquisition (IoT Layer):** An MQ5 gas sensor coupled with an ESP32 microcontroller deployed to log environmental hydrocarbon/gas concentrations, temperature, and relative humidity.
2. **Telemetry:** Historical sensor logs were curated into a robust, high-dimensional tabular dataset to capture non-linear environmental degradation and chemical lag.
3. **Machine Learning Pipeline:** Data preprocessing, standard scaling, and feature evaluation across multiple classification algorithms to enforce a strict zero-false-negative safety policy.

---

## 🛠️ Hardware & Tools Used
* **Microcontroller:** ESP32 (NodeMCU)
* **Gas Sensor:** MQ5 (LPG, Natural Gas, Town Gas detection)
* **Environment Sensors:** DHT11/DHT22 (Temperature & Humidity tracking)
* **Software Stack:** Python, Scikit-Learn, Pandas, NumPy, Matplotlib, Seaborn

---

## 📊 Machine Learning Model Evaluation

A key objective of this project was determining the optimal mathematical boundary for safety-critical classification. 

### 1. Linear & Logistic Regression (Baseline)
* **The Theory:** Attempted to establish a linear decision boundary hyperplane ($z = w^Tx + b$) passed through a sigmoid activation function.
* **The Flaw:** Yielded low $R^2$ scores and unoptimized accuracy. Real-world gas dispersion patterns, temperature shifts, and humidity variations are inherently non-linear. Forcing a straight line created multiple **Dangerous False Negatives** (failing to sound an alarm during a real leak).

### 2. Random Forest Ensemble (Production Model)
* **The Theory:** Utilized a bootstrap-aggregated committee of independent decision trees calculating structural splits based on Gini Impurity.
* **The Result:** The model successfully mapped out the non-linear "danger zones" within the multi-sensor space, achieving an accuracy of **99.998%**. 

### Performance Metrics Comparison

| Model Architecture | Accuracy | False Negatives (Critical Failures) | Best Deployment Target |
| :--- | :--- | :--- | :--- |
| **Logistic Regression** | Moderate | High (Unacceptable for Safety) | Ultra-low power microcontrollers |
| **Random Forest Ensemble**| **99.998%** | **0% (Calibrated Threshold)** | High-RAM Microcontrollers / Cloud Core |

---

## 📂 Repository Structure

```text
├── hardware/              # Arduino/ESP32 C++ code files (.ino)
├── data/                  # Sample sensor log datasets (CSV)
├── notebooks/             # Jupyter notebooks for model training & evaluation
├── models/                # Serialized production models (.pkl or .tflite)
└── README.md              # Project documentation
