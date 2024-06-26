/**************************************************************************************************
  Filename:       bdb_tlCommissioning.c
  Revised:        $Date: 2013-09-10 17:57:03 -0700 (Tue, 10 Sep 2013) $
  Revision:       $Revision: 35271 $

  Description:    Zigbee Cluster Library - Light Link commissioning cluster.


  Copyright 2011-2013 Texas Instruments Incorporated.

  All rights reserved not granted herein.
  Limited License.

  Texas Instruments Incorporated grants a world-wide, royalty-free,
  non-exclusive license under copyrights and patents it now or hereafter
  owns or controls to make, have made, use, import, offer to sell and sell
  ("Utilize") this software subject to the terms herein. With respect to the
  foregoing patent license, such license is granted solely to the extent that
  any such patent is necessary to Utilize the software alone. The patent
  license shall not apply to any combinations which include this software,
  other than combinations with devices manufactured by or for TI ("TI
  Devices"). No hardware patent is licensed hereunder.

  Redistributions must preserve existing copyright notices and reproduce
  this license (including the above copyright notice and the disclaimer and
  (if applicable) source code license limitations below) in the documentation
  and/or other materials provided with the distribution.

  Redistribution and use in binary form, without modification, are permitted
  provided that the following conditions are met:

    * No reverse engineering, decompilation, or disassembly of this software
      is permitted with respect to any software provided in binary form.
    * Any redistribution and use are licensed by TI for use only with TI Devices.
    * Nothing shall obligate TI to provide you with source code for the software
      licensed and provided to you in object code.

  If software source code is provided to you, modification and redistribution
  of the source code are permitted provided that the following conditions are
  met:

    * Any redistribution and use of the source code, including any resulting
      derivative works, are licensed by TI for use only with TI Devices.
    * Any redistribution and use of any object code compiled from the source
      code and any resulting derivative works, are licensed by TI for use
      only with TI Devices.

  Neither the name of Texas Instruments Incorporated nor the names of its
  suppliers may be used to endorse or promote products derived from this
  software without specific prior written permission.

  DISCLAIMER.

  THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
  OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**************************************************************************************************/


/*********************************************************************
 * INCLUDES
 */
#include <zstack/bdb/bdb.h>
#include <zstack/bdb/bdb_touchlink.h>
#include <zstack/common/touchlink/bdb_tl_commissioning.h>
#include <zstack/nwk/stub_aps.h>
#include "ti_zstack_config.h"
#include "zcl.h"
#include "zcl_general.h"
#include "stub_aps.h"

#if defined ( BDB_TL_TARGET ) || defined ( BDB_TL_INITIATOR )

/*********************************************************************
 * MACROS
 */

#define  bdbTL_AppSendInterPANCommand( a, b, c, d, e, f, g )       (bdbTL_SendInterPANCommand( a, b, c, d, e, f, g, TRUE ))
#define  bdbTL_StackSendInterPANCommand( a, b, c, d, e, f, g )     (bdbTL_SendInterPANCommand( a, b, c, d, e, f, g, FALSE ))


/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
bdbTLCBRec_t *bdbTLCBs = (bdbTLCBRec_t *)NULL;

// This is the Cluster ID List and should be filled with Application
// specific cluster IDs.
const cId_t touchLink_EP_InClusterList[TOUCHLINK_EP_MAX_INCLUSTERS] =
{
  ZCL_CLUSTER_ID_TOUCHLINK
};

const cId_t touchLink_EP_OutClusterList[TOUCHLINK_EP_MAX_OUTCLUSTERS] =
{
  ZCL_CLUSTER_ID_TOUCHLINK
};

SimpleDescriptionFormat_t touchLink_EP_SimpleDesc =
{
  TOUCHLINK_INTERNAL_ENDPOINT,         //  int Endpoint;
  TOUCHLINK_PROFILE_ID,                //  uint16_t AppProfId[2];
  TOUCHLINK_INTERNAL_DEVICE_ID,        //  uint16_t AppDeviceId[2];
  TOUCHLINK_DEVICE_VERSION,            //  int   AppDevVer:4;
  TOUCHLINK_INTERNAL_FLAGS,            //  int   AppFlags:4;
  TOUCHLINK_EP_MAX_INCLUSTERS,         //  byte  AppNumInClusters;
  (cId_t *)touchLink_EP_InClusterList, //  byte *pAppInClusterList;
  TOUCHLINK_EP_MAX_OUTCLUSTERS,        //  byte  AppNumInClusters;
  (cId_t *)touchLink_EP_OutClusterList //  byte *pAppInClusterList;
};

// Define endpoint structure to register with STUB APS for INTER-PAN support
endPointDesc_t touchLink_EP =
{
  TOUCHLINK_INTERNAL_ENDPOINT,
  0,
  NULL,
  (SimpleDescriptionFormat_t *)&touchLink_EP_SimpleDesc,  // No Simple description for this endpoint
  (afNetworkLatencyReq_t)0                                // No Network Latency req
};

/*********************************************************************
 * GLOBAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
static uint8_t bdbTLPluginRegisted = FALSE;
static bdbTL_InterPANCallbacks_t *pInterPANCBs = (bdbTL_InterPANCallbacks_t *)NULL;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static ZStatus_t bdbTL_SendInterPANCommand( uint8_t srcEP, afAddrType_t *destAddr, uint8_t cmd,
                                              uint8_t direction, uint8_t seqNum, uint16_t cmdFormatLen,
                                              uint8_t *cmdFormat, uint8_t isReqFromApp );
static ZStatus_t bdbTL_HdlIncoming(  zclIncoming_t *pInMsg );
static bdbTL_AppCallbacks_t *bdbTL_FindCallbacks( uint8_t endpoint );
static ZStatus_t bdbTL_HdlInSpecificCommands( zclIncoming_t *pInMsg );
static ZStatus_t bdbTL_ProcessInLLCmds( zclIncoming_t *pInMsg, bdbTL_AppCallbacks_t *pCBs );

static ZStatus_t bdbTL_ProcessInCmd_GetGrpIDsReq( zclIncoming_t *pInMsg, bdbTL_AppCallbacks_t *pCBs );
static ZStatus_t bdbTL_ProcessInCmd_GetEPListReq( zclIncoming_t *pInMsg, bdbTL_AppCallbacks_t *pCBs );

static ZStatus_t bdbTL_ProcessInCmd_EndpointInfo( zclIncoming_t *pInMsg, bdbTL_AppCallbacks_t *pCBs );
static ZStatus_t bdbTL_ProcessInCmd_GetGrpIDsRsp( zclIncoming_t *pInMsg, bdbTL_AppCallbacks_t *pCBs );
static ZStatus_t bdbTL_ProcessInCmd_GetEPListRsp( zclIncoming_t *pInMsg, bdbTL_AppCallbacks_t *pCBs );

static ZStatus_t bdbTL_HdlInInterPANCommands( zclIncoming_t *pInMsg );
static ZStatus_t bdbTL_ProcessInLLInterPANCmds( zclIncoming_t *pInMsg );

static void bdbTL_ParseInCmd_NwkJoinReq( uint8_t *pBuf, bdbTLNwkJoinReq_t *pReq );

static ZStatus_t bdbTL_ProcessInCmd_ScanReq( zclIncoming_t *pInMsg );
static ZStatus_t bdbTL_ProcessInCmd_DeviceInfoReq( zclIncoming_t *pInMsg );
static ZStatus_t bdbTL_ProcessInCmd_IdentifyReq( zclIncoming_t *pInMsg );
static ZStatus_t bdbTL_ProcessInCmd_ResetToFNReq( zclIncoming_t *pInMsg );
static ZStatus_t bdbTL_ProcessInCmd_NwkStartReq( zclIncoming_t *pInMsg );
static ZStatus_t bdbTL_ProcessInCmd_NwkJoinRtrReq( zclIncoming_t *pInMsg );
static ZStatus_t bdbTL_ProcessInCmd_NwkJoinEDReq( zclIncoming_t *pInMsg );
static ZStatus_t bdbTL_ProcessInCmd_NwkUpdateReq( zclIncoming_t *pInMsg );

static ZStatus_t bdbTL_ProcessInCmd_ScanRsp( zclIncoming_t *pInMsg );
static ZStatus_t bdbTL_ProcessInCmd_DeviceInfoRsp( zclIncoming_t *pInMsg );
static ZStatus_t bdbTL_ProcessInCmd_NwkStartRsp( zclIncoming_t *pInMsg );
static ZStatus_t bdbTL_ProcessInCmd_NwkJoinRtrRsp( zclIncoming_t *pInMsg );
static ZStatus_t bdbTL_ProcessInCmd_NwkJoinEDRsp( zclIncoming_t *pInMsg );


/*********************************************************************
 * @fn      bdbTL_RegisterInterPANCmdCallbacks
 *
 * @brief   Register an applications Inter-PAN command callbacks
 *
 * @param   callbacks - pointer to the callback record.
 *
 * @return  ZSuccess
 */
ZStatus_t bdbTL_RegisterInterPANCmdCallbacks( bdbTL_InterPANCallbacks_t *callbacks )
{
  // Register as a ZCL Plugin
  if ( !bdbTLPluginRegisted )
  {
    zcl_registerPlugin( ZCL_CLUSTER_ID_TOUCHLINK,
                        ZCL_CLUSTER_ID_TOUCHLINK,
                        bdbTL_HdlIncoming );
    bdbTLPluginRegisted = TRUE;
  }
  pInterPANCBs = callbacks;

  return ( ZSuccess );
}

/*********************************************************************
 * @fn      bdbTL_RegisterCmdCallbacks
 *
 * @brief   Register an applications command callbacks
 *
 * @param   endpoint - application's endpoint
 * @param   callbacks - pointer to the callback record.
 *
 * @return  ZMemError if not able to allocate
 */
