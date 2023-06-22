![Alt text](diagram.png?raw=true "diagram")

# Description
This is a calculator backend API server of the SPA [CalculatorFrontend](https://github.com/daparic/CalculatorFrontend). 
It serves the following endpoints:
- `'http://localhost:8080/add?param1=4.2&param2=3.1'`
- `'http://localhost:8080/subtract?param1=4.2&param2=3.1'`
- `'http://localhost:8080/multiply?param1=4.2&param2=3.1'`
- `'http://localhost:8080/divide?param1=4.2&param2=3.1'`

Each calculator operation request is sent to the MQTT broker `ops` channel. 

Download the executable [Release.zip](https://github.com/daparic/CalculatorBackend/releases/download/0.0.1/Release.zip) 

# Run an MQTT broker
There are two ways to quickly have an MQTT broker. I discourage the use of the native docker of Windows or Mac due to
the difficulty of accessing the container instance. 

#### I) MQTT using docker
```
git clone https://github.com/daparic/mosquitto-docker.git
cd mosquitto-docker
docker-compose up -d
```

#### II) MQTT using Windows installer
Install [mosquitto-2.0.15-install-windows-x64.exe](https://mosquitto.org/files/binary/win64/mosquitto-2.0.15-install-windows-x64.exe). 

Create a password file:
```
mosquitto_passwd.exe -c -b D:\password.file johnny p4ssw0rd
```

Create an MQTT configuration text file at `D:\mosquitto.conf` with the following two lines content:
```
listener 1883 0.0.0.0
password_file D:\password.file
```

Start the MQTT broker instance:
```
mosquitto.exe -v -c D:\mosquitto.conf
```

# Run the backend API server and specify the MQTT broker details
```bash
CalculatorBackend.exe tcp://localhost:1883 id12345 john p4ssw0rd
```

# Test the CalculatorBackend API endpoint: 
- By `curl -s 'http://localhost:8080/add?param1=4.2&param2=3.1'` 
- By SPA [CalculatorFrontend](https://github.com/daparic/CalculatorFrontend).

# Some useful extra tools
- [MQTT GUI Viewer](https://www.emqx.com/en/downloads/MQTTX/v1.9.3/MQTTX-Setup-1.9.3-x64.exe)
- [MQTT CLI Viewer](https://github.com/rainu/mqtt-shell/releases/download/v2.3.0/mqtt-shell-windows-amd64.exe)
- [MQTT Broker For Windows](https://mosquitto.org/files/binary/win64/mosquitto-2.0.15-install-windows-x64.exe)

The Jetson Nano also sends into the MQTT broker the recognized objects viewed from its attached USB video camera. 
