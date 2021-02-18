/* Design based on Jason Weimann's C# StateMachine.
 */

#pragma once

#include "CoreMinimal.h"
#include "State.h"
#include <functional>
#include <list>
#include <unordered_map>
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
	
public:
	State * _currentState = nullptr;
	std::unordered_map<int, std::list<Transition*>*> * _transitions = new std::unordered_map<int, std::list<Transition*>*>; 
	std::list<Transition*> * _currentTransitions = nullptr;
	std::list<Transition*> * _anyTransitions = new std::list<Transition*>;
	std::list<Transition*> * _emptyTransitions = new std::list<Transition*>;
	~StateMachine();
	void Tick();
	void SetState(State * state);
	void AddTransition(State * from, State * to, std::function<bool()> const& condition);
	void AddAnyTransition(State * to, std::function<bool()> condition);
};
