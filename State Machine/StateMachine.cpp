// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine.h"


StateMachine::~StateMachine()
{
    delete _currentState;
    delete _transitions;
    delete _currentTransitions;
    delete _anyTransitions;
    delete _emptyList;
}

StateMachine::Transition::Transition(State* to, std::function<bool()> condition)
{
    _to = to;
    _condition = condition;
}
StateMachine::Transition * StateMachine::GetTransition()
{
    for (std::list<Transition>::iterator transition = _anyTransitions->begin(); transition != _anyTransitions->end(); ++transition) {
        if (transition->_condition())
            return std::addressof(*transition);
    }
    for (std::list<Transition>::iterator transition = _currentTransitions->begin(); transition != _anyTransitions->end(); ++transition) {
        if (transition->_condition())
            return std::addressof(*transition);
    }
    
    return nullptr;
}

void StateMachine::Tick()
{
    StateMachine::Transition * transition = GetTransition();
    if (transition) {
        SetState(transition->_to);
    }
    if (_currentState) _currentState->Tick();
}

void StateMachine::SetState(State * state)
{
    if (state == _currentState) return;

    if (_currentState) _currentState->OnExit();
    _currentState = state;

    // Need to get the list of transitions for the current state. If its null, set to empty transitions.
    _currentTransitions = std::addressof(_transitions->at(_currentState->_id));
    if (_currentTransitions == nullptr) 
        _currentTransitions = _emptyList;

    _currentState->OnEnter();
}

void StateMachine::AddTransition(State* from, State* to, std::function<bool()> const& condition)
{
    if (_transitions->count(from->_id) > 0) {
        std::list<Transition> transitions;
        _transitions->insert(std::pair<int, std::list<Transition>>(from->_id, transitions));
    }
    _transitions->at(from->_id).push_back(Transition(to, condition));
}

void StateMachine::AddAnyTransition(State* to, std::function<bool()> condition)
{
    _anyTransitions->push_back(Transition(to, condition));
}

