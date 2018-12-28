#include <StateMachine.h>

StateMachine::StateMachine(StateID initialState) : thread_(nullptr), initialState_(initialState), currentState_(initialState)
{
}

void StateMachine::start()
{
  run_ = true;
  thread_ = std::make_unique<boost::thread>(boost::bind(&StateMachine::process, this));
}

void StateMachine::stop()
{
  run_ = false;
  thread_->join();
  thread_ = nullptr;
  currentPhase_ = Entry;
  currentState_ = initialState_;
  nextState_ = 0;
  pendingTransition_ = false;
}

void StateMachine::process()
{
  while (run_)
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
      printf("Entry into state: 0x%04x\n", currentState_);
      currentPhase_ = Do;
      break;
    case Do:
      break;
    case Exit:
      printf("Exit from state: 0x%04x\n", currentState_);
      currentState_ = nextState_;
      pendingTransition_ = false;
      currentPhase_ = Entry;
      break;
    }
  }
}

void StateMachine::registerState(StateID id, StateHandler handler)
{
  auto it = stateHandlers_.find(id);
  if (it == stateHandlers_.end())
  {
    stateHandlers_[id] = handler;
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
  transitionTable_.insert(transitionTable_.end(), transition);
}

bool StateMachine::externalEvent(StateID nextState)
{
  if (!run_)
  {
    printf("Warning: State machine not running\n");
    return false;
  }
  if (isValidTransition(currentState_, nextState))
  {
    nextState_ = nextState;
    return true;
  }
  else
  {
    printf("Warning: Invalid transition! (A: 0x%04x, B: 0x%04x)\n", currentState_, nextState);
    return false;
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

void StateMachine::commitExternalEvent()
{
  pendingTransition_ = true;
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