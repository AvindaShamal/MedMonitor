# MedMonitor
This is an IoT-based health monitoring system developed as a course project for EN3251: Internet of Things at the University of Moratuwa. MedMonitor allows for real-time tracking of vital health parameters, aiming to improve healthcare accessibility and reduce strain on medical facilities.

## Project Overview
The MedMonitor system continuously monitors essential health metrics such as blood oxygen levels (SpO2), heart rate, and body temperature. This data is transmitted to a cloud-based platform, where healthcare professionals and caregivers can monitor patient conditions remotely. Alerts are generated if any readings exceed defined threshold values, enabling timely intervention.

## Key Features
- **Real-Time Monitoring**: Continuous monitoring of SpO2, heart rate, and body temperature.
- **Data Visualization**: A Node-RED dashboard provides a live display of health metrics, including visual alerts for abnormal values.
- **Remote Alerts**: Notifications sent to healthcare providers and caregivers for prompt response when thresholds are crossed.

## Hardware Components
- **MAX30102**: Pulse oximeter and heart rate sensor.
- **MAX30205**: Temperature sensor, optimized for human body temperature monitoring.
- **ESP32**: Main microcontroller for collecting data and enabling Wi-Fi connectivity.

## Software and Cloud Components
- **Node-RED Dashboard**: User interface for live monitoring and alerts.
- **MQTT Protocol**: Enables efficient data transmission between sensors and the cloud.
- **ESP32 Arduino Code**: Interfaces sensors and transmits readings to the cloud.

## Setup Instructions
1. **Hardware Assembly**: Connect the MAX30102 and MAX30205 sensors to the ESP32, following wiring specifications.
2. **Software Installation**:
   - Install the Arduino libraries for the MAX30102, MAX30205, and Node-RED.
   - Configure the Node-RED dashboard for real-time visualization.
3. **MQTT Broker Configuration**:
   - Set up the MQTT broker on `broker.hivemq.com` for data publishing and subscribing.

## Functional Block Diagram
Refer to the project proposal document for detailed block diagrams illustrating data flow and system integration.

## Challenges and Solutions
Some challenges included sensor calibration for accuracy, real-time data transmission reliability, and user interface design. Solutions involved sensor adjustments, efficient use of MQTT for low-latency data transfer, and a user-friendly dashboard interface.

## Future Improvements
Future enhancements may include integrating machine vision, such as eye-tracking to monitor patient awareness levels, providing even more comprehensive care capabilities.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
