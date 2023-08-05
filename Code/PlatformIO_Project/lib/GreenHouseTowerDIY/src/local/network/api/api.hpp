#pragma once

/**
 * @brief Base API Methods
 * @note This class is responsible for handling the API requests
 * @note We will use the Decorator Pattern to add new clients that consume the
 * API
 */
class API {
 public:
  API();
  virtual ~API();

  // TODO: Add the API methods here

  /**
   * @brief What methods do we need?
   * @note We need to be able to enable and disable features
   * @note We need to be able to set the configuration for mqtt, wifi, home
   * assistant, and the pump
   * @note We need to be able to get the current configuration for mqtt, wifi,
   * home assistant, and the pump
   * @note We need to be able to get the current state of the system
   *
   */
};