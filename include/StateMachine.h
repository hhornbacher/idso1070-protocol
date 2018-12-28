#pragma once

#include <functional>
#include <map>
#include <vector>

#include <boost/thread.hpp>

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

  void start();
  void stop();

protected:
  void registerState(StateID id, StateHandler handler);
  void registerTransition(StateID stateA, StateID stateB);

  bool externalEvent(StateID nextState);
  void internalEvent(StateID nextState);

  void commitExternalEvent();

  bool isValidTransition(StateID stateA, StateID stateB);

  StateID getState();

  void process();

private:
  std::unique_ptr<boost::thread> thread_;
  std::map<StateID, StateHandler> stateHandlers_;
  Phase currentPhase_{Entry};
  StateID initialState_{0};
  StateID currentState_{0};
  StateID nextState_{0};
  bool pendingTransition_{false};
  std::vector<Transition> transitionTable_;
  bool run_{false};
};