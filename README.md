# ECE-570-Projects

The goal of this project was to create a system that reads data from the MPU6050 Gyroscope
sensor and communicates with an ESP32 module using I2C communication to establish a
master-slave network. Additionally, the ESP32 communicated wirelessly with a local server on
the computer using the UDP protocol. The project also included real-time data visualization,
where a 3D cube rotated based on the pitch and roll angles calculated from the sensor data. The
system built for this project includes an ESP32 connected to the home Wi-Fi network, enabling
wireless data transmission to the local server. The MPU6050 provided 3-axis acceleration,
temperature, and gyroscope data, with the acceleration data used to compute the pitch and roll
angles. A Python-based UDP server on the computer is responsible for receiving and interpreting
the sensor data from the ESP32, ensuring efficient communication and real-time feedback.
