// https://discourse.panda3d.org/t/c-finite-state-machine/14748
#pragma once
#include "stdafx.h"

class FSM;

typedef void(*state_enter_method_t)(void*, void*);
typedef void(*state_exit_method_t)(void*);
typedef std::vector<std::string> state_vec_t;

class State
{
public:
	State(const std::string& name, state_enter_method_t enter_method,
		state_exit_method_t exit_method);

	void add_transition(const std::string& next_state);
	bool is_next_state_valid(const State* next_state) const;

	std::string get_name() const;

	void enter(FSM* fsm, void* arg) const;
	void exit(FSM* fsm) const;

private:
	std::string m_name;
	state_enter_method_t m_enter_method;
	state_exit_method_t m_exit_method;
	state_vec_t m_next_states;
};

typedef std::map<std::string, State*> state_map_t;

class FSM
{
public:
	FSM(const std::string& name);
	~FSM();

	void set_FSM_name(const std::string& name);
	std::string get_FSM_name() const;

	void add_state(State* state);

	bool is_in_transition();

	State* get_current_state() const;
	State* get_prev_state() const;
	State* get_next_state() const;

	State* get_state_named(const std::string& state);

	void request(const std::string& next_state, void* arg = NULL);

private:
	std::string m_name;
	State* m_cur_state;
	State* m_prev_state;
	State* m_next_state;
	state_map_t m_state_map;
};

#define DECL_STATE(CLASS, X) inline static void __enter_##X(void* _this, void* arg) {((CLASS*)_this)->enter_##X(arg);};\
                             inline static void __exit_##X(void* _this) {((CLASS*)_this)->exit_##X();};\
                             void enter_##X(void* arg);void exit_##X();

#define ADD_STATE(CLASS, X) add_state(new State(#X, &##CLASS::__enter_##X, &##CLASS::__exit_##X))
#define ADD_TRANSITION(X, Y) get_state_named(#X)->add_transition(#Y)

#define DEF_ENTER(CLASS, X) void CLASS::enter_##X(void* arg)
#define DEF_EXIT(CLASS, X) void CLASS::exit_##X()

#define EMPTY_ENTER(CLASS, X) void CLASS::enter_##X(void*){};
#define EMPTY_EXIT(CLASS, X) void CLASS::exit_##X(){};
