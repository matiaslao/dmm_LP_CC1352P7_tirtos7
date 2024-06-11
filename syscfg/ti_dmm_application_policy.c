/*
 *  ======== ti_dmm_application_policy.c ========
 *  Configured DMM module definitions
 *
 *  DO NOT EDIT - This file is generated for the LP_CC1352P7_4
 *  by the SysConfig tool.
 */

#include <ti_dmm_application_policy.h>
#include <ti_dmm_custom_activities.h>

/***** Defines *****/

//! \brief Stack role policy indexes
#define CUSTOM1_STACK_POLICY_IDX 0
#define ZIGBEE_STACK_POLICY_IDX 1

DMMPolicy_Policy DMMPolicy_ApplicationPolicy[] = {
    // DMM Policy 0
    {
        // Custom 1 Policy
        .appState[CUSTOM1_STACK_POLICY_IDX] = {
            .state = DMMPOLICY_ANY,
            .weight = 0,
            .timingConstraint = DMMPOLICY_TIME_RESERVED,
            .pause = DMMPOLICY_NOT_PAUSED,
            .appliedActivity =  DMMPOLICY_APPLIED_ACTIVITY_NONE,
        },
        // Zigbee Router Policy
        .appState[ZIGBEE_STACK_POLICY_IDX] = {
            .state = DMMPOLICY_ANY,
            .weight = 1,
            .timingConstraint = DMMPOLICY_TIME_RESERVED,
            .pause = DMMPOLICY_NOT_PAUSED,
            .appliedActivity =  DMMPOLICY_APPLIED_ACTIVITY_NONE,
        },
        //Balanced Mode Policy
        .balancedMode = DMMPOLICY_BALANCED_NONE,
    },
};

DMMPolicy_PolicyTable DMMPolicy_ApplicationPolicyTable = {
    //Stack Roles
    .stackRole[CUSTOM1_STACK_POLICY_IDX] = DMMPolicy_StackRole_custom1,
    .stackRole[ZIGBEE_STACK_POLICY_IDX] = DMMPolicy_StackRole_ZigbeeRouter,
    //Policy table
    .policy = DMMPolicy_ApplicationPolicy,
    // Index Table for future use
    .indexTable = NULL,
};

//! \brief The application policy table size
uint32_t DMMPolicy_ApplicationPolicySize = (sizeof(DMMPolicy_ApplicationPolicy) / sizeof(DMMPolicy_Policy));
