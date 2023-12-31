#pragma once

#include <string>
#include <tuple>
#include <iostream>
#include "IResourceFactory.h"
#include "MQTTClient.h"

class CalcResourceFactory : public IResourceFactory {

public:

    CalcResourceFactory(const char *mqttAddress, const char *mqttClientId,
                        const char *mqttUsername, const char *mqttPassword);
    // CalcResourceFactory();
    ~CalcResourceFactory() {
        MQTTClient_disconnect(_client, 1000);
        MQTTClient_destroy(&_client);
    }

    shared_ptr<Resource> get_resource() const final;

private:

    float calculate(float num1, float num2, string operation);
    tuple<float, float, string> 
            get_path_parameters(const shared_ptr<Session> session) const;
    tuple<float, float, string> 
            get_path_parameters2(const shared_ptr<Session> session) const;
    string to_json(float result);
    void get_handler(const shared_ptr<Session> session);

    void publish(MQTTClient client, const char* topic, const char* payload) {
        MQTTClient_message pubmsg = MQTTClient_message_initializer;
        pubmsg.payload = static_cast<void*>(const_cast<char*>(payload));
        pubmsg.payloadlen = strlen(static_cast<const char*>(pubmsg.payload));
        pubmsg.qos = 2;
        pubmsg.retained = 0;
        MQTTClient_deliveryToken token;
        MQTTClient_publishMessage(client, topic, &pubmsg, &token);
        MQTTClient_waitForCompletion(client, token, 1000L);
        printf("Message '%s' with delivery token %d delivered\n", payload, token);
    }

    shared_ptr<Resource> _resource;
    MQTTClient _client;

    const char* _mqttAddress;
    const char* _mqttClientId;
    const char* _mqttUsername;
    const char* _mqttPassword;
};