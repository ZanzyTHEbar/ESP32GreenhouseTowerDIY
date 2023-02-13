#ifndef NETWORK_STATE_HPP
#define NETWORK_STATE_HPP

#include <Arduino.h>
#include <WiFi.h>
#include <data/statemanager/Statemanager.hpp>

/**
 * @brief Function to map the WiFi status to the WiFiState_e enum
 */
static void mapWifiStateToState() {
  switch (WiFi.status()) {
    case wl_status_t::WL_IDLE_STATUS:
      wifiStateManager.setState(WiFiState_e::WiFiState_None);
    case wl_status_t::WL_NO_SSID_AVAIL:
      wifiStateManager.setState(WiFiState_e::WiFiState_Error);
    case wl_status_t::WL_SCAN_COMPLETED:
      wifiStateManager.setState(WiFiState_e::WiFiState_None);
    case wl_status_t::WL_CONNECTED:
      wifiStateManager.setState(WiFiState_e::WiFiState_Connected);
    case wl_status_t::WL_CONNECT_FAILED:
      wifiStateManager.setState(WiFiState_e::WiFiState_Error);
    case wl_status_t::WL_CONNECTION_LOST:
      wifiStateManager.setState(WiFiState_e::WiFiState_Disconnected);
    case wl_status_t::WL_DISCONNECTED:
      wifiStateManager.setState(WiFiState_e::WiFiState_Disconnected);
    default:
      wifiStateManager.setState(WiFiState_e::WiFiState_None);
  }
}

#endif