ZStatus_t bdbTL_RegisterCmdCallbacks( uint8_t endpoint, bdbTL_AppCallbacks_t *callbacks )
{
  bdbTLCBRec_t *pNewItem;
  bdbTLCBRec_t *pLoop;

  // Register as a ZCL Plugin
  if ( !bdbTLPluginRegisted )
  {
    zcl_registerPlugin( ZCL_CLUSTER_ID_TOUCHLINK,
                        ZCL_CLUSTER_ID_TOUCHLINK,
                        bdbTL_HdlIncoming );
    bdbTLPluginRegisted = TRUE;
  }

  // Fill in the new profile list
  pNewItem = zcl_mem_alloc( sizeof( bdbTLCBRec_t ) );
  if ( pNewItem == NULL )
    return (ZMemError);

  pNewItem->next = (bdbTLCBRec_t *)NULL;
  pNewItem->endpoint = endpoint;
  pNewItem->CBs = callbacks;

  // Find spot in list
  if ( bdbTLCBs == NULL )
  {
    bdbTLCBs = pNewItem;
  }
  else
  {
    // Look for end of list
    pLoop = bdbTLCBs;
    while ( pLoop->next != NULL )
      pLoop = pLoop->next;

    // Put new item at end of list
    pLoop->next = pNewItem;
  }

  return ( ZSuccess );
}

/*********************************************************************
 * @fn      bdbTL_HdlIncoming
 *
 * @brief   Callback from ZCL to process incoming Commands specific
 *          to this cluster library or Profile commands for attributes
 *          that aren't in the attribute list
 *
 * @param   pInMsg - pointer to the incoming message
 *
 * @return  ZStatus_t
 */
ZStatus_t bdbTL_HdlIncoming(  zclIncoming_t *pInMsg )
{
  ZStatus_t status = ZSuccess;

  if ( StubAPS_InterPan( pInMsg->msg->srcAddr.panId, pInMsg->msg->srcAddr.endPoint ) )
  {
    return ( bdbTL_HdlInInterPANCommands( pInMsg ) );
  }

  if ( zcl_ClusterCmd( pInMsg->hdr.fc.type ) )
  {
    // Is this a manufacturer specific command?
    if ( pInMsg->hdr.fc.manuSpecific == 0 )
    {
      status = bdbTL_HdlInSpecificCommands( pInMsg );
    }
    else
    {
      // We don't support any manufacturer specific command.
      status = ZFailure;
    }
  }
  else
  {
    // Handle all the normal (Read, Write...) commands -- should never get here
    status = ZFailure;
  }
  return ( status );
}

/*********************************************************************
 * @fn      touchLink_SendDeviceInfoRsp
 *
 * @brief   Send out a Device Information Response command.
 *
 * @param   srcEP - sender's endpoint
 * @param   dstAddr - destination address
 * @param   startIndex - start index
 * @param   transID - received transaction id
 * @param   seqNum - received sequence number
 *
 * @return  ZStatus_t
 */
uint8_t touchLink_SendDeviceInfoRsp( uint8_t srcEP, afAddrType_t *dstAddr, uint8_t startIndex,
                             uint32_t transID, uint8_t seqNum )
{
  bdbTLDeviceInfoRsp_t *pRsp;
  uint8_t cnt;
  uint8_t rspLen;
  uint8_t status = ZSuccess;

  cnt = touchLink_GetNumSubDevices( startIndex );
  if ( cnt > TOUCHLINK_DEVICE_INFO_RSP_REC_COUNT_MAX )
  {
    cnt = TOUCHLINK_DEVICE_INFO_RSP_REC_COUNT_MAX; // should be between 0x00-0x05
  }

  rspLen = sizeof( bdbTLDeviceInfoRsp_t ) + ( cnt * sizeof( devInfoRec_t ) );

  pRsp = (bdbTLDeviceInfoRsp_t *)OsalPort_malloc( rspLen );
  if ( pRsp )
  {
    pRsp->transID = transID;

    pRsp->numSubDevices = touchLink_GetNumSubDevices( 0 );
    pRsp->startIndex = startIndex;
    pRsp->cnt = cnt;

    for ( uint8_t i = 0; i < cnt; i++ )
    {
      devInfoRec_t *pRec = &(pRsp->devInfoRec[i]);

      osal_cpyExtAddr( pRec->ieeeAddr, NLME_GetExtAddr() );

      touchLink_GetSubDeviceInfo( startIndex + i, &(pRec->deviceInfo) );

      pRec->sort = 0;
    }

    // Send a response back
    status = bdbTL_Send_DeviceInfoRsp( srcEP, dstAddr, pRsp, seqNum );

    OsalPort_free( pRsp );
  }
  else
  {
    status = ZMemError;
  }

  return ( status );
}

/*********************************************************************
 * @fn      bdbTL_Send_ScanReq
 *
 * @brief   Call to send out an Scan Request command
 *
 * @param   srcEP - sending application's endpoint
 * @param   dstAddr - where you want the message to go
 * @param   pReq - request parameters
 * @param   seqNum - sequence number
 *
 * @return  ZStatus_t
 */
ZStatus_t bdbTL_Send_ScanReq( uint8_t srcEP, afAddrType_t *dstAddr,
                              bdbTLScanReq_t *pReq, uint8_t seqNum )
{
  uint8_t buf[TOUCHLINK_CMDLEN_SCAN_REQ];

  VOID zcl_buffer_uint32( buf, pReq->transID );

  buf[4] = pReq->zInfo.zInfoByte;
  buf[5] = pReq->touchLinkInfo.touchLinkInfoByte;

  return bdbTL_StackSendInterPANCommand( srcEP, dstAddr, COMMAND_TOUCHLINK_SCAN_REQ,
                                    ZCL_FRAME_CLIENT_SERVER_DIR, seqNum, TOUCHLINK_CMDLEN_SCAN_REQ, buf );
}

/*********************************************************************
 * @fn      bdbTL_Send_DeviceInfoReq
 *
 * @brief   Call to send out a Device Information Request command
 *
 * @param   srcEP - sending application's endpoint
 * @param   dstAddr - where you want the message to go
 * @param   pReq - request parameters
 * @param   seqNum - sequence number
 *
 * @return  ZStatus_t
 */
ZStatus_t bdbTL_Send_DeviceInfoReq( uint8_t srcEP, afAddrType_t *dstAddr,
                                    bdbTLDeviceInfoReq_t *pReq, uint8_t seqNum )
{
  uint8_t buf[TOUCHLINK_CMDLEN_DEVICE_INFO_REQ];

  VOID zcl_buffer_uint32( buf, pReq->transID );

  buf[4] = pReq->startIndex;

  return bdbTL_StackSendInterPANCommand( srcEP, dstAddr, COMMAND_TOUCHLINK_DEVICE_INFO_REQ,
                                    ZCL_FRAME_CLIENT_SERVER_DIR, seqNum, TOUCHLINK_CMDLEN_DEVICE_INFO_REQ, buf );
}

/*********************************************************************
 * @fn      bdbTL_Send_ScanRsp
 *
 * @brief   Call to send out an Scan Response command
 *
 * @param   srcEP - sending application's endpoint
 * @param   dstAddr - where you want the message to go
 * @param   pRsp - response parameters
 * @param   seqNum - sequence number
 *
 * @return  ZStatus_t
 */
ZStatus_t bdbTL_Send_ScanRsp( uint8_t srcEP, afAddrType_t *dstAddr,
                              bdbTLScanRsp_t *pRsp, uint8_t seqNum )
{
  uint8_t *buf;
  uint8_t bufLen = TOUCHLINK_CMDLEN_SCAN_RSP;
  ZStatus_t status;

  // Calculate the total length needed
  if ( pRsp->numSubDevices == 1 )
  {
    bufLen += TOUCHLINK_CMDLENOPTIONAL_SCAN_RSP;
  }

  buf = zcl_mem_alloc( bufLen );
  if ( buf != NULL )
  {
    uint8_t *pBuf = buf;

    pBuf = zcl_buffer_uint32( pBuf, pRsp->transID );

    *pBuf++ = pRsp->rssiCorrection;
    *pBuf++ = pRsp->zInfo.zInfoByte;
    *pBuf++ = pRsp->touchLinkInfo.touchLinkInfoByte;

    *pBuf++ = LO_UINT16( pRsp->keyBitmask );
    *pBuf++ = HI_UINT16( pRsp->keyBitmask );

    pBuf = zcl_buffer_uint32( pBuf, pRsp->responseID );
    pBuf = zcl_cpyExtAddr( pBuf, pRsp->extendedPANID );

    *pBuf++ = pRsp->nwkUpdateId;
    *pBuf++ = pRsp->logicalChannel;

    *pBuf++ = LO_UINT16( pRsp->PANID );
    *pBuf++ = HI_UINT16( pRsp->PANID );

    *pBuf++ = LO_UINT16( pRsp->nwkAddr );
    *pBuf++ = HI_UINT16( pRsp->nwkAddr );

    *pBuf++ = pRsp->numSubDevices;
    *pBuf++ = pRsp->totalGrpIDs;

    if ( pRsp->numSubDevices == 1 )
    {
      *pBuf++ = pRsp->deviceInfo.endpoint;

      *pBuf++ = LO_UINT16( pRsp->deviceInfo.profileID );
      *pBuf++ = HI_UINT16( pRsp->deviceInfo.profileID );

      *pBuf++ = LO_UINT16( pRsp->deviceInfo.deviceID );
      *pBuf++ = HI_UINT16( pRsp->deviceInfo.deviceID );

      *pBuf++ = pRsp->deviceInfo.version;
      *pBuf++ = pRsp->deviceInfo.grpIdCnt;
    }

    status = bdbTL_StackSendInterPANCommand( srcEP, dstAddr, COMMAND_TOUCHLINK_SCAN_RSP,
                                        ZCL_FRAME_SERVER_CLIENT_DIR, seqNum, bufLen, buf );
    zcl_mem_free( buf );
  }
  else
  {
    status = ZMemError;
  }

  return ( status );
}

/*********************************************************************
 * @fn      bdbTL_Send_IndentifyReq
 *
 * @brief   Call to send out a Identify Request command
 *
 * @param   srcEP - sending application's endpoint
 * @param   dstAddr - where you want the message to go
 * @param   pReq - request parameters
 * @param   seqNum - sequence number
 *
 * @return  ZStatus_t
 */
