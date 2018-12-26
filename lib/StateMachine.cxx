#include <StateMachine.h>

StateMachine::StateMachine(StateID initialState) : currentState_(initialState)
{
}

void StateMachine::process()
{
  if (pendingTransition_)
  {
    currentPhase_ = Exit;
  }
  auto it = stateHandlers_.find(currentState_);
  if (it != stateHandlers_.end())
  {
    stateHandlers_[currentState_](currentPhase_);
  }
  switch (currentPhase_)
  {
  case Entry:
    currentPhase_ = Do;
    break;
  case Do:
    break;
  case Exit:
    currentState_ = nextState_;
    pendingTransition_ = false;
    currentPhase_ = Entry;
    break;
  }
}

void StateMachine::registerState(StateID id, StateHandler handler)
{
  auto it = stateHandlers_.find(id);
  if (it == stateHandlers_.end())
  {
    stateHandlers_[id] = handler;
    printf("Registered Handler for State: 0x%04x\n", id);
  }
}

void StateMachine::registerTransition(StateID stateA, StateID stateB)
{
  Transition transition = (Transition)stateA | ((Transition)stateB << 16);
  for (auto t : transitionTable_)
  {
    if (t == transition)
    {
      return;
    }
  }

  printf("Registered Transition: 0x%08x (A: 0x%04x, B: 0x%04x)\n", transition, stateA, stateB);
  transitionTable_.insert(transitionTable_.end(), transition);
}

void StateMachine::externalEvent(StateID nextState)
{
  if (isValidTransition(currentState_, nextState))
  {
    nextState_ = nextState;
    pendingTransition_ = true;
  }
  else
  {
    printf("Warning: Invalid transition! (A: 0x%04x, B: 0x%04x)\n", currentState_, nextState);
  }
}

void StateMachine::internalEvent(StateID nextState)
{
  if (isValidTransition(currentState_, nextState))
  {
    nextState_ = nextState;
    pendingTransition_ = true;
  }
  else
  {
    printf("Warning: Invalid transition! (A: 0x%04x, B: 0x%04x)\n", currentState_, nextState);
  }
}

StateMachine::StateID StateMachine::getState()
{
  return currentState_;
}

bool StateMachine::isValidTransition(StateID stateA, StateID stateB)
{
  Transition transition = (Transition)stateA | ((Transition)stateB << 16);
  for (auto t : transitionTable_)
  {
    if (t == transition)
    {
      return true;
    }
  }
  return false;
}