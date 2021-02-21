// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine.h"


StateMachine::~StateMachine()
{
    std::list<Transition*>::iterator transition;
    // For each element, free its list
    
    for (int i = 0; i < _transitions->size(); i++) 
    {
        // For each list, free all Transitions
        for (transition = _transitions->at(i)->begin(); transition != _transitions->at(i)->end(); ++transition) { 
            delete *transition;
        }
        delete _transitions->at(i);
    }
    delete _transitions; // Finally, free the map itself
    
    // Free all Transitions
    for (transition = _anyTransitions->begin(); transition != _anyTransitions->end(); ++transition) { 
        delete *transition;
    }
    delete _anyTransitions;
    //delete _emptyTransitions;
}

StateMachine::Transition::Transition(State* to, std::function<bool()> condition)
{
    _to = to;
    _condition = condition;
}
StateMachine::Transition * StateMachine::GetTransition()
{
    std::list<Transition*>::iterator transition;
    for (transition = _anyTransitions->begin(); transition != _anyTransitions->end();) {
        if ((*transition) == nullptr || (*transition)->_condition == nullptr) break;
        if ((*transition)->_condition())
            return *transition;
        ++transition;
    }
    for (transition = _currentTransitions->begin(); transition != _currentTransitions->end();) {
        if ((*transition) == nullptr || (*transition)->_condition == nullptr) break;
        if ((*transition)->_condition())
            return *transition;
        ++transition;
    }
    return nullptr;
}

void StateMachine::Tick()
{
    Transition * transition = GetTransition();
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
    if (_transitions->count(_currentState->_id) > 0) // 
        _currentTransitions = _transitions->at(_currentState->_id);
    else
        _currentTransitions = _emptyTransitions;

    if (_currentTransitions == _emptyTransitions)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("_currentTrans is _empty")));
    }
    
    _currentState->OnEnter();
}

void StateMachine::AddTransition(State* from, State* to, std::function<bool()> const& condition)
{
    if (_transitions->count(from->_id) == 0) { // If this state is not in the map yet
        std::list<Transition*> *transitions = new std::list<Transition*>;
        _transitions->insert(std::pair<int, std::list<Transition*>*>(from->_id, transitions));
    }
    _transitions->at(from->_id)->push_back(new Transition(to, condition)); 
}

void StateMachine::AddAnyTransition(State* to, std::function<bool()> condition)
{
    _anyTransitions->push_back(new Transition(to, condition));
}