ZStatus_t bdbTL_Send_IndentifyReq( uint8_t srcEP, afAddrType_t *dstAddr,
                                   bdbTLIdentifyReq_t *pReq, uint8_t seqNum )
{
  uint8_t buf[TOUCHLINK_CMDLEN_IDENTIFY_REQ];

  VOID zcl_buffer_uint32( buf, pReq->transID );

  buf[4] = LO_UINT16( pReq->IdDuration );
  buf[5] = HI_UINT16( pReq->IdDuration );

  return bdbTL_StackSendInterPANCommand( srcEP, dstAddr, COMMAND_TOUCHLINK_IDENTIFY_REQ,
                                    ZCL_FRAME_CLIENT_SERVER_DIR, seqNum, TOUCHLINK_CMDLEN_IDENTIFY_REQ, buf );
}

/*********************************************************************
 * @fn      bdbTL_Send_ResetToFNReq
 *
 * @brief   Call to send out a Reset to Factory New Request command
 *
 * @param   srcEP - sending application's endpoint
 * @param   dstAddr - where you want the message to go
 * @param   pReq - request parameters
 * @param   seqNum - sequence number
 *
 * @return  ZStatus_t
 */
ZStatus_t bdbTL_Send_ResetToFNReq( uint8_t srcEP, afAddrType_t *dstAddr,
                                   bdbTLResetToFNReq_t *pReq, uint8_t seqNum )
{
  uint8_t buf[TOUCHLINK_CMDLEN_RESET_TO_FN_REQ];

  VOID zcl_buffer_uint32( buf, pReq->transID );

  return bdbTL_StackSendInterPANCommand( srcEP, dstAddr, COMMAND_TOUCHLINK_RESET_TO_FN_REQ,
                                    ZCL_FRAME_CLIENT_SERVER_DIR, seqNum, TOUCHLINK_CMDLEN_RESET_TO_FN_REQ, buf );
}

/*********************************************************************
 * @fn      bdbTL_Send_NwkStartReq
 *
 * @brief   Call to send out a Network Start Request command
 *
 * @param   srcEP - sending application's endpoint
 * @param   dstAddr - where you want the message to go
 * @param   pRsp - response parameters
 * @param   seqNum - sequence number
 *
 * @return  ZStatus_t
 */
ZStatus_t bdbTL_Send_NwkStartReq( uint8_t srcEP, afAddrType_t *dstAddr,
                                  bdbTLNwkStartReq_t *pRsp, uint8_t seqNum )
{
  uint8_t *buf;
  uint8_t *pBuf;
  ZStatus_t status;

  buf = zcl_mem_alloc( TOUCHLINK_CMDLEN_NWK_START_REQ );
  if ( buf != NULL )
  {
    pBuf = buf;

    pBuf = zcl_buffer_uint32( pBuf, pRsp->transID );
    pBuf = zcl_cpyExtAddr( pBuf, pRsp->nwkParams.extendedPANID );

    *pBuf++ = pRsp->nwkParams.keyIndex;

    pBuf = zcl_memcpy( pBuf, pRsp->nwkParams.nwkKey, SEC_KEY_LEN );

    *pBuf++ = pRsp->nwkParams.logicalChannel;

    *pBuf++ = LO_UINT16( pRsp->nwkParams.panId );
    *pBuf++ = HI_UINT16( pRsp->nwkParams.panId );

    *pBuf++ = LO_UINT16( pRsp->nwkParams.nwkAddr );
    *pBuf++ = HI_UINT16( pRsp->nwkParams.nwkAddr );

    *pBuf++ = LO_UINT16( pRsp->nwkParams.grpIDsBegin );
    *pBuf++ = HI_UINT16( pRsp->nwkParams.grpIDsBegin );

    *pBuf++ = LO_UINT16( pRsp->nwkParams.grpIDsEnd );
    *pBuf++ = HI_UINT16( pRsp->nwkParams.grpIDsEnd );

    *pBuf++ = LO_UINT16( pRsp->nwkParams.freeNwkAddrBegin );
    *pBuf++ = HI_UINT16( pRsp->nwkParams.freeNwkAddrBegin );

    *pBuf++ = LO_UINT16( pRsp->nwkParams.freeNwkAddrEnd );
    *pBuf++ = HI_UINT16( pRsp->nwkParams.freeNwkAddrEnd );

    *pBuf++ = LO_UINT16( pRsp->nwkParams.freeGrpIDBegin );
    *pBuf++ = HI_UINT16( pRsp->nwkParams.freeGrpIDBegin );

    *pBuf++ = LO_UINT16( pRsp->nwkParams.freeGrpIDEnd );
    *pBuf++ = HI_UINT16( pRsp->nwkParams.freeGrpIDEnd );

    pBuf = zcl_cpyExtAddr( pBuf, pRsp->initiatorIeeeAddr );

    *pBuf++ = LO_UINT16( pRsp->initiatorNwkAddr );
    *pBuf++ = HI_UINT16( pRsp->initiatorNwkAddr );

    status = bdbTL_StackSendInterPANCommand( srcEP, dstAddr, COMMAND_TOUCHLINK_NWK_START_REQ,
                                        ZCL_FRAME_CLIENT_SERVER_DIR, seqNum, TOUCHLINK_CMDLEN_NWK_START_REQ, buf );
    zcl_mem_free( buf );
  }
  else
  {
    status = ZMemError;
  }

  return ( status );
}

/*********************************************************************
 * @fn      bdbTL_Send_NwkJoinReq
 *
 * @brief   Call to send out a Network Join Router/End Device Request command
 *
 * @param   srcEP - sending application's endpoint
 * @param   dstAddr - where you want the message to go
 * @param   pRsp - response parameters
 * @param   cmd - COMMAND_TOUCHLINK_NWK_JOIN_RTR_REQ or COMMAND_TOUCHLINK_NWK_JOIN_ED_REQ
 * @param   seqNum - sequence number
 *
 * @return  ZStatus_t
 */
ZStatus_t bdbTL_Send_NwkJoinReq( uint8_t srcEP, afAddrType_t *dstAddr,
                                 bdbTLNwkJoinReq_t *pRsp, uint8_t cmd, uint8_t seqNum )
{
  uint8_t *buf;
  uint8_t *pBuf;
  ZStatus_t status;

  buf = zcl_mem_alloc( TOUCHLINK_CMDLEN_NWK_JOIN_REQ );
  if ( buf != NULL )
  {
    pBuf = buf;

    pBuf = zcl_buffer_uint32( pBuf, pRsp->transID );
    pBuf = zcl_cpyExtAddr( pBuf, pRsp->nwkParams.extendedPANID );

    *pBuf++ = pRsp->nwkParams.keyIndex;

    pBuf = zcl_memcpy( pBuf, pRsp->nwkParams.nwkKey, SEC_KEY_LEN );

    *pBuf++ = pRsp->nwkUpdateId;
    *pBuf++ = pRsp->nwkParams.logicalChannel;

    *pBuf++ = LO_UINT16( pRsp->nwkParams.panId );
    *pBuf++ = HI_UINT16( pRsp->nwkParams.panId );

    *pBuf++ = LO_UINT16( pRsp->nwkParams.nwkAddr );
    *pBuf++ = HI_UINT16( pRsp->nwkParams.nwkAddr );

    *pBuf++ = LO_UINT16( pRsp->nwkParams.grpIDsBegin );
    *pBuf++ = HI_UINT16( pRsp->nwkParams.grpIDsBegin );

    *pBuf++ = LO_UINT16( pRsp->nwkParams.grpIDsEnd );
    *pBuf++ = HI_UINT16( pRsp->nwkParams.grpIDsEnd );

    *pBuf++ = LO_UINT16( pRsp->nwkParams.freeNwkAddrBegin );
    *pBuf++ = HI_UINT16( pRsp->nwkParams.freeNwkAddrBegin );

    *pBuf++ = LO_UINT16( pRsp->nwkParams.freeNwkAddrEnd );
    *pBuf++ = HI_UINT16( pRsp->nwkParams.freeNwkAddrEnd );

    *pBuf++ = LO_UINT16( pRsp->nwkParams.freeGrpIDBegin );
    *pBuf++ = HI_UINT16( pRsp->nwkParams.freeGrpIDBegin );

    *pBuf++ = LO_UINT16( pRsp->nwkParams.freeGrpIDEnd );
    *pBuf++ = HI_UINT16( pRsp->nwkParams.freeGrpIDEnd );

    status = bdbTL_StackSendInterPANCommand( srcEP, dstAddr, cmd, ZCL_FRAME_CLIENT_SERVER_DIR, seqNum,
                                        TOUCHLINK_CMDLEN_NWK_JOIN_REQ, buf );
    zcl_mem_free( buf );
  }
  else
  {
    status = ZMemError;
  }

  return ( status );
}

/*********************************************************************
 * @fn      bdbTL_Send_NwkUpdateReq
 *
 * @brief   Call to send out a Network Update Request command
 *
 * @param   srcEP - sending application's endpoint
 * @param   dstAddr - where you want the message to go
 * @param   pReq - response parameters
 * @param   seqNum - sequence number
 *
 * @return  ZStatus_t
 */
