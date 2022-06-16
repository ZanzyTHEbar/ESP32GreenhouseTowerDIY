#ifndef WEBSERVERHANDLER_HPP
#define WEBSERVERHANDLER_HPP
#include <AsyncTCP.h>
#include "ArduinoJson.h"
#include "storage.hpp"
#include <ESPAsyncWebServer.h>

class HTTPDHandler
{
public:
    HTTPDHandler(Configuration *TrackerConfig);
    void startStreamServer();

private:
    void config_update_handler(AsyncWebServerRequest *request);
    void stream_handler(AsyncWebServerRequest *request);
    Configuration *filterConfig;
    AsyncWebServer *server;
};

#endif // WEBSERVERHANDLER_HPP