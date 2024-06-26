/**************************************************************************************************
  Filename:       zcl_electrical_measurement.c
  Revised:        $Date: 2013-10-16 16:38:58 -0700 (Wed, 16 Oct 2013) $
  Revision:       $Revision: 35701 $

  Description:    Zigbee Cluster Library - Electrical Measurement


  Copyright (c) 2019, Texas Instruments Incorporated
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  *  Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

  *  Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

  *  Neither the name of Texas Instruments Incorporated nor the names of
      its contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include <zstack/common/zcl/zcl.h>
#include <zstack/common/zcl/zcl_electrical_measurement.h>
#include <zstack/common/zcl/zcl_general.h>
#include "zcomdef.h"
#include "ti_zstack_config.h"

#if defined ( INTER_PAN ) || defined ( BDB_TL_INITIATOR ) || defined ( BDB_TL_TARGET )
  #include "stub_aps.h"
#endif

#ifdef ZCL_ELECTRICAL_MEASUREMENT

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */
typedef struct zclElectricalMeasurementCBRec
{
  struct zclElectricalMeasurementCBRec *next;
  uint8_t endpoint; // Used to link it into the endpoint descriptor
  zclElectricalMeasurement_AppCallbacks_t *CBs;     // Pointer to Callback function
} zclElectricalMeasurementCBRec_t;

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * GLOBAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
static zclElectricalMeasurementCBRec_t *zclElectricalMeasurementCBs = (zclElectricalMeasurementCBRec_t *)NULL;
static uint8_t zclElectricalMeasurementPluginRegisted = FALSE;

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static ZStatus_t zclElectricalMeasurement_HdlIncoming( zclIncoming_t *pInHdlrMsg );
static ZStatus_t zclElectricalMeasurement_HdlInSpecificCommands( zclIncoming_t *pInMsg );
static zclElectricalMeasurement_AppCallbacks_t *zclElectricalMeasurement_FindCallbacks( uint8_t endpoint );
static ZStatus_t zclElectricalMeasurement_ProcessInCmds( zclIncoming_t *pInMsg, zclElectricalMeasurement_AppCallbacks_t *pCBs );

static ZStatus_t zclElectricalMeasurement_ProcessInCmd_GetProfileInfo( zclIncoming_t *pInMsg, zclElectricalMeasurement_AppCallbacks_t *pCBs );
static ZStatus_t zclElectricalMeasurement_ProcessInCmd_GetProfileInfoRsp( zclIncoming_t *pInMsg, zclElectricalMeasurement_AppCallbacks_t *pCBs );
static ZStatus_t zclElectricalMeasurement_ProcessInCmd_GetMeasurementProfile( zclIncoming_t *pInMsg, zclElectricalMeasurement_AppCallbacks_t *pCBs );
static ZStatus_t zclElectricalMeasurement_ProcessInCmd_GetMeasurementProfileRsp( zclIncoming_t *pInMsg, zclElectricalMeasurement_AppCallbacks_t *pCBs );

/*********************************************************************
 * @fn      zclElectricalMeasurement_RegisterCmdCallbacks
 *
 * @brief   Register applications command callbacks
 *
 * @param   endpoint - application's endpoint
 * @param   callbacks - pointer to the callback record.
 *
 * @return  ZMemError if not able to allocate
 */
ZStatus_t zclElectricalMeasurement_RegisterCmdCallbacks( uint8_t endpoint, zclElectricalMeasurement_AppCallbacks_t *callbacks )
{
  zclElectricalMeasurementCBRec_t *pNewItem;
  zclElectricalMeasurementCBRec_t *pLoop;

  // Register as a ZCL Plugin
  if ( zclElectricalMeasurementPluginRegisted == FALSE )
  {
    zcl_registerPlugin( ZCL_CLUSTER_ID_MS_ELECTRICAL_MEASUREMENT,
                        ZCL_CLUSTER_ID_MS_ELECTRICAL_MEASUREMENT,
                        zclElectricalMeasurement_HdlIncoming );
    zclElectricalMeasurementPluginRegisted = TRUE;
  }

  // Fill in the new profile list
  pNewItem = zcl_mem_alloc( sizeof( zclElectricalMeasurementCBRec_t ) );
  if ( pNewItem == NULL )
  {
    return ( ZMemError );
  }

  pNewItem->next = (zclElectricalMeasurementCBRec_t *)NULL;
  pNewItem->endpoint = endpoint;
  pNewItem->CBs = callbacks;

  // Find spot in list
  if ( zclElectricalMeasurementCBs == NULL )
  {
    zclElectricalMeasurementCBs = pNewItem;
  }
  else
  {
    // Look for end of list
    pLoop = zclElectricalMeasurementCBs;
    while ( pLoop->next != NULL )
      pLoop = pLoop->next;

    // Put new item at end of list
    pLoop->next = pNewItem;
  }

  return ( ZSuccess );
}