ZStatus_t bdbTL_Send_NwkUpdateReq( uint8_t srcEP, afAddrType_t *dstAddr,
                                   bdbTLNwkUpdateReq_t *pReq, uint8_t seqNum )
{
  uint8_t *buf;
  uint8_t *pBuf;
  ZStatus_t status;

  buf = zcl_mem_alloc( TOUCHLINK_CMDLEN_NWK_UPDATE_REQ );
  if ( buf != NULL )
  {
    pBuf = buf;

    pBuf = zcl_buffer_uint32( pBuf, pReq->transID );
    pBuf = zcl_cpyExtAddr( pBuf, pReq->extendedPANID );

    *pBuf++ = pReq->nwkUpdateId;
    *pBuf++ = pReq->logicalChannel;

    *pBuf++ = LO_UINT16( pReq->PANID );
    *pBuf++ = HI_UINT16( pReq->PANID );

    *pBuf++ = LO_UINT16( pReq->nwkAddr );
    *pBuf++ = HI_UINT16( pReq->nwkAddr );

    status = bdbTL_StackSendInterPANCommand( srcEP, dstAddr, COMMAND_TOUCHLINK_NWK_UPDATE_REQ,
                                        ZCL_FRAME_CLIENT_SERVER_DIR, seqNum, TOUCHLINK_CMDLEN_NWK_UPDATE_REQ, buf );
    zcl_mem_free( buf );
  }
  else
  {
    status = ZMemError;
  }

  return ( status );
}

/*********************************************************************
 * @fn      bdbTL_Send_DeviceInfoRsp
 *
 * @brief   Call to send out a Device Information Response command
 *
 * @param   srcEP - sending application's endpoint
 * @param   dstAddr - where you want the message to go
 * @param   pRsp - response parameters
 * @param   seqNum - sequence number
 *
 * @return  ZStatus_t
 */
ZStatus_t bdbTL_Send_DeviceInfoRsp( uint8_t srcEP, afAddrType_t *dstAddr,
                                    bdbTLDeviceInfoRsp_t *pRsp, uint8_t seqNum )
{
  uint8_t *buf;
  uint8_t bufLen = TOUCHLINK_CMDLEN_DEVICE_INFO_RSP;
  ZStatus_t status;

  // Calculate the total length needed
  bufLen += ( pRsp->cnt * TOUCHLINK_CMDLENOPTIONAL_DEVICE_INFO_RSP );

  buf = zcl_mem_alloc( bufLen );
  if ( buf != NULL )
  {
    uint8_t *pBuf = buf;
    uint8_t i;

    pBuf = zcl_buffer_uint32( pBuf, pRsp->transID );

    *pBuf++ = pRsp->numSubDevices;
    *pBuf++ = pRsp->startIndex;
    *pBuf++ = pRsp->cnt;

    // Device information record
    for ( i = 0; i < pRsp->cnt; i++ )
    {
      devInfoRec_t *pRec = &(pRsp->devInfoRec[i]);

      pBuf = zcl_cpyExtAddr( pBuf, pRec->ieeeAddr );

      *pBuf++ = pRec->deviceInfo.endpoint;

      *pBuf++ = LO_UINT16( pRec->deviceInfo.profileID );
      *pBuf++ = HI_UINT16( pRec->deviceInfo.profileID );

      *pBuf++ = LO_UINT16( pRec->deviceInfo.deviceID );
      *pBuf++ = HI_UINT16( pRec->deviceInfo.deviceID );

      *pBuf++ = pRec->deviceInfo.version;
      *pBuf++ = pRec->deviceInfo.grpIdCnt;

      *pBuf++ = pRec->sort;
    }

    status = bdbTL_AppSendInterPANCommand( srcEP, dstAddr, COMMAND_TOUCHLINK_DEVICE_INFO_RSP,
                                           ZCL_FRAME_SERVER_CLIENT_DIR, seqNum, bufLen, buf );
    zcl_mem_free( buf );
  }
  else
  {
    status = ZMemError;
  }

  return ( status );
}

/*********************************************************************
 * @fn      bdbTL_Send_NwkStartRsp
 *
 * @brief   Call to send out a Network Start Response command
 *
 * @param   srcEP - sending application's endpoint
 * @param   dstAddr - where you want the message to go
 * @param   pRsp - response parameters
 * @param   seqNum - sequence number
 *
 * @return  ZStatus_t
 */
ZStatus_t bdbTL_Send_NwkStartRsp( uint8_t srcEP, afAddrType_t *dstAddr,
                                  bdbTLNwkStartRsp_t *pRsp, uint8_t seqNum )
{
  uint8_t *buf;
  ZStatus_t status;

  buf = zcl_mem_alloc( TOUCHLINK_CMDLEN_NWK_START_RSP );
  if ( buf != NULL )
  {
    uint8_t *pBuf = buf;

    pBuf = zcl_buffer_uint32( pBuf, pRsp->transID );

    *pBuf++ = pRsp->status;

    pBuf = zcl_cpyExtAddr( pBuf, pRsp->extendedPANID );

    *pBuf++ = pRsp->nwkUpdateId;
    *pBuf++ = pRsp->logicalChannel;

    *pBuf++ = LO_UINT16( pRsp->panId );
    *pBuf++ = HI_UINT16( pRsp->panId );

    status = bdbTL_StackSendInterPANCommand( srcEP, dstAddr, COMMAND_TOUCHLINK_NWK_START_RSP,
                                        ZCL_FRAME_SERVER_CLIENT_DIR, seqNum, TOUCHLINK_CMDLEN_NWK_START_RSP, buf );
    zcl_mem_free( buf );
  }
  else
  {
    status = ZMemError;
  }

  return ( status );
}

/*********************************************************************
 * @fn      bdbTL_Send_NwkJoinRsp
 *
 * @brief   Call to send out a Network Join Response command
 *
 * @param   srcEP - sending application's endpoint
 * @param   dstAddr - where you want the message to go
 * @param   pRsp - response parameters
 * @param   cmd - COMMAND_TOUCHLINK_NWK_JOIN_RTR_RSP or COMMAND_TOUCHLINK_NWK_JOIN_ED_RSP
 * @param   seqNum - sequence number
 *
 * @return  ZStatus_t
 */
ZStatus_t bdbTL_Send_NwkJoinRsp( uint8_t srcEP, afAddrType_t *dstAddr,
                                 bdbTLNwkJoinRsp_t *pRsp, uint8_t cmd, uint8_t seqNum )
{
  uint8_t buf[TOUCHLINK_CMDLEN_NWK_JOIN_RSP];

  VOID zcl_buffer_uint32( buf, pRsp->transID );

  buf[4] = pRsp->status;

  return bdbTL_StackSendInterPANCommand( srcEP, dstAddr, cmd, ZCL_FRAME_SERVER_CLIENT_DIR, seqNum,
                                    TOUCHLINK_CMDLEN_NWK_JOIN_RSP, buf );
}

/*********************************************************************
 * @fn      bdbTL_Send_EndpointInfo
 *
 * @brief   Call to send out an Endpoint Information command
 *
 * @param   srcEP - sending application's endpoint
 * @param   dstAddr - where you want the message to go
 * @param   pCmd - cmd parameters
 * @param   disableDefaultRsp - whether to disable the Default Response command
 * @param   seqNum - sequence number
 *
 * @return  ZStatus_t
 */
ZStatus_t bdbTL_Send_EndpointInfo( uint8_t srcEP, afAddrType_t *dstAddr,
                                   bdbTLEndpointInfo_t *pCmd,
                                   uint8_t disableDefaultRsp, uint8_t seqNum )
{
  uint8_t *buf;
  ZStatus_t status;

  buf = zcl_mem_alloc( TOUCHLINK_CMDLEN_EP_INFO );
  if ( buf != NULL )
  {
    uint8_t *pBuf = buf;

    pBuf = zcl_cpyExtAddr( pBuf, pCmd->ieeeAddr );

    *pBuf++ = LO_UINT16( pCmd->nwkAddr );
    *pBuf++ = HI_UINT16( pCmd->nwkAddr );

    *pBuf++ = pCmd->endpoint;

    *pBuf++ = LO_UINT16( pCmd->profileID );
    *pBuf++ = HI_UINT16( pCmd->profileID );

    *pBuf++ = LO_UINT16( pCmd->deviceID );
    *pBuf++ = HI_UINT16( pCmd->deviceID );

    *pBuf++ = pCmd->version;

    status = zcl_SendCommand( srcEP, dstAddr, ZCL_CLUSTER_ID_TOUCHLINK,
                              COMMAND_TOUCHLINK_EP_INFO, TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                              disableDefaultRsp, 0, seqNum, TOUCHLINK_CMDLEN_EP_INFO, buf );
    zcl_mem_free( buf );
  }
  else
  {
    status = ZMemError;
  }

  return ( status );
}

/*********************************************************************
 * @fn      bdbTL_Send_GetGrpIDsRsp
 *
 * @brief   Call to send out a Get Group Identifiers Response command
 *
 * @param   srcEP - sending application's endpoint
 * @param   dstAddr - where you want the message to go
 * @param   pRsp - response parameters
 * @param   disableDefaultRsp - whether to disable the Default Response command
 * @param   seqNum - sequence number
 *
 * @return  ZStatus_t
 */
ZStatus_t bdbTL_Send_GetGrpIDsRsp( uint8_t srcEP, afAddrType_t *dstAddr,
                                    bdbTLGetGrpIDsRsp_t *pRsp,
                                    uint8_t disableDefaultRsp, uint8_t seqNum )
{
  uint8_t *buf;
  uint8_t bufLen = TOUCHLINK_CMDLEN_GET_GRP_IDS_RSP;
  ZStatus_t status;

  // Calculate the total length needed
  bufLen += ( pRsp->cnt * TOUCHLINK_CMDLENOPTIONAL_GET_GRP_IDS_RSP );

  buf = zcl_mem_alloc( bufLen );
  if ( buf != NULL )
  {
    uint8_t *pBuf = buf;
    uint8_t i;

    *pBuf++ = pRsp->total;
    *pBuf++ = pRsp->startIndex;
    *pBuf++ = pRsp->cnt;

    // Group information record
    for ( i = 0; i < pRsp->cnt; i++ )
    {
      grpInfoRec_t *pRec = &(pRsp->grpInfoRec[i]);

      *pBuf++ = LO_UINT16( pRec->grpID );
      *pBuf++ = HI_UINT16( pRec->grpID );

      *pBuf++ = pRec->grpType;
    }

    status = zcl_SendCommand( srcEP, dstAddr, ZCL_CLUSTER_ID_TOUCHLINK,
                              COMMAND_TOUCHLINK_GET_GRP_IDS_RSP, TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                              disableDefaultRsp, 0, seqNum, bufLen, buf );
    zcl_mem_free( buf );
  }
  else
  {
    status = ZMemError;
  }

  return ( status );
}

