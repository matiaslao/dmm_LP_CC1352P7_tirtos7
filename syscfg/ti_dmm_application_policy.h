/*
 *  ======== ti_dmm_application_policy.h ========
 *  Configured DMM module definitions
 *
 *  DO NOT EDIT - This file is generated for the LP_CC1352P7_4
 *  by the SysConfig tool.
 */
#ifndef ti_dmm_application_policy__H_
#define ti_dmm_application_policy__H_

#include <dmm/dmm_policy.h>

/***** Defines *****/
//! \brief Application states bit map
#define DMMPOLICY_ANY 0xFFFFFFFF
#define DMMPOLICY_ZB_UNINIT 0x00000001
#define DMMPOLICY_ZB_PROVISIONING 0x00000002
#define DMMPOLICY_ZB_CONNECTED 0x00000004

//! \brief The application policy table
extern DMMPolicy_PolicyTable DMMPolicy_ApplicationPolicyTable;

//! \brief The application policy table size
extern uint32_t DMMPolicy_ApplicationPolicySize;

#endif //ti_dmm_application_policy__H_