/*********************************************************************
 * @fn      zclElectricalMeasurement_Send_GetProfileInfo
 *
 * @brief   Call to send out Electrical Measurement Get Profile info command from ZED to ZR/ZC. The Rsp
 *          will indicate the parameters of the device's profile.
 *
 * @param   srcEP - Sending application's endpoint
 * @param   dstAddr - where you want the message to go
 * @param   disableDefaultRsp - whether to disable the Default Response command
 * @param   seqNum - sequence number
 *
 * @return  ZStatus_t
 */
ZStatus_t zclElectricalMeasurement_Send_GetProfileInfo( uint8_t srcEP, afAddrType_t *dstAddr,
                                                        uint8_t disableDefaultRsp, uint8_t seqNum )
{
  // no payload
  return zcl_SendCommand( srcEP, dstAddr, ZCL_CLUSTER_ID_MS_ELECTRICAL_MEASUREMENT,
                          COMMAND_ELECTRICAL_MEASUREMENT_GET_PROFILE_INFO, TRUE, ZCL_FRAME_CLIENT_SERVER_DIR,
                          disableDefaultRsp, 0, seqNum, 0, NULL);
}

/*********************************************************************
 * @fn      zclElectricalMeasurement_Send_GetProfileInfoRsp
 *
 * @brief   Call to send out Electrical Measurement Get Profile Info Response. This will tell
 *          the client the appropriate parameters of the measurement profile.
 *
 * @param   srcEP - Sending application's endpoint
 * @param   dstAddr - where you want the message to go
 * @param   pPayload:
 *          profileCount - total number of supported profiles
 *          profileIntervalPeriod - indicates the time frame of capture for profiling purposes
 *          maxNumberOfIntervals - maximum number of intervals allowed for the response
 *          pListOfAttributes - represents list of attributes being profiled
 * @param   disableDefaultRsp - whether to disable the Default Response command
 * @param   seqNum - sequence number
 *
 * @return  ZStatus_t
 */
ZStatus_t zclElectricalMeasurement_Send_GetProfileInfoRsp( uint8_t srcEP, afAddrType_t *dstAddr,
                                                           zclElectricalMeasurementGetProfileInfoRsp_t *pPayload,
                                                           uint8_t disableDefaultRsp, uint8_t seqNum )
{
  uint8_t i;
  uint8_t *pBuf;    // variable length payload
  uint8_t offset;
  uint16_t calculatedBufSize;
  ZStatus_t status;

  // get a buffer large enough to hold the whole packet
  calculatedBufSize = ( 3 + ( pPayload->numberOfAttributes * sizeof( uint16_t ) ) );  // size of fixed variables plus variable array

  pBuf = zcl_mem_alloc( calculatedBufSize );
  if ( !pBuf )
  {
    return ( ZMemError );  // no memory
  }

  pBuf[0] = pPayload->profileCount;
  pBuf[1] = pPayload->profileIntervalPeriod;
  pBuf[2] = pPayload->maxNumberOfIntervals;
  offset = 3;
  for ( i = 0; i < pPayload->numberOfAttributes; i++ )
  {
    pBuf[offset++] = LO_UINT16(pPayload->pListOfAttributes[i]);
    pBuf[offset++] = HI_UINT16(pPayload->pListOfAttributes[i]);
  }

  status = zcl_SendCommand( srcEP, dstAddr, ZCL_CLUSTER_ID_MS_ELECTRICAL_MEASUREMENT,
                            COMMAND_ELECTRICAL_MEASUREMENT_GET_PROFILE_INFO_RSP, TRUE,
                            ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNum, calculatedBufSize, pBuf );
  zcl_mem_free( pBuf );

  return ( status );
}