/*********************************************************************
 * @fn      bdbTL_Send_GetEPListRsp
 *
 * @brief   Call to send out a Get Endpoint List Response command
 *
 * @param   srcEP - sending application's endpoint
 * @param   dstAddr - where you want the message to go
 * @param   pRsp - response parameters
 * @param   disableDefaultRsp - whether to disable the Default Response command
 * @param   seqNum - sequence number
 *
 * @return  ZStatus_t
 */
ZStatus_t bdbTL_Send_GetEPListRsp( uint8_t srcEP, afAddrType_t *dstAddr,
                                   bdbTLGetEPListRsp_t *pRsp,
                                   uint8_t disableDefaultRsp, uint8_t seqNum )
{
  uint8_t *buf;
  uint8_t bufLen = TOUCHLINK_CMDLEN_GET_EP_LIST_RSP;
  ZStatus_t status;

  // Calculate the total length needed
  bufLen += ( pRsp->cnt * TOUCHLINK_CMDLENOPTIONAL_GET_EP_LIST_RSP );

  buf = zcl_mem_alloc( bufLen );
  if ( buf != NULL )
  {
    uint8_t *pBuf = buf;
    uint8_t i;

    *pBuf++ = pRsp->total;
    *pBuf++ = pRsp->startIndex;
    *pBuf++ = pRsp->cnt;

    // Endpoint information record
    for ( i = 0; i < pRsp->cnt; i++ )
    {
      epInfoRec_t *pRec = &(pRsp->epInfoRec[i]);

      *pBuf++ = LO_UINT16( pRec->nwkAddr );
      *pBuf++ = HI_UINT16( pRec->nwkAddr );

      *pBuf++ = pRec->endpoint;

      *pBuf++ = LO_UINT16( pRec->profileID );
      *pBuf++ = HI_UINT16( pRec->profileID );

      *pBuf++ = LO_UINT16( pRec->deviceID );
      *pBuf++ = HI_UINT16( pRec->deviceID );

      *pBuf++ = pRec->version;
    }

    status = zcl_SendCommand( srcEP, dstAddr, ZCL_CLUSTER_ID_TOUCHLINK,
                              COMMAND_TOUCHLINK_GET_EP_LIST_RSP, TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                              disableDefaultRsp, 0, seqNum, bufLen, buf );
    zcl_mem_free( buf );
  }
  else
  {
    status = ZMemError;
  }

  return ( status );
}

/*********************************************************************
 * @fn      bdbTL_SendInterPANCommand
 *
 * @brief   Used to send TOUCHLINK Profile and Cluster Specific Inter-PAN Command
 *          messages.
 *
 *          NOTE: The calling application is responsible for incrementing
 *                the Sequence Number.
 *
 * @param   srcEp - source endpoint
 * @param   destAddr - destination address
 * @param   cmd - command ID
 * @param   direction - direction of the command
 * @param   seqNumber - identification number for the transaction
 * @param   cmdFormatLen - length of the command to be sent
 * @param   cmdFormat - command to be sent
 *
 * @return  ZSuccess if OK
 */
ZStatus_t bdbTL_SendInterPANCommand( uint8_t srcEP, afAddrType_t *destAddr, uint8_t cmd,
                                     uint8_t direction, uint8_t seqNum, uint16_t cmdFormatLen,
                                     uint8_t *cmdFormat, uint8_t isReqFromApp )
{
  ZStatus_t status;
  //
  // Note: TOUCHLINK Frame Control has a different format than ZCL Frame Control
  //

  // TOUCHLINK Header Format:
  // - Frame control (1 octet):
  //   * b3-b0: TOUCHLINK version (0b0000)
  //   * b7-b4: Reserved (0b0000)
  // - Transaction sequence number (1 octet)
  // - Command identifier (1 octet)

  if( isReqFromApp == TRUE)
  {
    status = zcl_SendCommand( srcEP, destAddr, ZCL_CLUSTER_ID_TOUCHLINK,
                            cmd, TRUE, direction, TRUE, 0,
                            seqNum, cmdFormatLen, cmdFormat );
  }
  else
  {
    status = zcl_StackSendCommand( srcEP, destAddr, ZCL_CLUSTER_ID_TOUCHLINK,
                          cmd, TRUE, direction, TRUE, 0,
                          seqNum, cmdFormatLen, cmdFormat );
  }
  return status;
}

/*********************************************************************
 * @fn      bdbTL_FindCallbacks
 *
 * @brief   Find the callbacks for an endpoint
 *
 * @param   endpoint
 *
 * @return  pointer to the callbacks
 */
static bdbTL_AppCallbacks_t *bdbTL_FindCallbacks( uint8_t endpoint )
{
  bdbTLCBRec_t *pCBs;

  pCBs = bdbTLCBs;
  while ( pCBs )
  {
    if ( pCBs->endpoint == endpoint )
      return ( pCBs->CBs );
    pCBs = pCBs->next;
  }

  return ( (bdbTL_AppCallbacks_t *)NULL );
}

/*********************************************************************
 * @fn      bdbTL_HdlInSpecificCommands
 *
 * @brief   Callback from ZCL to process incoming Commands specific
 *          to this cluster library
 *
 * @param   pInMsg - pointer to the incoming message
 *
 * @return  ZStatus_t
 */
static ZStatus_t bdbTL_HdlInSpecificCommands( zclIncoming_t *pInMsg )
{
  ZStatus_t status;
  bdbTL_AppCallbacks_t *pCBs;

  // make sure endpoint exists
  pCBs = bdbTL_FindCallbacks( pInMsg->msg->endPoint );
  if ( pCBs == NULL )
    return ( ZFailure );

  switch ( pInMsg->msg->clusterId )
  {
    case ZCL_CLUSTER_ID_TOUCHLINK:
      status = bdbTL_ProcessInLLCmds( pInMsg, pCBs );
      break;

    default:
      status = ZFailure;
      break;
  }

  return ( status );
}

/*********************************************************************
 * @fn      bdbTL_ProcessInLLCmds
 *
 * @brief   Callback from ZCL to process incoming Commands specific
 *          to this cluster library on a command ID basis
 *
 * @param   pInMsg - pointer to the incoming message
 * @param   pCBs - pointer to callbacks
 *
 * @return  ZStatus_t
 */
static ZStatus_t bdbTL_ProcessInLLCmds( zclIncoming_t *pInMsg, bdbTL_AppCallbacks_t *pCBs )
{
  ZStatus_t status = SUCCESS;

  if ( zcl_ServerCmd( pInMsg->hdr.fc.direction ) )
  {
    switch ( pInMsg->hdr.commandID )
    {
      case COMMAND_TOUCHLINK_GET_GRP_IDS_REQ:
        status = bdbTL_ProcessInCmd_GetGrpIDsReq( pInMsg, pCBs );
        break;

      case COMMAND_TOUCHLINK_GET_EP_LIST_REQ:
        status = bdbTL_ProcessInCmd_GetEPListReq( pInMsg, pCBs );
        break;

      default:
        status = ZFailure;   // Error ignore the command
        break;
     }
  }
  else // Client Commands
  {
    switch ( pInMsg->hdr.commandID )
    {
      case COMMAND_TOUCHLINK_EP_INFO:
        status = bdbTL_ProcessInCmd_EndpointInfo( pInMsg, pCBs );
        break;

      case COMMAND_TOUCHLINK_GET_GRP_IDS_RSP:
        status = bdbTL_ProcessInCmd_GetGrpIDsRsp( pInMsg, pCBs );
        break;

      case COMMAND_TOUCHLINK_GET_EP_LIST_RSP:
        status = bdbTL_ProcessInCmd_GetEPListRsp( pInMsg, pCBs );
        break;

      default:
        status = ZFailure;   // Error ignore the command
        break;
    }
  }

  return ( status );
}


/*********************************************************************
 * @fn      bdbTL_ProcessInCmd_GetGrpIDsReq
 *
 * @brief   Process in the received Get Group Identifiers Request command.
 *
 * @param   pInMsg - pointer to the incoming message
 * @param   pCBs - pointer to the application callbacks
 *
 * @return  ZStatus_t
 */
static ZStatus_t bdbTL_ProcessInCmd_GetGrpIDsReq( zclIncoming_t *pInMsg,
                                                  bdbTL_AppCallbacks_t *pCBs )
{
  if ( pCBs->pfnGetGrpIDsReq )
  {
    bdbTLGetGrpIDsReq_t req;

    req.startIndex = pInMsg->pData[0];

    if ( pCBs->pfnGetGrpIDsReq( &(pInMsg->msg->srcAddr), &req, pInMsg->hdr.transSeqNum ) == ZSuccess )
    {
      return ( ZCL_STATUS_CMD_HAS_RSP );
    }
  }

  return ( ZFailure );
}

/*********************************************************************
 * @fn      bdbTL_ProcessInCmd_GetEPListReq
 *
 * @brief   Process in the received Get Endpoint List Request command.
 *
 * @param   pInMsg - pointer to the incoming message
 * @param   pCBs - pointer to the application callbacks
 *
 * @return  ZStatus_t
 */
static ZStatus_t bdbTL_ProcessInCmd_GetEPListReq( zclIncoming_t *pInMsg,
                                                  bdbTL_AppCallbacks_t *pCBs )
{
  if ( pCBs->pfnGetEPListReq )
  {
    bdbTLGetEPListReq_t req;

    req.startIndex = pInMsg->pData[0];

    if ( pCBs->pfnGetEPListReq( &(pInMsg->msg->srcAddr), &req, pInMsg->hdr.transSeqNum ) == ZSuccess )
    {
      return ( ZCL_STATUS_CMD_HAS_RSP );
    }
  }

  return ( ZFailure );
}


