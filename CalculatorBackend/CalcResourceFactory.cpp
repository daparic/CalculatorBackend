#include "CalcResourceFactory.h"

#include <sstream>
#include <iomanip>
#include "json.hpp"

using namespace nlohmann;

bool is_valid_float(const std::string& value) {
    try {
        std::size_t pos = 0;
        std::stof(value, &pos);
        return pos == value.length(); // Check if the entire string was consumed
    } catch (const std::exception&) {
        return false;
    }
}

int on_message(void* context, char* topicName, int topicLen, MQTTClient_message* message) {
    char* payload = static_cast<char*>(message->payload);
    printf("Received operation %s\n", payload);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

CalcResourceFactory::CalcResourceFactory(const char *mqttAddress,
                                         const char *mqttClientId,
                                         const char *mqttUsername,
                                         const char *mqttPassword)
    : _mqttAddress(mqttAddress), _mqttClientId(mqttClientId), _mqttUsername(mqttUsername),
      _mqttPassword(mqttPassword) {
    _resource = make_shared<Resource>();
    _resource->set_path("/{operation: add|subtract|multiply|divide}"); 
    _resource->set_method_handler("GET", 
        [&](const auto session) {
            get_handler(session);
        });

    MQTTClient_create(&_client, _mqttAddress, _mqttClientId, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.username = _mqttUsername;
    conn_opts.password = _mqttPassword;
    MQTTClient_setCallbacks(_client, NULL, NULL, on_message, NULL);
    int rc;
    if ((rc = MQTTClient_connect(_client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
}

shared_ptr<Resource> CalcResourceFactory::get_resource() const {
        return _resource;
}

float CalcResourceFactory::calculate(float num1, float num2, string operation) {
    if(operation == "add") {
        return num1 + num2;
    }
    else if(operation == "subtract") {
        return num1 - num2;
    }
    else if(operation == "multiply") {
        return num1 * num2;
    }
    else if(operation == "divide") {
        return num1 / num2;
    }
}

tuple<float, float, string> CalcResourceFactory::get_path_parameters(
        const shared_ptr<Session> session) const {

    const auto& request = session->get_request();
    string operation = "invalid";
    float num1, num2;
    if (request->has_query_parameter("param1") && request->has_query_parameter("param2")) {
        std::string param1 = request->get_query_parameter("param1");
        std::string param2 = request->get_query_parameter("param2");
        if (is_valid_float(param1) && is_valid_float(param2)) {
            operation = request->get_path_parameter("operation");
            num1 = std::stof(param1);
            num2 = std::stof(param2);
        }
    }

    return make_tuple(num1, num2, operation);
} 

string CalcResourceFactory::to_json(float result) {
    ostringstream str_stream;
    str_stream << result;
    json jsonResult = {
        {"result", str_stream.str()}
    };
    return jsonResult.dump();
}

void CalcResourceFactory::get_handler(const shared_ptr<Session> session) {
    const auto [num1, num2, operation] = get_path_parameters(session);
    string content = "{\"result\":\"invalid\"}";
    string msg = "nan";
    float result;
    if (operation != "invalid") {
        result = calculate(num1, num2, operation);
        msg = std::to_string(result);
        content = to_json(result);
    }

    string oper = operation + ":" + std::to_string(num1) + "," + std::to_string(num2) + "(" + msg + ")";
    publish(_client, "ops", oper.c_str());

    session->close(OK, content,
        { {"Content-Length", to_string(content.size())} });
}