/*********************************************************************
 * @fn      zclElectricalMeasurement_Send_GetMeasurementProfile
 *
 * @brief   Call to send out Electrical Measurement Get Measurement Profile. This will
 *          ask the server for the appropriate parameters of the measurement profile.
 *
 * @param   srcEP - Sending application's endpoint
 * @param   dstAddr - where you want the message to go
 * @param   attributeID - the electricity measurement attribute being profiled
 * @param   startTime - selects the interval block from available interval blocks
 * @param   numberOfIntervals - represents the number of intervals being requested
 * @param   disableDefaultRsp - whether to disable the Default Response command
 * @param   seqNum - sequence number
 *
 * @return  ZStatus_t
 */
ZStatus_t zclElectricalMeasurement_Send_GetMeasurementProfile( uint8_t srcEP, afAddrType_t *dstAddr,
                                                               uint16_t attributeID, uint32_t startTime,
                                                               uint8_t numberOfIntervals,
                                                               uint8_t disableDefaultRsp, uint8_t seqNum )
{
  uint8_t buf[7];   // 7 byte payload

  buf[0] = LO_UINT16(attributeID);
  buf[1] = HI_UINT16(attributeID);
  buf[2] = BREAK_UINT32(startTime, 0);
  buf[3] = BREAK_UINT32(startTime, 1);
  buf[4] = BREAK_UINT32(startTime, 2);
  buf[5] = BREAK_UINT32(startTime, 3);
  buf[6] = numberOfIntervals;

  return zcl_SendCommand( srcEP, dstAddr, ZCL_CLUSTER_ID_MS_ELECTRICAL_MEASUREMENT,
                          COMMAND_ELECTRICAL_MEASUREMENT_GET_MEASUREMENT_PROFILE, TRUE,
                          ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNum, sizeof(buf), buf );
}

/*********************************************************************
 * @fn      zclElectricalMeasurement_Send_GetMeasurementProfileRsp
 *
 * @brief   Call to send out Electrical Measurement Get Measurement Profile Response. This will
 *          tell the client the appropriate parameters of the measurement profile.
 *
 * @param   srcEP - Sending application's endpoint
 * @param   dstAddr - where you want the message to go
 * @param   pPayload:
 *          startTime - represents the end time of the most chronologically recent interval being requested
 *          status - table status enumeration lists the valid values returned in status field
 *          profileIntervalPeriod - time frame used to capture parameter for profiling purposes
 *          numberOfIntervalsDelivered - number of intervals the device is returning
 *          attributeID - attribute that has been profiled by the application
 *          intervals   - array of intervals that depend on numberOfIntervalsDelivered, type based on attributeID
 * @param   disableDefaultRsp - whether to disable the Default Response command
 * @param   seqNum - sequence number
 *
 * @return  ZStatus_t
 */
ZStatus_t zclElectricalMeasurement_Send_GetMeasurementProfileRsp( uint8_t srcEP, afAddrType_t *dstAddr,
                                                                  zclElectricalMeasurementGetMeasurementProfileRsp_t *pPayload,
                                                                  uint8_t disableDefaultRsp, uint8_t seqNum )
{
  uint8_t i;
  uint8_t offset;
  uint8_t *pBuf;    // variable length payload
  uint8_t attrRtn;
  uint8_t calculatedAttrLen;
  uint16_t calculatedIntervalSize;
  uint16_t calculatedBufLen;
  ZStatus_t status;
  zclAttrRec_t attrRec;

  // determine if attribute ID is found per EP and cluster ID
  attrRtn = zclFindAttrRec( dstAddr->endPoint, ZCL_CLUSTER_ID_MS_ELECTRICAL_MEASUREMENT, pPayload->attributeID, &attrRec );

  if ( attrRtn == TRUE )
  {
    // if found, determine length of attribute based on given type
    calculatedAttrLen = zclGetDataTypeLength( attrRec.attr.dataType );
  }

  calculatedIntervalSize = calculatedAttrLen * pPayload->numberOfIntervalsDelivered;

  // get a buffer large enough to hold the whole packet, including size of variable array
  calculatedBufLen = ( 9 + calculatedIntervalSize );

  pBuf = zcl_mem_alloc( calculatedBufLen );
  if ( !pBuf )
  {
    return ( ZMemError );  // no memory, return failure
  }

  pBuf[0] = BREAK_UINT32(pPayload->startTime, 0);
  pBuf[1] = BREAK_UINT32(pPayload->startTime, 1);
  pBuf[2] = BREAK_UINT32(pPayload->startTime, 2);
  pBuf[3] = BREAK_UINT32(pPayload->startTime, 3);
  pBuf[4] = pPayload->status;
  pBuf[5] = pPayload->profileIntervalPeriod;
  pBuf[6] = pPayload->numberOfIntervalsDelivered;
  pBuf[7] = LO_UINT16(pPayload->attributeID);
  pBuf[8] = HI_UINT16(pPayload->attributeID);
  offset = 9;
  for ( i = 0; i < calculatedIntervalSize; i++ )
  {
    pBuf[offset++] = pPayload->pIntervals[i];
  }

  status = zcl_SendCommand( srcEP, dstAddr, ZCL_CLUSTER_ID_MS_ELECTRICAL_MEASUREMENT,
                            COMMAND_ELECTRICAL_MEASUREMENT_GET_MEASUREMENT_PROFILE_RSP, TRUE,
                            ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNum, calculatedBufLen, pBuf );
  zcl_mem_free( pBuf );

  return ( status );
}