/*********************************************************************
 * @fn      bdbTL_ProcessInCmd_EndpointInfo
 *
 * @brief   Process in the received Endpoint Information command.
 *
 * @param   pInMsg - pointer to the incoming message
 * @param   pCBs - pointer to the application callbacks
 *
 * @return  ZStatus_t
 */
static ZStatus_t bdbTL_ProcessInCmd_EndpointInfo( zclIncoming_t *pInMsg,
                                                  bdbTL_AppCallbacks_t *pCBs )
{
  ZStatus_t status = ZFailure;

  if ( pCBs->pfnEndpointInfo )
  {
    bdbTLEndpointInfo_t *pCmd;

    pCmd = (bdbTLEndpointInfo_t *)zcl_mem_alloc( sizeof( bdbTLEndpointInfo_t ) );
    if ( pCmd )
    {
      uint8_t *pBuf = pInMsg->pData;

      zcl_cpyExtAddr( pCmd->ieeeAddr, pBuf );
      pBuf += Z_EXTADDR_LEN;

      pCmd->nwkAddr = BUILD_UINT16( pBuf[0], pBuf[1] );
      pBuf += 2;

      pCmd->endpoint = *pBuf++;

      pCmd->profileID = BUILD_UINT16( pBuf[0], pBuf[1] );
      pBuf += 2;

      pCmd->deviceID = BUILD_UINT16( pBuf[0], pBuf[1] );
      pBuf += 2;

      pCmd->version = *pBuf++;

      status = pCBs->pfnEndpointInfo( &(pInMsg->msg->srcAddr), pCmd );

      zcl_mem_free( pCmd );
    }
  }
  else
  {
    status = ZSuccess;
  }

  return ( status );
}

/*********************************************************************
 * @fn      bdbTL_ProcessInCmd_GetGrpIDsRsp
 *
 * @brief   Process in the received Get Group Identifiers Response command.
 *
 * @param   pInMsg - pointer to the incoming message
 * @param   pCBs - pointer to the application callbacks
 *
 * @return  ZStatus_t
 */
static ZStatus_t bdbTL_ProcessInCmd_GetGrpIDsRsp( zclIncoming_t *pInMsg,
                                                  bdbTL_AppCallbacks_t *pCBs )
{
  ZStatus_t status = ZFailure;

  if ( pCBs->pfnGetGrpIDsRsp )
  {
    bdbTLGetGrpIDsRsp_t *pRsp;
    uint8_t cnt = pInMsg->pData[TOUCHLINK_CMDLEN_GET_GRP_IDS_RSP-1];
    // total + startIndex + cnt
    uint16_t rspLen = sizeof( bdbTLGetGrpIDsRsp_t) + ( cnt * sizeof( grpInfoRec_t ) );
    uint16_t actualDataLen = pInMsg->pDataLen;

    if ( rspLen > actualDataLen )
    {
      return status;
    }

    pRsp = (bdbTLGetGrpIDsRsp_t *)zcl_mem_alloc( rspLen );
    if ( pRsp )
    {
      uint8_t *pBuf = pInMsg->pData;
      uint8_t i;

      pRsp->total = *pBuf++;
      pRsp->startIndex = *pBuf++;
      pRsp->cnt = *pBuf++;

      for ( i = 0; i < cnt; i++ )
      {
        grpInfoRec_t *pRec = &(pRsp->grpInfoRec[i]);

        pRec->grpID = BUILD_UINT16( pBuf[0], pBuf[1] );
        pBuf += 2;

        pRec->grpType = *pBuf++;
      }

      status = pCBs->pfnGetGrpIDsRsp( &(pInMsg->msg->srcAddr), pRsp );

      zcl_mem_free( pRsp );
    }
  }

  return ( status );
}

/*********************************************************************
 * @fn      bdbTL_ProcessInCmd_GetEPListRsp
 *
 * @brief   Process in the received Get Endpoint List Response command.
 *
 * @param   pInMsg - pointer to the incoming message
 * @param   pCBs - pointer to the application callbacks
 *
 * @return  ZStatus_t
 */
static ZStatus_t bdbTL_ProcessInCmd_GetEPListRsp( zclIncoming_t *pInMsg,
                                                  bdbTL_AppCallbacks_t *pCBs )
{
  ZStatus_t status = ZFailure;

  if ( pCBs->pfnGetEPListRsp )
  {
    bdbTLGetEPListRsp_t *pRsp;
    uint8_t cnt = pInMsg->pData[TOUCHLINK_CMDLEN_GET_EP_LIST_RSP-1];
    // total + startIndex + cnt
    uint16_t rspLen = sizeof( bdbTLGetEPListRsp_t ) + ( cnt * sizeof( epInfoRec_t ) );
    uint16_t actualDataLen = pInMsg->pDataLen;

    if ( rspLen > actualDataLen )
    {
      return status;
    }

    pRsp = (bdbTLGetEPListRsp_t *)zcl_mem_alloc( rspLen );
    if ( pRsp )
    {
      uint8_t *pBuf = pInMsg->pData;
      uint8_t i;

      pRsp->total = *pBuf++;
      pRsp->startIndex = *pBuf++;
      pRsp->cnt = *pBuf++;

      for ( i = 0; i < cnt; i++ )
      {
        epInfoRec_t *pRec = &(pRsp->epInfoRec[i]);

        pRec->nwkAddr = BUILD_UINT16( pBuf[0], pBuf[1] );
        pBuf += 2;

        pRec->endpoint = *pBuf++;

        pRec->profileID = BUILD_UINT16( pBuf[0], pBuf[1] );
        pBuf += 2;

        pRec->deviceID = BUILD_UINT16( pBuf[0], pBuf[1] );
        pBuf += 2;

        pRec->version = *pBuf++;
      }

      status = pCBs->pfnGetEPListRsp( &(pInMsg->msg->srcAddr), pRsp );

      zcl_mem_free( pRsp );
    }
  }

  return ( status );
}

/*********************************************************************
* Inter-PAN functions
**********************************************************************/

/*********************************************************************
 * @fn      bdbTL_HdlInInterPANCommands
 *
 * @brief   Callback from ZCL to process incoming Inter-PAN Commands
 *          specific to this cluster library

 * @param   pInMsg - pointer to the incoming message
 *
 * @return  ZStatus_t
 */
static ZStatus_t bdbTL_HdlInInterPANCommands( zclIncoming_t *pInMsg )
{
  ZStatus_t status;

  // make sure Inter-PAN callbacks exist
  if ( pInterPANCBs == NULL )
    return ( ZFailure );

  switch ( pInMsg->msg->clusterId )
  {
    case ZCL_CLUSTER_ID_TOUCHLINK:
      status = bdbTL_ProcessInLLInterPANCmds( pInMsg );
      break;

    default:
      status = ZFailure;
      break;
  }

  return ( status );
}

/*********************************************************************
 * @fn      bdbTL_ProcessInLLInterPANCmds
 *
 * @brief   Callback from ZCL to process incoming Inter-PAN Commands
 *          specific to this cluster library on a command ID basis
 *
 * @param   pInMsg - pointer to the incoming message
 *
 * @return  ZStatus_t
 */
static ZStatus_t bdbTL_ProcessInLLInterPANCmds( zclIncoming_t *pInMsg )
{
  ZStatus_t status = SUCCESS;

#if defined BDB_TL_TARGET
  if ( touchLinkTargetEnabled == FALSE )
  {
    return status;
  }
#endif

  switch ( pInMsg->hdr.commandID )
  {
    case COMMAND_TOUCHLINK_SCAN_REQ:
      status = bdbTL_ProcessInCmd_ScanReq( pInMsg );
      break;

    case COMMAND_TOUCHLINK_DEVICE_INFO_REQ:
      status = bdbTL_ProcessInCmd_DeviceInfoReq( pInMsg );
      break;

    case COMMAND_TOUCHLINK_IDENTIFY_REQ:
      status = bdbTL_ProcessInCmd_IdentifyReq( pInMsg );
      break;

    case COMMAND_TOUCHLINK_RESET_TO_FN_REQ:
      status = bdbTL_ProcessInCmd_ResetToFNReq( pInMsg );
      break;

    case COMMAND_TOUCHLINK_NWK_START_REQ:
      status = bdbTL_ProcessInCmd_NwkStartReq( pInMsg );
      break;

    case COMMAND_TOUCHLINK_NWK_JOIN_RTR_REQ:
      status = bdbTL_ProcessInCmd_NwkJoinRtrReq( pInMsg );
      break;

    case COMMAND_TOUCHLINK_NWK_JOIN_ED_REQ:
      status = bdbTL_ProcessInCmd_NwkJoinEDReq( pInMsg );
      break;

    case COMMAND_TOUCHLINK_NWK_UPDATE_REQ:
      status = bdbTL_ProcessInCmd_NwkUpdateReq( pInMsg );
      break;

    case COMMAND_TOUCHLINK_SCAN_RSP:
      status = bdbTL_ProcessInCmd_ScanRsp( pInMsg );
      break;

    case COMMAND_TOUCHLINK_DEVICE_INFO_RSP:
      status = bdbTL_ProcessInCmd_DeviceInfoRsp( pInMsg );
      break;

    case COMMAND_TOUCHLINK_NWK_START_RSP:
      status = bdbTL_ProcessInCmd_NwkStartRsp( pInMsg );
      break;

    case COMMAND_TOUCHLINK_NWK_JOIN_RTR_RSP:
      status = bdbTL_ProcessInCmd_NwkJoinRtrRsp( pInMsg );
      break;

    case COMMAND_TOUCHLINK_NWK_JOIN_ED_RSP:
      status = bdbTL_ProcessInCmd_NwkJoinEDRsp( pInMsg );
      break;

    default:
      status = ZFailure;   // Error ignore the command
      break;
  }

  return ( status );
}

/*********************************************************************
 * @fn      bdbTL_ProcessInCmd_ScanReq
 *
 * @brief   Process in the received Scan Request command.
 *
 * @param   pInMsg - pointer to the incoming message
 *
 * @return  ZStatus_t
 */
