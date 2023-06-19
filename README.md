### Description
This is a calculator backend API server that serves the following endpoints:
- `http://localhost:8080/add/4.2/3.1`
- `http://localhost:8080/substract/4.2/3.1`
- `http://localhost:8080/multiply/4.2/3.1`
- `http://localhost:8080/divide/4.2/3.1`

Every request is sent to the MQTT broker in the `ops` channel. 

Download the executable [Release.zip](https://github.com/daparic/CalculatorBackend/releases/download/0.0.1/Release.zip) 

### Run the backend API server and specifying the MQTT broker details
```bash
CalculatorBackend.exe tcp://192.168.2.146:1883 id12345 john p4ssw0rd
```

Test the API endpoint: `curl -s http://localhost:8080/add/4.2/3.1`

You can also build the solution using Visual Studio 2019. 
