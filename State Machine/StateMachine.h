/* Design based on Jason Weimann's C# StateMachine.
 */

#pragma once

#include "CoreMinimal.h"
#include "State.h"
#include <functional>
#include <list>
#include <map>
#include <new>

class SYNTHBIOTICDUNGEONC_API StateMachine
{
	class Transition 
	{
	public:
		Transition(State * to, std::function<bool()> condition);
		std::function<bool()> _condition;
		State* _to;
	};
	Transition * GetTransition();
	State * _currentState = nullptr;
	std::map<int, std::list<Transition>> * _transitions = new std::map<int, std::list<Transition>>; 
	std::list<Transition> * _currentTransitions = new std::list<Transition>;
	std::list<Transition> * _anyTransitions = new std::list<Transition>;
	std::list<Transition> * _emptyList = new std::list<Transition>;

public:
	~StateMachine();
	void Tick();
	void SetState(State * state);
	void AddTransition(State * from, State * to, std::function<bool()> const& condition);
	void AddAnyTransition(State * to, std::function<bool()> condition);
};
