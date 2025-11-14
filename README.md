#Air monitoring system

•	Developed a real-time air quality monitoring system using ESP32, programmed entirely in low-level C with direct register access for high performance and control.

•	Interfaced an MQ-135 gas sensor to measure analog gas concentration, calculated Rs/R₀ ratio, and applied log-log interpolation to estimate CO₂-equivalent values in ppm.

•	Displayed live air quality status on an I2C-connected LCD.

•	Integrated ThingSpeak IoT platform to upload, visualize, and analyze environmental data over Wi-Fi.

•	Designed modular codebase for scalability with additional sensors or wireless alerts.
