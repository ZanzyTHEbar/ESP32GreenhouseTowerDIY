#pragma once
#include <defines.hpp>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "network/networkStack/network.hpp"
#include "mbedtls/md.h"
class APIServer
{
public:
    APIServer(int CONTROL_PORT, Network *network);
    void begin();
    void startAPIServer();
    void findParam(AsyncWebServerRequest *request, const char *param, String &value);

    std::shared_ptr<AsyncWebServer> server;

    class API_Utilities
    {
    public:
        API_Utilities();
        void notFound(AsyncWebServerRequest *request);
        std::string shaEncoder(std::string data);
        std::unordered_map<WebRequestMethodComposite, std::string> _networkMethodsMap = {
            {HTTP_GET, "GET"},
            {HTTP_POST, "POST"},
            {HTTP_PUT, "PUT"},
            {HTTP_DELETE, "DELETE"},
            {HTTP_PATCH, "PATCH"},
            {HTTP_OPTIONS, "OPTIONS"},
        };
    };

private:
    void command_handler(AsyncWebServerRequest *request);
    void manageSecurity(AsyncWebServerRequest *request);
    bool is_authenticated(AsyncWebServerRequest *request);
    Network *network;

    /* Commands */
    void setSSID(const char *value);
    void setPass(const char *value);
    void setChannel(const char *value);

    void setDataJson(AsyncWebServerRequest *request);
    void setConfigJson(AsyncWebServerRequest *request);
    void setSettingsJson(AsyncWebServerRequest *request);

    void factoryReset();
    void rebootDevice();

    typedef std::function<void(const char *)> wifi_conf_function;
    typedef std::function<void(void)> function;
    typedef std::function<void(AsyncWebServerRequest *)> function_w_request;

    typedef std::unordered_map<std::string, function> command_map_funct_t;
    typedef std::unordered_map<std::string, wifi_conf_function> command_map_wifi_conf_t;
    typedef std::unordered_map<std::string, function_w_request> command_map_json_t;

    command_map_funct_t command_map_funct;
    command_map_wifi_conf_t command_map_wifi_conf;
    command_map_json_t command_map_json;

    static const char *MIMETYPE_HTML;
    /* static const char *MIMETYPE_CSS; */
    /* static const char *MIMETYPE_JS; */
    /* static const char *MIMETYPE_PNG; */
    /* static const char *MIMETYPE_JPG; */
    /* static const char *MIMETYPE_ICO; */
    static const char *MIMETYPE_JSON;

    std::string www_username;
    std::string www_password;
    std::string api_root;
};

extern APIServer::API_Utilities api_utilities;
