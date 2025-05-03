#include <SimpleStateMachine.h>

/**
 * @file
 * @brief   Simple state machine handler class
 * 
 *          This class provides a minimum state machine processor, that can run through
 *          an array of states. The states are injected through an array of state 
 *          descriptions. \see tFSM_State
 * 
 * @author    Ueli Niederer, ueli.niederer@niederer-engineering.ch         
 * @copyright Copyright (c) 2025 by Niederer Engineering GmbH
 */

static SSM_STATE_HANDLER(NullState);
static const tSSM_State NullState = SSM_STATE_DESCRIBE("NullState", NullState);

SimpleStateMachine::SimpleStateMachine(uintptr_t init, const tSSM_State *states, void *context) {
    m_states = states;
    while (states->name != 0) {
        m_states_len++;
        states++;
    }

    m_state_init = &m_states[init];
    m_state_curr = &NullState;
    m_state_next = m_state_init;
    m_context = context;
}

SimpleStateMachine::~SimpleStateMachine() {

}

void SimpleStateMachine::reset() {
    m_state_curr->handler(this, SSM_REASON_EXIT, m_context);
    m_state_curr = &NullState;
    m_state_next = m_state_init;
}

int SimpleStateMachine::run() {
    if (m_state_next != m_state_curr) {
        m_state_curr->handler(this, SSM_REASON_EXIT, m_context);
        m_state_next->handler(this, SSM_REASON_ENTER, m_context);
        m_state_curr = m_state_next;
    }
    return m_state_curr->handler(this, SSM_REASON_DO, m_context);
}

bool SimpleStateMachine::NextStateSet(uintptr_t nextState) {
    if (nextState >= m_states_len) {
        return false;
    }

    m_state_next = &m_states[nextState];
    return true;
}

uintptr_t SimpleStateMachine::CurrentStateGet() {
    return m_state_curr - m_states;
}

const char *SimpleStateMachine::CurrentStateNameGet() {
    return m_state_curr->name;
}

static SSM_STATE_HANDLER(NullState) {
    return 0;
}
