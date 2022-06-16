#include "StateManager.hpp"

void StateManager::setState(_State state)
{
    current_state = state;
}

_State StateManager::getCurrentState()
{
    return current_state;
}