static ZStatus_t bdbTL_ProcessInCmd_ScanReq( zclIncoming_t *pInMsg )
{
  if ( pInterPANCBs->pfnScanReq )
  {
    bdbTLScanReq_t req;

    req.transID = zcl_build_uint32( pInMsg->pData, 4 );

    req.zInfo.zInfoByte = pInMsg->pData[4];
    req.touchLinkInfo.touchLinkInfoByte = pInMsg->pData[5];

    return ( pInterPANCBs->pfnScanReq( &(pInMsg->msg->srcAddr), &req,
                                       pInMsg->hdr.transSeqNum ) );
  }

  return ( ZSuccess );
}

/*********************************************************************
 * @fn      bdbTL_ProcessInCmd_DeviceInfoReq
 *
 * @brief   Process in the received Device Information Request command.
 *
 * @param   pInMsg - pointer to the incoming message
 *
 * @return  ZStatus_t
 */
static ZStatus_t bdbTL_ProcessInCmd_DeviceInfoReq( zclIncoming_t *pInMsg )
{
  if ( pInterPANCBs->pfnDeviceInfoReq )
  {
    bdbTLDeviceInfoReq_t req;

    req.transID = zcl_build_uint32( pInMsg->pData, 4 );

    req.startIndex = pInMsg->pData[4];

    return ( pInterPANCBs->pfnDeviceInfoReq( &(pInMsg->msg->srcAddr), &req,
                                             pInMsg->hdr.transSeqNum ) );
  }

  return ( ZSuccess );
}


/*********************************************************************
 * @fn      bdbTL_ProcessInCmd_IdentifyReq
 *
 * @brief   Process in the received Identify Request command.
 *
 * @param   pInMsg - pointer to the incoming message
 *
 * @return  ZStatus_t
 */
static ZStatus_t bdbTL_ProcessInCmd_IdentifyReq( zclIncoming_t *pInMsg )
{
  if ( pInterPANCBs->pfnIdentifyReq )
  {
    bdbTLIdentifyReq_t req;

    req.transID = zcl_build_uint32( pInMsg->pData, 4 );

    req.IdDuration = BUILD_UINT16( pInMsg->pData[4], pInMsg->pData[5] );

    return ( pInterPANCBs->pfnIdentifyReq( &(pInMsg->msg->srcAddr), &req ) );
  }

  return ( ZSuccess );
}

/*********************************************************************
 * @fn      bdbTL_ProcessInCmd_ResetToFNReq
 *
 * @brief   Process in the received Reset to Factory New Request command.
 *
 * @param   pInMsg - pointer to the incoming message
 *
 * @return  ZStatus_t
 */
static ZStatus_t bdbTL_ProcessInCmd_ResetToFNReq( zclIncoming_t *pInMsg )
{
  if ( pInterPANCBs->pfnResetToFNReq )
  {
    bdbTLResetToFNReq_t req;

    req.transID = zcl_build_uint32( pInMsg->pData, 4 );

    return ( pInterPANCBs->pfnResetToFNReq( &(pInMsg->msg->srcAddr), &req ) );
  }

  return ( ZSuccess );
}

/*********************************************************************
 * @fn      bdbTL_ProcessInCmd_NwkStartReq
 *
 * @brief   Process in the received Network Start Request command.
 *
 * @param   pInMsg - pointer to the incoming message
 *
 * @return  ZStatus_t
 */
static ZStatus_t bdbTL_ProcessInCmd_NwkStartReq( zclIncoming_t *pInMsg )
{
  uint8_t status = ZSuccess;

  if ( pInterPANCBs->pfnNwkStartReq )
  {
    bdbTLNwkStartReq_t *pReq;

    pReq = (bdbTLNwkStartReq_t *)zcl_mem_alloc( sizeof( bdbTLNwkStartReq_t ) );
    if ( pReq )
    {
      uint8_t *pBuf = pInMsg->pData;

      pReq->transID = zcl_build_uint32( pBuf, 4 );
      pBuf += 4;

      zcl_cpyExtAddr( pReq->nwkParams.extendedPANID, pBuf );
      pBuf += Z_EXTADDR_LEN;

      pReq->nwkParams.keyIndex = *pBuf++;

      zcl_memcpy( pReq->nwkParams.nwkKey, pBuf, SEC_KEY_LEN );
      pBuf += SEC_KEY_LEN;

      pReq->nwkParams.logicalChannel = *pBuf++;

      pReq->nwkParams.panId = BUILD_UINT16( pBuf[0], pBuf[1] );
      pBuf += 2;

      pReq->nwkParams.nwkAddr = BUILD_UINT16( pBuf[0], pBuf[1] );
      pBuf += 2;

      pReq->nwkParams.grpIDsBegin = BUILD_UINT16( pBuf[0], pBuf[1] );
      pBuf += 2;

      pReq->nwkParams.grpIDsEnd = BUILD_UINT16( pBuf[0], pBuf[1] );
      pBuf += 2;

      pReq->nwkParams.freeNwkAddrBegin = BUILD_UINT16( pBuf[0], pBuf[1] );
      pBuf += 2;

      pReq->nwkParams.freeNwkAddrEnd = BUILD_UINT16( pBuf[0], pBuf[1] );
      pBuf += 2;

      pReq->nwkParams.freeGrpIDBegin = BUILD_UINT16( pBuf[0], pBuf[1] );
      pBuf += 2;

      pReq->nwkParams.freeGrpIDEnd = BUILD_UINT16( pBuf[0], pBuf[1] );
      pBuf += 2;

      zcl_cpyExtAddr( pReq->initiatorIeeeAddr, pBuf );
      pBuf += Z_EXTADDR_LEN;

      pReq->initiatorNwkAddr = BUILD_UINT16( pBuf[0], pBuf[1] );

      status = pInterPANCBs->pfnNwkStartReq( &(pInMsg->msg->srcAddr), pReq,
                                             pInMsg->hdr.transSeqNum );

      zcl_mem_free( pReq );
    }
  }

  return ( status );
}

/*********************************************************************
 * @fn      bdbTL_ParseInCmd_NwkJoinReq
 *
 * @brief   Parse in the received Network Router/End Device Join Request command.
 *
 * @param   pBuf - pointer to the incoming message
 *
 * @return  void
 */
static void bdbTL_ParseInCmd_NwkJoinReq( uint8_t *pBuf, bdbTLNwkJoinReq_t *pReq )
{
  pReq->transID = zcl_build_uint32( pBuf, 4 );
  pBuf += 4;

  zcl_cpyExtAddr( pReq->nwkParams.extendedPANID, pBuf );
  pBuf += Z_EXTADDR_LEN;

  pReq->nwkParams.keyIndex = *pBuf++;

  zcl_memcpy( pReq->nwkParams.nwkKey, pBuf, SEC_KEY_LEN );
  pBuf += SEC_KEY_LEN;

  pReq->nwkUpdateId = *pBuf++;
  pReq->nwkParams.logicalChannel = *pBuf++;

  pReq->nwkParams.panId = BUILD_UINT16( pBuf[0], pBuf[1] );
  pBuf += 2;

  pReq->nwkParams.nwkAddr = BUILD_UINT16( pBuf[0], pBuf[1] );
  pBuf += 2;

  pReq->nwkParams.grpIDsBegin = BUILD_UINT16( pBuf[0], pBuf[1] );
  pBuf += 2;

  pReq->nwkParams.grpIDsEnd = BUILD_UINT16( pBuf[0], pBuf[1] );
  pBuf += 2;

  pReq->nwkParams.freeNwkAddrBegin = BUILD_UINT16( pBuf[0], pBuf[1] );
  pBuf += 2;

  pReq->nwkParams.freeNwkAddrEnd = BUILD_UINT16( pBuf[0], pBuf[1] );
  pBuf += 2;

  pReq->nwkParams.freeGrpIDBegin = BUILD_UINT16( pBuf[0], pBuf[1] );
  pBuf += 2;

  pReq->nwkParams.freeGrpIDEnd = BUILD_UINT16( pBuf[0], pBuf[1] );
}

/*********************************************************************
 * @fn      bdbTL_ProcessInCmd_NwkJoinRtrReq
 *
 * @brief   Process in the received Network Join Router Request command.
 *
 * @param   pInMsg - pointer to the incoming message
 *
 * @return  ZStatus_t
 */
static ZStatus_t bdbTL_ProcessInCmd_NwkJoinRtrReq( zclIncoming_t *pInMsg )
{
  ZStatus_t status = ZSuccess;

  if ( pInterPANCBs->pfnNwkJoinRtrReq )
  {
    bdbTLNwkJoinReq_t *pReq;

    pReq = (bdbTLNwkJoinReq_t *)zcl_mem_alloc( sizeof( bdbTLNwkJoinReq_t ) );
    if ( pReq )
    {
      bdbTL_ParseInCmd_NwkJoinReq( pInMsg->pData, pReq );

      status = pInterPANCBs->pfnNwkJoinRtrReq( &(pInMsg->msg->srcAddr), pReq,
                                               pInMsg->hdr.transSeqNum  );
      zcl_mem_free( pReq );
    }
  }

  return ( status );
}

/*********************************************************************
 * @fn      bdbTL_ProcessInCmd_NwkJoinEDReq
 *
 * @brief   Process in the received Network Join End Device  Request command.
 *
 * @param   pInMsg - pointer to the incoming message
 *
 * @return  ZStatus_t
 */
static ZStatus_t bdbTL_ProcessInCmd_NwkJoinEDReq( zclIncoming_t *pInMsg )
{
  ZStatus_t status = ZSuccess;

  if ( pInterPANCBs->pfnNwkJoinEDReq )
  {
    bdbTLNwkJoinReq_t *pReq;

    pReq = (bdbTLNwkJoinReq_t *)zcl_mem_alloc( sizeof( bdbTLNwkJoinReq_t ) );
    if ( pReq )
    {
      bdbTL_ParseInCmd_NwkJoinReq( pInMsg->pData, pReq );

      status = pInterPANCBs->pfnNwkJoinEDReq( &(pInMsg->msg->srcAddr), pReq,
                                              pInMsg->hdr.transSeqNum );
      zcl_mem_free( pReq );
    }
  }

  return ( status );
}

