#pragma once

#include <functional>
#include <map>
#include <vector>

class StateMachine
{
public:
  enum Phase
  {
    Entry,
    Do,
    Exit
  };
  typedef std::function<void(Phase)> StateHandler;
  typedef uint16_t StateID;
  typedef uint32_t Transition;

  StateMachine(StateID initialState);

  void process();

protected:
  void registerState(StateID id, StateHandler handler);
  void registerTransition(StateID stateA, StateID stateB);

  void externalEvent(StateID nextState);
  void internalEvent(StateID nextState);

  bool isValidTransition(StateID stateA, StateID stateB);

  StateID getState();

private:
  std::map<StateID, StateHandler> stateHandlers_;
  Phase currentPhase_{Entry};
  StateID currentState_{0};
  StateID nextState_{0};
  bool pendingTransition_{false};
  std::vector<Transition> transitionTable_;
};