/*********************************************************************
 * @fn      zclElectricalMeasurement_FindCallbacks
 *
 * @brief   Find the callbacks for an endpoint
 *
 * @param   endpoint - endpoint to find the application callbacks for
 *
 * @return  pointer to the callbacks
 */
static zclElectricalMeasurement_AppCallbacks_t *zclElectricalMeasurement_FindCallbacks( uint8_t endpoint )
{
  zclElectricalMeasurementCBRec_t *pCBs;

  pCBs = zclElectricalMeasurementCBs;
  while ( pCBs != NULL )
  {
    if ( pCBs->endpoint == endpoint )
    {
      return ( pCBs->CBs );
    }
    pCBs = pCBs->next;
  }
  return ( (zclElectricalMeasurement_AppCallbacks_t *)NULL );
}

/*********************************************************************
 * @fn      zclElectricalMeasurement_HdlIncoming
 *
 * @brief   Callback from ZCL to process incoming Commands specific
 *          to this cluster library or Profile commands for attributes
 *          that aren't in the attribute list
 *
 * @param   pInMsg - pointer to the incoming message
 *
 * @return  ZStatus_t
 */
static ZStatus_t zclElectricalMeasurement_HdlIncoming( zclIncoming_t *pInMsg )
{
  ZStatus_t stat = ZSuccess;

#if defined ( INTER_PAN ) || defined ( BDB_TL_INITIATOR ) || defined ( BDB_TL_TARGET )
  if ( StubAPS_InterPan( pInMsg->msg->srcAddr.panId, pInMsg->msg->srcAddr.endPoint ) )
  {
    return ( stat ); // Cluster not supported thru Inter-PAN
  }
#endif
  if ( zcl_ClusterCmd( pInMsg->hdr.fc.type ) )
  {
    // Is this a manufacturer specific command?
    if ( pInMsg->hdr.fc.manuSpecific == 0 )
    {
      stat = zclElectricalMeasurement_HdlInSpecificCommands( pInMsg );
    }
    else
    {
      // We don't support any manufacturer specific command.
      stat = ZFailure;
    }
  }
  else
  {
    // Handle all the normal (Read, Write...) commands -- should never get here
    stat = ZFailure;
  }
  return ( stat );
}

/*********************************************************************
 * @fn      zclElectricalMeasurement_HdlInSpecificCommands
 *
 * @brief   Callback from ZCL to process incoming Commands specific
 *          to this cluster library

 * @param   pInMsg - pointer to the incoming message
 *
 * @return  ZStatus_t
 */
static ZStatus_t zclElectricalMeasurement_HdlInSpecificCommands( zclIncoming_t *pInMsg )
{
  ZStatus_t stat = ZSuccess;
  zclElectricalMeasurement_AppCallbacks_t *pCBs;

  // make sure endpoint exists
  pCBs = zclElectricalMeasurement_FindCallbacks( pInMsg->msg->endPoint );
  if ( pCBs == NULL )
  {
    return ( ZFailure );
  }

  stat = zclElectricalMeasurement_ProcessInCmds( pInMsg, pCBs );

  return ( stat );
}