/*********************************************************************
 * @fn      bdbTL_ProcessInCmd_NwkUpdateReq
 *
 * @brief   Process in the received Network Update Request command.
 *
 * @param   pInMsg - pointer to the incoming message
 *
 * @return  ZStatus_t
 */
static ZStatus_t bdbTL_ProcessInCmd_NwkUpdateReq( zclIncoming_t *pInMsg )
{
  ZStatus_t status = ZSuccess;

  if ( pInterPANCBs->pfnNwkUpdateReq )
  {
    bdbTLNwkUpdateReq_t *pReq;

    pReq = (bdbTLNwkUpdateReq_t *)zcl_mem_alloc( sizeof( bdbTLNwkUpdateReq_t ) );
    if ( pReq )
    {
      uint8_t *pBuf = pInMsg->pData;

      pReq->transID = zcl_build_uint32( pBuf, 4 );
      pBuf += 4;

      zcl_cpyExtAddr( pReq->extendedPANID, pBuf );
      pBuf += Z_EXTADDR_LEN;

      pReq->nwkUpdateId = *pBuf++;
      pReq->logicalChannel = *pBuf++;

      pReq->PANID = BUILD_UINT16( pBuf[0], pBuf[1] );
      pBuf += 2;

      pReq->nwkAddr = BUILD_UINT16( pBuf[0], pBuf[1] );
      pBuf += 2;

      status = pInterPANCBs->pfnNwkUpdateReq( &(pInMsg->msg->srcAddr), pReq );

      zcl_mem_free( pReq );
    }
  }

  return ( status );
}

/*********************************************************************
 * @fn      bdbTL_ProcessInCmd_ScanRsp
 *
 * @brief   Process in the received Scan Response command.
 *
 * @param   pInMsg - pointer to the incoming message
 *
 * @return  ZStatus_t
 */
static ZStatus_t bdbTL_ProcessInCmd_ScanRsp( zclIncoming_t *pInMsg )
{
  ZStatus_t status = ZSuccess;

  if ( pInterPANCBs->pfnScanRsp )
  {
    bdbTLScanRsp_t *pRsp;

    pRsp = (bdbTLScanRsp_t *)zcl_mem_alloc( sizeof( bdbTLScanRsp_t ) );
    if ( pRsp )
    {
      uint8_t *pBuf = pInMsg->pData;

      zcl_memset( pRsp, 0, sizeof( bdbTLScanRsp_t ) );

      pRsp->transID = zcl_build_uint32( pBuf, 4 );
      pBuf += 4;

      pRsp->rssiCorrection = *pBuf++;
      pRsp->zInfo.zInfoByte = *pBuf++;
      pRsp->touchLinkInfo.touchLinkInfoByte = *pBuf++;

      pRsp->keyBitmask = BUILD_UINT16( pBuf[0], pBuf[1] );
      pBuf += 2;

      pRsp->responseID = zcl_build_uint32( pBuf, 4 );
      pBuf += 4;

      zcl_cpyExtAddr( pRsp->extendedPANID, pBuf );
      pBuf += Z_EXTADDR_LEN;

      pRsp->nwkUpdateId = *pBuf++;
      pRsp->logicalChannel = *pBuf++;

      pRsp->PANID = BUILD_UINT16( pBuf[0], pBuf[1] );
      pBuf += 2;

      pRsp->nwkAddr = BUILD_UINT16( pBuf[0], pBuf[1] );
      pBuf += 2;

      pRsp->numSubDevices = *pBuf++;
      pRsp->totalGrpIDs = *pBuf++;

      if ( pRsp->numSubDevices == 1 )
      {
        pRsp->deviceInfo.endpoint = *pBuf++;

        pRsp->deviceInfo.profileID = BUILD_UINT16( pBuf[0], pBuf[1] );
        pBuf += 2;

        pRsp->deviceInfo.deviceID = BUILD_UINT16( pBuf[0], pBuf[1] );
        pBuf += 2;

        pRsp->deviceInfo.version = *pBuf++;
        pRsp->deviceInfo.grpIdCnt = *pBuf;
      }

      status = pInterPANCBs->pfnScanRsp( &(pInMsg->msg->srcAddr), pRsp );
      zcl_mem_free( pRsp );
    }
  }

  return ( status );
}

/*********************************************************************
 * @fn      bdbTL_ProcessInCmd_DeviceInfoRsp
 *
 * @brief   Process in the received Device Information Response command.
 *
 * @param   pInMsg - pointer to the incoming message
 *
 * @return  ZStatus_t
 */
static ZStatus_t bdbTL_ProcessInCmd_DeviceInfoRsp( zclIncoming_t *pInMsg )
{
  ZStatus_t status = ZSuccess;

  if ( pInterPANCBs->pfnDeviceInfoRsp )
  {
    bdbTLDeviceInfoRsp_t *pRsp;
    uint8_t cnt = pInMsg->pData[TOUCHLINK_CMDLEN_DEVICE_INFO_RSP-1];
    // transID + numSubDevices + startIndex + cnt
    uint16_t rspLen = sizeof( bdbTLDeviceInfoRsp_t) + ( cnt * sizeof( devInfoRec_t ) );
    uint16_t actualDataLen = pInMsg->pDataLen;

    if ( rspLen > actualDataLen )
    {
      return status;
    }

    pRsp = (bdbTLDeviceInfoRsp_t *)zcl_mem_alloc( rspLen );
    if ( pRsp )
    {
      uint8_t *pBuf = pInMsg->pData;
      uint8_t i;

      pRsp->transID = zcl_build_uint32( pBuf, 4 );
      pBuf += 4;

      pRsp->numSubDevices = *pBuf++;
      pRsp->startIndex = *pBuf++;
      pRsp->cnt = *pBuf++;

      for ( i = 0; i < cnt; i++ )
      {
        devInfoRec_t *pRec = &(pRsp->devInfoRec[i]);

        zcl_cpyExtAddr( pRec->ieeeAddr, pBuf );
        pBuf += Z_EXTADDR_LEN;

        pRec->deviceInfo.endpoint = *pBuf++;

        pRec->deviceInfo.profileID = BUILD_UINT16( pBuf[0], pBuf[1] );
        pBuf += 2;

        pRec->deviceInfo.deviceID = BUILD_UINT16( pBuf[0], pBuf[1] );
        pBuf += 2;

        pRec->deviceInfo.version = *pBuf++;
        pRec->deviceInfo.grpIdCnt = *pBuf++;

        pRec->sort = *pBuf++;
      }

      status = pInterPANCBs->pfnDeviceInfoRsp( &(pInMsg->msg->srcAddr), pRsp );

      zcl_mem_free( pRsp );
    }
  }

  return ( status );
}

/*********************************************************************
 * @fn      bdbTL_ProcessInCmd_NwkStartRsp
 *
 * @brief   Process in the received Network Start Response command.
 *
 * @param   pInMsg - pointer to the incoming message
 *
 * @return  ZStatus_t
 */
static ZStatus_t bdbTL_ProcessInCmd_NwkStartRsp( zclIncoming_t *pInMsg )
{
  ZStatus_t status = ZSuccess;

  if ( pInterPANCBs->pfnNwkStartRsp )
  {
    bdbTLNwkStartRsp_t *pRsp;

    pRsp = (bdbTLNwkStartRsp_t *)zcl_mem_alloc( sizeof( bdbTLNwkStartRsp_t ) );
    if ( pRsp )
    {
      uint8_t *pBuf = pInMsg->pData;

      pRsp->transID = zcl_build_uint32( pBuf, 4 );
      pBuf += 4;

      pRsp->status = *pBuf++;

      zcl_cpyExtAddr( pRsp->extendedPANID, pBuf );
      pBuf += Z_EXTADDR_LEN;

      pRsp->nwkUpdateId = *pBuf++;
      pRsp->logicalChannel = *pBuf++;

      pRsp->panId = BUILD_UINT16( pBuf[0], pBuf[1] );

      status = pInterPANCBs->pfnNwkStartRsp( &(pInMsg->msg->srcAddr), pRsp );

      zcl_mem_free( pRsp );
    }
  }

  return ( status );
}

/*********************************************************************
 * @fn      bdbTL_ProcessInCmd_NwkJoinRtrRsp
 *
 * @brief   Process in the received Network Join Router Response command.
 *
 * @param   pInMsg - pointer to the incoming message
 *
 * @return  ZStatus_t
 */
static ZStatus_t bdbTL_ProcessInCmd_NwkJoinRtrRsp( zclIncoming_t *pInMsg )
{
  if ( pInterPANCBs->pfnNwkJoinRtrRsp )
  {
    bdbTLNwkJoinRsp_t rsp;

    rsp.transID = zcl_build_uint32( pInMsg->pData, 4 );

    rsp.status = pInMsg->pData[4];

    return ( pInterPANCBs->pfnNwkJoinRtrRsp( &(pInMsg->msg->srcAddr), &rsp ) );
  }

  return ( ZSuccess );
}


/*********************************************************************
 * @fn      bdbTL_ProcessInCmd_NwkJoinEDRsp
 *
 * @brief   Process in the received Network Join End Device Response command.
 *
 * @param   pInMsg - pointer to the incoming message
 *
 * @return  ZStatus_t
 */
static ZStatus_t bdbTL_ProcessInCmd_NwkJoinEDRsp( zclIncoming_t *pInMsg )
{
  if ( pInterPANCBs->pfnNwkJoinEDRsp )
  {
    bdbTLNwkJoinRsp_t rsp;

    rsp.transID = zcl_build_uint32( pInMsg->pData, 4 );

    rsp.status = pInMsg->pData[4];

    return ( pInterPANCBs->pfnNwkJoinEDRsp( &(pInMsg->msg->srcAddr), &rsp ) );
  }

  return ( ZSuccess );
}

#endif // BDB_TL_TARGET || BDB_TL_INITIATOR

/********************************************************************************************
*********************************************************************************************/
