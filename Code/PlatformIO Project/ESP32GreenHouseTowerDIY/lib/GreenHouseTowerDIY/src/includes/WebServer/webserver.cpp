#include "webserverHandler.hpp"

HTTPDHandler::HTTPDHandler(Configuration *filterConfig)
{
    this->filterConfig = filterConfig;
    this->server = new AsyncWebServer(80);
}

void HTTPDHandler::startStreamServer()
{
    this->server->on(
        "/",
        HTTP_GET,
        std::bind(&HTTPDHandler::stream_handler, this, std::placeholders::_1));
    this->server->on(
        "/config/update",
        HTTP_GET,
        std::bind(&HTTPDHandler::config_update_handler, this, std::placeholders::_1));

    Serial.println("Initializing web server");
    this->server->begin();
}

void HTTPDHandler::config_update_handler(AsyncWebServerRequest *request)
{
    AsyncWebParameter *p = request->getParam("body", true);
    DynamicJsonDocument doc(DESERIALIZE_CONFIG_SIZE);
    DeserializationError error = deserializeJson(doc, p->value());

    if (error)
        return request->send(403);

    if (doc.containsKey("device"))
    {
        JsonObject deviceConfig = doc["device"].as<JsonObject>();
        this->filterConfig->updateDeviceConfig(deviceConfig, true);
    }
    if (doc.containsKey("camera"))
    {
        JsonObject cameraConfig = doc["camera"].as<JsonObject>();
        this->filterConfig->updateCameraConfig(cameraConfig, true);
    }
    if (doc.containsKey("wifi"))
    {
        JsonObject wifiConfig = doc["wifi"].as<JsonObject>();
        // this->filterConfig->updateNetwork();
    }

    this->filterConfig->save();

    return request->send(200);
}

void HTTPDHandler::stream_handler(AsyncWebServerRequest *request)
{
    /* AsyncJpegStreamResponse *response = new AsyncJpegStreamResponse();
    if (!response)
    {
        request->send(501);
        return;
    }
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response); */
}