/*********************************************************************
 * @fn      zclElectricalMeasurement_ProcessInCmds
 *
 * @brief   Callback from ZCL to process incoming Commands specific
 *          to this cluster library on a command ID basis

 * @param   pInMsg - pointer to the incoming message
 * @param   pCBs - pointer to the application callbacks
 *
 * @return  ZStatus_t
 */
static ZStatus_t zclElectricalMeasurement_ProcessInCmds( zclIncoming_t *pInMsg,
                                                         zclElectricalMeasurement_AppCallbacks_t *pCBs )
{
  ZStatus_t stat;

  // Client-to-Server
  if ( zcl_ServerCmd( pInMsg->hdr.fc.direction ) )
  {
    switch ( pInMsg->hdr.commandID )
    {
      case COMMAND_ELECTRICAL_MEASUREMENT_GET_PROFILE_INFO:
        stat = zclElectricalMeasurement_ProcessInCmd_GetProfileInfo( pInMsg, pCBs );
        break;

      case COMMAND_ELECTRICAL_MEASUREMENT_GET_MEASUREMENT_PROFILE:
        stat = zclElectricalMeasurement_ProcessInCmd_GetMeasurementProfile( pInMsg, pCBs );
        break;

      default:
        // Unknown command
        stat = ZFailure;
        break;
    }
  }
  // Server-to-Client
  else
  {
    switch ( pInMsg->hdr.commandID )
    {
      case COMMAND_ELECTRICAL_MEASUREMENT_GET_PROFILE_INFO_RSP:
        stat = zclElectricalMeasurement_ProcessInCmd_GetProfileInfoRsp( pInMsg, pCBs );
        break;

      case COMMAND_ELECTRICAL_MEASUREMENT_GET_MEASUREMENT_PROFILE_RSP:
        stat = zclElectricalMeasurement_ProcessInCmd_GetMeasurementProfileRsp( pInMsg, pCBs );
        break;

      default:
        // Unknown command
        stat = ZFailure;
        break;
    }
  }

  return ( stat );
}

/*********************************************************************
 * @fn      zclElectricalMeasurement_ProcessInCmd_GetProfileInfoRsp
 *
 * @brief   Process in the received Electrical Measurement Get Profile Info Response cmd
 *
 * @param   pInMsg - pointer to the incoming message
 * @param   pCBs - pointer to the application callbacks
 *
 * @return  ZStatus_t
 */
static ZStatus_t zclElectricalMeasurement_ProcessInCmd_GetProfileInfoRsp( zclIncoming_t *pInMsg,
                                                                          zclElectricalMeasurement_AppCallbacks_t *pCBs )
{
  uint8_t i;
  uint8_t offset;
  uint16_t calculatedArraySize;
  zclElectricalMeasurementGetProfileInfoRsp_t cmd;
  ZStatus_t status;

  if ( pCBs->pfnElectricalMeasurement_GetProfileInfoRsp )
  {
    // calculate size of variable array
    calculatedArraySize = pInMsg->pDataLen - 3;  // variable array - 3 bytes of fixed variables

    cmd.pListOfAttributes = zcl_mem_alloc( calculatedArraySize );
    if ( !cmd.pListOfAttributes )
    {
      return ( ZMemError );  // no memory, return failure
    }

    cmd.profileCount = pInMsg->pData[0];
    cmd.profileIntervalPeriod = pInMsg->pData[1];
    cmd.maxNumberOfIntervals = pInMsg->pData[2];
    cmd.numberOfAttributes = calculatedArraySize / sizeof( uint16_t );
    offset = 3;
    for ( i = 0; i < cmd.numberOfAttributes; i++ )
    {
      cmd.pListOfAttributes[i] = BUILD_UINT16( pInMsg->pData[offset], pInMsg->pData[offset + 1] );
      offset += 2;
    }

    status = ( pCBs->pfnElectricalMeasurement_GetProfileInfoRsp( &cmd ) );
    zcl_mem_free( cmd.pListOfAttributes );
    return status;
  }

  return ( ZFailure );
}

