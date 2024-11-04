# CommunityMesh

## Overview

CommunityMesh is a project designed to create a simple communication network that helps communities stay connected. It utilizes a Raspberry Pi as a server to host a library of resources and allows ESP32 devices to communicate in a mesh network. This guide will help you set up and use both the Raspberry Pi and ESP32 devices without needing advanced technical skills.

## Features

- **Easy Setup:** Simple instructions for setting up a Raspberry Pi as a server.
- **ESP32 Mesh Network:** Connect multiple ESP32 devices to communicate with each other and access the library.
- **User-Friendly Web Interface:** Access resources and communicate through a browser on your phone or computer.

## Getting Started

### Setting Up the Raspberry Pi 4 as a Server

1. **Hardware Requirements:**
   - Raspberry Pi 4
   - MicroSD card (at least 16GB)
   - Power supply for Raspberry Pi
   - Wi-Fi connection

2. **Install the Operating System:**
   - Download and install Raspberry Pi Imager from [Raspberry Pi Downloads](https://www.raspberrypi.com/software/).
   - Use the imager to install Raspberry Pi OS (recommended) on the MicroSD card.
   - Insert the MicroSD card into the Raspberry Pi and power it on.

3. **Set Up the Server:**
   - Connect to your Raspberry Pi using SSH or directly connect a monitor and keyboard.
   - Open a terminal and run the following commands:
     ```bash
     sudo apt update
     sudo apt install python3 python3-pip
     pip3 install flask
     ```
   - Download the CommunityMesh code to your Raspberry Pi (https://github.com/SilenceGeneric/CommunityMesh.git):
     ```bash
     git clone <https://github.com/SilenceGeneric/CommunityMesh.git>
     cd CommunityMesh/rpi4library
     ```

4. **Adding Content to the Library:**
   - Place your HTML files and any resources you want to share in the `library` folder within the `rpi4library` directory.
   - You can find resources from projects like [Internet in a Box](https://internet-in-a-box.org/) to use in your library.

5. **Run the Server:**
   - Start the Flask server with:
     ```bash
     python3 app.py
     ```
   - The server will run and listen for connections.

### Setting Up the ESP32 Node

1. **Hardware Requirements:**
   - ESP32 microcontroller board
   - LoRa module (if not integrated)
   - Access to a Wi-Fi network (if using Wi-Fi)

2. **Install Arduino IDE:**
   - Download and install the [Arduino IDE](https://www.arduino.cc/en/software).

3. **Install Required Libraries:**
   - Open the Arduino IDE.
   - Go to `Sketch` > `Include Library` > `Manage Libraries...` and install the following libraries:
     - LoRa library
     - WiFi library

4. **Configure the ESP32 Code:**
   - Open the CommunityMesh code in the Arduino IDE.
   - Enter your Wi-Fi SSID and password where indicated in the code.

5. **Upload the Code:**
   - Connect the ESP32 to your computer via USB.
   - Select the correct board and port in the Arduino IDE.
   - Click the "Upload" button to compile and upload the code.

### Accessing the Library and Communicating

1. **Connecting to the Mesh Network:**
   - Power on the ESP32 devices to establish the mesh network.
   - Ensure your personal device (phone or computer) is connected to the same Wi-Fi network.

2. **Using the Web Interface:**
   - Open a web browser and enter the IP address of your Raspberry Pi (usually `http://<raspberry-pi-ip>:5000`).
   - You should see the library access page where you can view available resources.

3. **Communicating with Other Nodes:**
   - Use the chat feature in the web interface to send messages to other nodes in the mesh network.

## Helpful Links

- For additional resources and example content, check out [Internet in a Box](https://internet-in-a-box.org/).
- Explore more about using the Raspberry Pi on the [Raspberry Pi Documentation](https://www.raspberrypi.com/documentation/).

## Contributing

CommunityMesh is open-source, and we welcome contributions! Feel free to fork the repository, make your changes, and submit a pull request.

## License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT). See the [LICENSE](LICENSE) file for details.

---
