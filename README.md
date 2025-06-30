# IOT-BASED-SMART-PARKING 🚗

![GitHub release](https://img.shields.io/github/release/hussain12145/IOT-BASED-SMART-PARKING.svg)

## Project Overview

This project is an RFID-based Smart Parking System using Arduino UNO. It integrates several components to create an efficient and user-friendly parking solution. The system uses RFID technology for user authentication, sensors for detecting vehicle presence, and cloud communication for data management.

### Features

- **RFID Authentication**: Uses MFRC522 RFID module for secure user identification.
- **Slot Occupancy Detection**: Employs IR sensors to monitor parking slot availability.
- **Vehicle Detection**: Utilizes ultrasonic sensors to detect vehicles at entry and exit points.
- **Gate Control**: Controls entry and exit gates with servo motors.
- **Cloud Data Management**: Connects to the internet via ESP8266 to upload parking data to Google Sheets.
- **User Feedback**: Provides real-time feedback through LEDs and buzzers.

## Hardware Components

| Component              | Quantity | Description                                  |
|-----------------------|----------|----------------------------------------------|
| Arduino UNO           | 1        | Main controller for the system               |
| MFRC522 RFID          | 1        | For scanning RFID tags                        |
| Servo Motor           | 2        | For entry and exit gates                     |
| HC-SR04               | 2        | For vehicle detection at entry and exit      |
| IR Sensors            | 3        | For detecting slot availability               |
| ESP8266 (NodeMCU)     | 1        | For cloud communication                       |

## Getting Started

To set up the IOT-BASED-SMART-PARKING system, follow these steps:

### Prerequisites

- **Arduino IDE**: Download and install the Arduino IDE from [Arduino's official website](https://www.arduino.cc/en/software).
- **Hardware**: Gather all the components listed above.
- **Libraries**: Install necessary libraries for RFID, ultrasonic sensors, and ESP8266 in the Arduino IDE.

### Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/hussain12145/IOT-BASED-SMART-PARKING.git
   cd IOT-BASED-SMART-PARKING
   ```

2. **Upload the Code**:
   Open the Arduino IDE and load the main code file from the repository. Select the correct board and port, then upload the code to the Arduino UNO.

3. **Connect Hardware**:
   Follow the schematic provided in the repository to connect all hardware components correctly.

4. **Set Up Cloud Communication**:
   Configure the ESP8266 settings to connect to your Wi-Fi network. Update the Google Sheets API key and other necessary parameters in the code.

5. **Test the System**:
   Power up the system and test the RFID authentication, vehicle detection, and cloud data upload functionalities.

### Using the System

- **Parking a Vehicle**: Scan the RFID tag at the entry gate. The system will check slot availability and open the gate if a slot is free.
- **Exiting the Parking**: Scan the RFID tag at the exit gate. The system will update the slot availability and open the gate.

## Releases

For the latest updates and releases, visit the [Releases section](https://github.com/hussain12145/IOT-BASED-SMART-PARKING/releases). You can download the latest version of the code and any additional resources needed to run the system.

## Topics

This project covers various topics related to IoT and smart parking solutions. Some of the key topics include:

- arduino-uno
- authorizer
- automated-parking
- car
- electric-vehicles
- esp8266
- futures-ideas
- innovation
- iot-automation
- madan-smartparking2025
- parking2025
- safe-settings
- security
- smart-parking-iot
- smartparking
- ticketing-system
- vehicle-monitoring

## Contributing

Contributions are welcome! If you have suggestions for improvements or new features, please open an issue or submit a pull request.

### How to Contribute

1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Make your changes and commit them.
4. Push to your branch.
5. Open a pull request.

## Acknowledgments

- Thanks to the Arduino community for their support and resources.
- Special thanks to contributors who help improve this project.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

## Contact

For any inquiries or support, please contact Madan R at [your-email@example.com].

---

This README provides a comprehensive overview of the IOT-BASED-SMART-PARKING project. For detailed instructions, code, and updates, please refer to the repository.