#include "stdafx.h"
#include "fsm.h"

State::State(const std::string& name, state_enter_method_t enter_method,
	state_exit_method_t exit_method) : m_name(name),
	m_enter_method(enter_method),
	m_exit_method(exit_method)
{
}

void State::add_transition(const std::string& next_state)
{
	m_next_states.push_back(next_state);
}

bool State::is_next_state_valid(const State* next_state) const
{
	return std::find(m_next_states.begin(), m_next_states.end(), next_state->get_name()) != m_next_states.end();
}

std::string State::get_name() const
{
	return m_name;
}

void State::enter(FSM* fsm, void* arg) const
{
	m_enter_method(fsm, arg);
}

void State::exit(FSM* fsm) const
{
	m_exit_method(fsm);
}

FSM::FSM(const std::string& name) : m_name(name),
m_cur_state(NULL),
m_prev_state(NULL),
m_next_state(NULL)
{
}

FSM::~FSM()
{
	m_cur_state = NULL;
	m_prev_state = NULL;
	m_next_state = NULL;

	for (state_map_t::iterator it = m_state_map.begin(); it != m_state_map.end(); ++it) {
		delete it->second;
	}

}

void FSM::set_FSM_name(const std::string& name)
{
	m_name = name;
}

std::string FSM::get_FSM_name() const
{
	return m_name;
}

void FSM::add_state(State* state)
{
	m_state_map[state->get_name()] = state;
}

bool FSM::is_in_transition()
{
	return (m_prev_state != NULL || m_next_state != NULL);
}

State* FSM::get_current_state() const
{
	return m_cur_state;
};

State* FSM::get_prev_state() const
{
	return m_prev_state;
};

State* FSM::get_next_state() const
{
	return m_next_state;
};

State* FSM::get_state_named(const std::string& state)
{
	for (state_map_t::iterator it = m_state_map.begin(); it != m_state_map.end(); ++it)
	{
		State* s = it->second;
		if (s->get_name() == state)
			return s;
	}

	return NULL;
}

void FSM::request(const std::string& next_state, void* arg)
{
	if (is_in_transition())
	{
		std::cerr << "already in transition!!" << std::endl;
		return;
	}

	m_next_state = get_state_named(next_state);
	if (!m_next_state)
	{
		std::cerr << "state undefined: " << next_state << std::endl;
		return;
	}

	if (m_cur_state && !m_cur_state->is_next_state_valid(m_next_state))
	{
		m_next_state = NULL;
		std::cerr << "no transition from " << m_cur_state->get_name() << " to " << next_state << std::endl;
		return;
	}

	m_prev_state = m_cur_state;
	m_cur_state = NULL;

	if (m_prev_state)
		m_prev_state->exit(this);

	m_next_state->enter(this, arg);

	m_cur_state = m_next_state;
	m_prev_state = NULL;
	m_next_state = NULL;
}