/*********************************************************************
 * @fn      zclElectricalMeasurement_ProcessInCmd_GetMeasurementProfileRsp
 *
 * @brief   Process in the received Electrical Measurement Get Measurement Profile Response cmd
 *
 * @param   pInMsg - pointer to the incoming message
 * @param   pCBs - pointer to the application callbacks
 *
 * @return  ZStatus_t
 */
static ZStatus_t zclElectricalMeasurement_ProcessInCmd_GetMeasurementProfileRsp( zclIncoming_t *pInMsg,
                                                                                 zclElectricalMeasurement_AppCallbacks_t *pCBs )
{
  uint8_t i;
  uint8_t offset;
  uint16_t calculatedIntervalSize;
  zclElectricalMeasurementGetMeasurementProfileRsp_t cmd;
  ZStatus_t status;

  if ( pCBs->pfnElectricalMeasurement_GetMeasurementProfileRsp )
  {
    // determine size of intervals by subtracting size of startTime, status,
    // profileIntervalPeriod, numberOfIntervalsDelivered, and attributeID from message length
    calculatedIntervalSize = pInMsg->pDataLen - 9;

    cmd.pIntervals = zcl_mem_alloc( calculatedIntervalSize );
    if ( !cmd.pIntervals )
    {
      return ( ZMemError );  // no memory, return failure
    }

    cmd.startTime = BUILD_UINT32( pInMsg->pData[0], pInMsg->pData[1], pInMsg->pData[2], pInMsg->pData[3] );
    cmd.status = pInMsg->pData[4];
    cmd.profileIntervalPeriod = pInMsg->pData[5];
    cmd.numberOfIntervalsDelivered = pInMsg->pData[6];
    cmd.attributeID = BUILD_UINT16( pInMsg->pData[7], pInMsg->pData[8] );
    offset = 9;
    for ( i = 0; i < calculatedIntervalSize; i++ )
    {
      cmd.pIntervals[i] = pInMsg->pData[offset++];
    }

    status = ( pCBs->pfnElectricalMeasurement_GetMeasurementProfileRsp( &cmd ) );
    zcl_mem_free( cmd.pIntervals );
    return status;
  }

  return ( ZFailure );
}

/*********************************************************************
 * @fn      zclElectricalMeasurement_ProcessInCmd_GetProfileInfo
 *
 * @brief   Process in the received Electrical Measurement Get Profile Info cmd
 *
 * @param   pInMsg - pointer to the incoming message
 * @param   pCBs - pointer to the application callbacks
 *
 * @return  ZStatus_t
 */
static ZStatus_t zclElectricalMeasurement_ProcessInCmd_GetProfileInfo( zclIncoming_t *pInMsg,
                                                                       zclElectricalMeasurement_AppCallbacks_t *pCBs )
{
  if ( pCBs->pfnElectricalMeasurement_GetProfileInfo )
  {
    return ( pCBs->pfnElectricalMeasurement_GetProfileInfo( ) );
  }

  return ( ZFailure );
}

/*********************************************************************
 * @fn      zclElectricalMeasurement_ProcessInCmd_GetMeasurementProfile
 *
 * @brief   Process in the received Electrical Measurement Get Measurement Profile cmd
 *
 * @param   pInMsg - pointer to the incoming message
 * @param   pCBs - pointer to the application callbacks
 *
 * @return  ZStatus_t
 */
static ZStatus_t zclElectricalMeasurement_ProcessInCmd_GetMeasurementProfile( zclIncoming_t *pInMsg,
                                                                              zclElectricalMeasurement_AppCallbacks_t *pCBs )
{
  if ( pCBs->pfnElectricalMeasurement_GetMeasurementProfile )
  {
    zclElectricalMeasurementGetMeasurementProfile_t cmd;

    cmd.attributeID = BUILD_UINT16( pInMsg->pData[0], pInMsg->pData[1] );
    cmd.startTime = BUILD_UINT32(pInMsg->pData[2], pInMsg->pData[3], pInMsg->pData[4], pInMsg->pData[5]);
    cmd.numberOfIntervals = pInMsg->pData[6];

    return ( pCBs->pfnElectricalMeasurement_GetMeasurementProfile( &cmd ) );
  }

  return ( ZFailure );
}

/****************************************************************************
****************************************************************************/

#endif // ZCL_ELECTRICAL_MEASUREMENT
