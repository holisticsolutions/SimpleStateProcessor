#pragma once

/**
 * @file
 * @brief Simple state machine handler class
 * 
 * @author Ueli Niederer, ueli.niederer@niederer-engineering.ch         
 * @copyright Copyright (c) 2025 by Niederer Engineering GmbH
 */
 
#include <stdint.h>

class SimpleStateMachine;

/**
    \brief  Represents the potential reasons to enter a state handler 
*/
typedef enum eSSM_Reason {
    SSM_REASON_ENTER = 0,   /**< Handler is called to execute "enter" action */
    SSM_REASON_DO,          /**< Handler is called to execute "do" action */
    SSM_REASON_EXIT,        /**< Handler is called to execute "exit" action */

    SSM_REASON_COUNT
}tSSM_Reason;

typedef bool (*tSSM_StateHandler)(SimpleStateMachine *SSM, tSSM_Reason reason, void *context);
#define SSM_STATE_HANDLER(stateName) bool stateName ## Handler(SimpleStateMachine *fsm, tSSM_Reason reason, void *context)

/**
    \brief  Describes a single state for the FSM. For maximum portability, 
            use #SSM_STATE_DESCRIBE to create instances rather than 
            creating the instances yourself.
*/
typedef struct sSSM_State {
    const char *name;               /**< Printable name of the state */
    tSSM_StateHandler handler;      /**< State handler */
}tSSM_State;
#define SSM_STATE_DESCRIBE(stateName, stateHandler)    { stateName, stateHandler ## Handler }
#define SSM_STATE_LAST()                               { 0, 0 }

/**
    \brief  Simple finite state machine library
*/
class SimpleStateMachine
{
    public:
                        SimpleStateMachine(uintptr_t init, const tSSM_State *states, void *context);
        virtual         ~SimpleStateMachine();

        int             run();
        void            reset();

        bool            NextStateSet(uintptr_t nextState);
        uintptr_t       CurrentStateGet();
        const char *    CurrentStateNameGet();

    private:
    void             *m_context;

    const tSSM_State *m_states;
    uintptr_t         m_states_len;

    const tSSM_State *m_state_init; /**< Initial state */
    const tSSM_State *m_state_curr; /**< Current state running */
    const tSSM_State *m_state_next; /**< Next state to enter */
};
