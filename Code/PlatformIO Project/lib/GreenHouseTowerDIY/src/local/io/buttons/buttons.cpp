#include "buttons.hpp"

// ***********************************************************
//                Buttons class code
// ***********************************************************
Buttons* Buttons::_instance = nullptr;

void onStateReceived() {
  if (Buttons::instance() == nullptr) {
    return;
  }

  Buttons::instance()->processState();
}

// Constructor, not much going on. Just passing the pin number in and the input
// state.
Buttons::Buttons(byte inPin, bool input)
    : mechButton(inPin),
      _messageCallback(nullptr),
      _connectedCallback(nullptr),
      _connectionFailedCallback(nullptr),
      _inPin(inPin),
      _input(input) {
  _instance = this;
  if (!input) {
    pinMode(inPin, OUTPUT);
  } else {
    pinMode(inPin, INPUT);
  }
}

// Destructor, nothing allocated so nothing to do.
Buttons::~Buttons(void) {}

// begin, Needed somewhere to place the hookup() call. begin is traditional.
bool Buttons::begin(void) {
  log_d("Initializing Buttons");
  Serial.println();

  if (!getPin(_inPin)) {
    log_e("Failed to initialize Buttons. Missing the buttons Pin number.");
    return false;
  }

  if (_initialized) {
    log_e("Buttons is already initialized");
    return false;
  }

  setCallback(onStateReceived);
  hookup();
  _initialized = true;
  return true;
}

// takeAction, this is what's called when there is no callback set.
void Buttons::takeAction(void) {
  if (!setAs) {
    log_d("Button is pressed");
  }
  return;
}

void Buttons::processState(void) {
  if (_messageCallback == nullptr) {
    takeAction();
    return;
  }

  _messageCallback();
}

// Your standard sketch loop()
void Buttons::ButtonLoop(void) {
  bool Buttonstate;
  idle();  // Let all the idlers have time to do their thing.
  Buttonstate =
      trueFalse();  // Have a look at what the current Buttons state is.
  digitalWrite(_inPin, !Buttonstate);  // Since the Buttons ground when pushed,
                                       // invert logic with !
}
