/**************************************************************************************************
  Filename:       zcl_pi.c
  Revised:        $Date: 2013-06-11 13:53:09 -0700 (Tue, 11 Jun 2013) $
  Revision:       $Revision: 34523 $

  Description:    Zigbee Cluster Library - Protocol Interfaces (PI)


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
#include <zstack/common/zcl/zcl_general.h>
#include <zstack/common/zcl/zcl_pi.h>
#include "ti_zstack_config.h"

#if defined ( INTER_PAN ) || defined ( BDB_TL_INITIATOR ) || defined ( BDB_TL_TARGET )
  #include "stub_aps.h"
#endif

#ifdef ZCL_PI
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */
typedef struct zclPICBRec
{
  struct zclPICBRec     *next;
  uint8_t                 endpoint; // Used to link it into the endpoint descriptor
  zclPI_AppCallbacks_t  *CBs;     // Pointer to Callback function
} zclPICBRec_t;

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * GLOBAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
static zclPICBRec_t *zclPICBs = (zclPICBRec_t *)NULL;

static uint8_t zclPIPluginRegisted = FALSE;

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static ZStatus_t zclPI_HdlIncoming( zclIncoming_t *pInHdlrMsg );


static ZStatus_t zclPI_HdlInSpecificCommands( zclIncoming_t *pInMsg );
static zclPI_AppCallbacks_t *zclPI_FindCallbacks( uint8_t endpoint );


static ZStatus_t zclPI_ProcessIn_GenericTunneServer( zclIncoming_t *pInMsg,
                                                     zclPI_AppCallbacks_t *pCBs );
static ZStatus_t zclPI_ProcessIn_GenericTunneClient( zclIncoming_t *pInMsg,
                                                     zclPI_AppCallbacks_t *pCBs );
static ZStatus_t zclPI_ProcessIn_BACnetTunnelCmds( zclIncoming_t *pInMsg,
                                                   zclPI_AppCallbacks_t *pCBs );
static ZStatus_t zclPI_ProcessIn_7818TunnelCmds( zclIncoming_t *pInMsg,
                                                  zclPI_AppCallbacks_t *pCBs );
static ZStatus_t zclPI_ProcessIn_11073TunnelCmds( zclIncoming_t *pInMsg,
                                                  zclPI_AppCallbacks_t *pCBs );

/*********************************************************************
 * @fn      zclPI_RegisterCmdCallbacks
 *
 * @brief   Register an applications command callbacks
 *
 * @param   endpoint - application's endpoint
 * @param   callbacks - pointer to the callback record.
 *
 * @return  ZMemError if not able to allocate
 */
ZStatus_t zclPI_RegisterCmdCallbacks( uint8_t endpoint, zclPI_AppCallbacks_t *callbacks )
{
  zclPICBRec_t *pNewItem;
  zclPICBRec_t *pLoop;

  // Register as a ZCL Plugin
  if ( !zclPIPluginRegisted )
  {
    zcl_registerPlugin( ZCL_CLUSTER_ID_PI_GENERIC_TUNNEL,
                        ZCL_CLUSTER_ID_PI_ISO7818_PROTOCOL_TUNNEL,
                        zclPI_HdlIncoming );
    zclPIPluginRegisted = TRUE;
  }

  // Fill in the new profile list
  pNewItem = zcl_mem_alloc( sizeof( zclPICBRec_t ) );
  if ( pNewItem == NULL )
    return (ZMemError);

  pNewItem->next = (zclPICBRec_t *)NULL;
  pNewItem->endpoint = endpoint;
  pNewItem->CBs = callbacks;

  // Find spot in list
  if ( zclPICBs == NULL )
  {
    zclPICBs = pNewItem;
  }
  else
  {
    // Look for end of list
    pLoop = zclPICBs;
    while ( pLoop->next != NULL )
      pLoop = pLoop->next;

    // Put new item at end of list
    pLoop->next = pNewItem;
  }
  return ( ZSuccess );
}

/*******************************************************************************
 * @fn      zclPI_Send_MatchProtocolAddrCmd
 *
 * @brief   Call to send out a Match Protocol Address Command. This command is
 *          used when an associated protocol specific tunnel wishes to find out
 *          the ZigBee address of the Generic Tunnel server cluster representing
 *          a protocol-specific device with a given protocol address. The
 *          command is typically multicast to a group of inter-communicating
 *          Generic Tunnel clusters.
 *
 * @param   srcEP - Sending application's endpoint
 * @param   dstAddr - where you want the message to go
 * @param   len - length of protocol address
 * @param   protocolAddr - protocol address
 * @param   disableDefaultRsp - whether to disable the Default Response command
 * @param   seqNum - sequence number
 *
 * @return  ZStatus_t
 */
ZStatus_t zclPI_Send_MatchProtocolAddrCmd( uint8_t srcEP, afAddrType_t *dstAddr,
                                           uint8_t len, uint8_t *protocolAddr,
                                           uint8_t disableDefaultRsp, uint8_t seqNum )
{
  uint8_t *buf;
  ZStatus_t stat;

  buf = zcl_mem_alloc( len+1 );  // 1 for length field
  if ( buf )
  {
    buf[0] = len;
    zcl_memcpy( &(buf[1]), protocolAddr, len );

    stat = zcl_SendCommand( srcEP, dstAddr, ZCL_CLUSTER_ID_PI_GENERIC_TUNNEL,
                            COMMAND_PI_GENERIC_TUNNEL_MATCH_PROTOCOL_ADDR, TRUE,
                            ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNum,
                            (len+1), buf );
    zcl_mem_free( buf );
  }
  else
  {
    stat = ZMemError;
  }

  return ( stat );
}

/*******************************************************************************
 * @fn      zclPI_Send_MatchProtocolAddrRsp
 *
 * @brief   Call to send out a Match Protocol Address Response. This response
 *          is sent back upon receipt of a Match Protocol Address command to
 *          indicate that the Protocol Address was successfully matched.
 *
 * @param   srcEP - Sending application's endpoint
 * @param   dstAddr - where you want the message to go
 * @param   ieeeAddr - device address
 * @param   len - length of protocol address
 * @param   protocolAddr - protocol address
 * @param   disableDefaultRsp - whether to disable the Default Response command
 * @param   seqNum - sequence number
 *
 * @return  ZStatus_t
 */
ZStatus_t zclPI_Send_MatchProtocolAddrRsp( uint8_t srcEP, afAddrType_t *dstAddr,
                                           uint8_t *ieeeAddr, uint8_t len, uint8_t *protocolAddr,
                                           uint8_t disableDefaultRsp, uint8_t seqNum )
{
  uint8_t *buf;
  uint8_t msgLen = Z_EXTADDR_LEN + 1 + len; // IEEE Address + 1 for length field
  ZStatus_t stat;

  buf = zcl_mem_alloc( msgLen ); // 1 for length field
  if ( buf )
  {
    // Copy over IEEE Address
    zcl_cpyExtAddr( buf, ieeeAddr );

    // Copy over Protocol Address
    buf[8] = len;
    zcl_memcpy( &(buf[9]), protocolAddr, len );

    stat = zcl_SendCommand( srcEP, dstAddr, ZCL_CLUSTER_ID_PI_GENERIC_TUNNEL,
                            COMMAND_PI_GENERIC_TUNNEL_MATCH_PROTOCOL_ADDR_RSP, TRUE,
                            ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNum,
                            msgLen, buf );
    zcl_mem_free( buf );
  }
  else
  {
    stat = ZMemError;
  }

  return ( stat );
}

/*******************************************************************************
 * @fn      zclPI_Send_AdvertiseProtocolAddrCmd
 *
 * @brief   Call to send out an Advertise Protocol Address Command. This command
 *          is sent out typically upon startup or whenever the Protocol Address
 *          attribute changes. It is typically multicast to a group of inter-
 *          communicating Generic Tunnel clusters.
 *
 * @param   srcEP - Sending application's endpoint
 * @param   dstAddr - where you want the message to go
 * @param   len - length of protocol address
 * @param   protocolAddr - protocol address
 * @param   disableDefaultRsp - whether to disable the Default Response command
 * @param   seqNum - sequence number
 *
 * @return  ZStatus_t
 */
ZStatus_t zclPI_Send_AdvertiseProtocolAddrCmd( uint8_t srcEP, afAddrType_t *dstAddr,
                                               uint8_t len, uint8_t *protocolAddr,
                                               uint8_t disableDefaultRsp, uint8_t seqNum )
{
  uint8_t *buf;
  ZStatus_t stat;

  buf = zcl_mem_alloc( len+1 ); // 1 for length field
  if ( buf )
  {
    buf[0] = len;
    zcl_memcpy( &(buf[1]), protocolAddr, len );

    stat = zcl_SendCommand( srcEP, dstAddr, ZCL_CLUSTER_ID_PI_GENERIC_TUNNEL,
                            COMMAND_PI_GENERIC_TUNNEL_ADVERTISE_PROTOCOL_ADDR, TRUE,
                            ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNum,
                            (len+1), buf );
    zcl_mem_free( buf );
  }
  else
  {
    stat = ZMemError;
  }

  return ( stat );
}

/*******************************************************************************
*******************************************************************************/
/*******************************************************************************
 * @fn      zclPI_Send_7818TransferAPDUCmd
 *
 * @brief
 *
 * @param   srcEP - Sending application's endpoint
 * @param   dstAddr - where you want the message to go
 * @param   len - length of APDU
 * @param   apdu - APDU to be sent
 * @param   direction - ZCL_FRAME_SERVER_CLIENT_DIR or ZCL_FRAME_CLIENT_SERVER_DIR
 * @param   seqNum - sequence number
 *
 * @return  ZStatus_t
 */
ZStatus_t zclPI_Send_7818TransferAPDUCmd( uint8_t srcEP, afAddrType_t *dstAddr,
                                           uint16_t len, uint8_t *apdu, uint8_t direction, uint8_t seqNum )
{
  uint8_t *buf;
  ZStatus_t stat;

  buf = zcl_mem_alloc( len+1 ); // 2 for length field (long octet string)
  if ( buf )
  {
    buf[0] = LO_UINT16( len );
    buf[1] = HI_UINT16( len );
    zcl_memcpy( &(buf[2]), apdu, len );

    // This command shall always be transmitted with the Disable Default
    // Response bit in the ZCL frame control field set to 1.
    stat = zcl_SendCommand( srcEP, dstAddr, ZCL_CLUSTER_ID_PI_ISO7818_PROTOCOL_TUNNEL,
                            COMMAND_PI_7818_TUNNEL_TRANSFER_APDU, TRUE,
                            direction, TRUE, 0, seqNum, (len+1), buf );
    zcl_mem_free( buf );
  }
  else
  {
    stat = ZMemError;
  }

  return ( stat );
}
/*******************************************************************************
 * @fn      zclPI_Send_11073TransferAPDUCmd
 *
 * @brief   Call to send out an 11073 Transfer APDU Command. This command is
 *          used when an 11073 network layer wishes to transfer an 11073 APDU
 *          across a ZigBee tunnel to another 11073 network layer.
 *
 *          The most stringent reliability characteristic of a given transport
 *          technology is "Best" reliability. Note - For ZigBee, this corresponds
 *          to use of APS-ACKs.
 *
 *          The least stringent reliability characteristic of a given transport
 *          technology is "Good" reliability. Note - For ZigBee, this corresponds
 *          to no use of APS-ACKs.
 *
 *          Note: This command shall always be transmitted with the Disable Default
 *          Response bit in the ZCL frame control field set to 1.
 *
 * @param   srcEP - Sending application's endpoint
 * @param   dstAddr - where you want the message to go
 * @param   len - length of APDU
 * @param   apdu - APDU to be sent
 * @param   seqNum - sequence number
 *
 * @return  ZStatus_t
 */
ZStatus_t zclPI_Send_11073TransferAPDUCmd( uint8_t srcEP, afAddrType_t *dstAddr,
                                           uint16_t len, uint8_t *apdu, uint8_t seqNum )
{
  uint8_t *buf;
  ZStatus_t stat;

  buf = zcl_mem_alloc( len+2 ); // 2 for length field (long octet string)
  if ( buf )
  {
    buf[0] = LO_UINT16( len );
    buf[1] = HI_UINT16( len );
    zcl_memcpy( &(buf[2]), apdu, len );

    // This command shall always be transmitted with the Disable Default
    // Response bit in the ZCL frame control field set to 1.
    stat = zcl_SendCommand( srcEP, dstAddr, ZCL_CLUSTER_ID_PI_11073_PROTOCOL_TUNNEL,
                            COMMAND_PI_11073_TUNNEL_TRANSFER_APDU, TRUE,
                            ZCL_FRAME_CLIENT_SERVER_DIR, TRUE, 0, seqNum, (len+2), buf );
    zcl_mem_free( buf );
  }
  else
  {
    stat = ZMemError;
  }

  return ( stat );
}

/*******************************************************************************
 * @fn      zclPI_Send_11073ConnectReq
 *
 * @brief   Call to send out an 11073 Connect Request Command. This command
 *          is generated when a Data Management device wishes to connect to
 *          an 11073 agent device. This may be in response to receiving a
 *          connect status notification command from that agent device with
 *          the connect status field set to RECONNECT_REQUEST.
 *
 * @param   srcEP - Sending application's endpoint
 * @param   dstAddr - where you want the message to go
 * @param   connectCtrl - connect control
 * @param   idleTimeout - inactivity time (in minutes) which Data Management device
 *                        will wait w/o receiving any data before it disconnects
 * @param   managerAddr - IEEE address (64-bit) of Data Management device
 *                        transmitting this frame
 * @param   managerEP - source endpoint from which Data Management device is
                        transmitting this frame
 * @param   disableDefaultRsp - whether to disable the Default Response command
 * @param   seqNum - sequence number
 *
 * @return  ZStatus_t
 */
ZStatus_t zclPI_Send_11073ConnectReq( uint8_t srcEP, afAddrType_t *dstAddr,
                                      uint8_t connectCtrl, uint16_t idleTimeout,
                                      uint8_t *managerAddr, uint8_t managerEP,
                                      uint8_t disableDefaultRsp, uint8_t seqNum )
{
  uint8_t *buf;
  uint8_t msgLen = 1 + 2 + Z_EXTADDR_LEN + 1; // connect ctrl + idle timeout + IEEE Address + manager EP
  ZStatus_t stat;

  buf = zcl_mem_alloc( msgLen );
  if ( buf )
  {
    buf[0] = connectCtrl;
    buf[1] = LO_UINT16( idleTimeout );
    buf[2] = HI_UINT16( idleTimeout );
    zcl_memcpy( &(buf[3]), managerAddr, Z_EXTADDR_LEN );
    buf[11] = managerEP;

    stat = zcl_SendCommand( srcEP, dstAddr, ZCL_CLUSTER_ID_PI_11073_PROTOCOL_TUNNEL,
                            COMMAND_PI_11073_TUNNEL_CONNECT_REQ, TRUE,
                            ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNum,
                            msgLen, buf );
    zcl_mem_free( buf );
  }
  else
  {
    stat = ZMemError;
  }

  return ( stat );
}


/*********************************************************************
 * @fn      zclPI_FindCallbacks
 *
 * @brief   Find the callbacks for an endpoint
 *
 * @param   endpoint
 *
 * @return  pointer to the callbacks
 */
static zclPI_AppCallbacks_t *zclPI_FindCallbacks( uint8_t endpoint )
{
  zclPICBRec_t *pCBs;

  pCBs = zclPICBs;
  while ( pCBs )
  {
    if ( pCBs->endpoint == endpoint )
      return ( pCBs->CBs );
  }
  return ( (zclPI_AppCallbacks_t *)NULL );
}

/*********************************************************************
 * @fn      zclPI_HdlIncoming
 *
 * @brief   Callback from ZCL to process incoming Commands specific
 *          to this cluster library or Profile commands for attributes
 *          that aren't in the attribute list
 *
 * @param   pInMsg - pointer to the incoming message
 * @param   logicalClusterID
 *
 * @return  ZStatus_t
 */
static ZStatus_t zclPI_HdlIncoming( zclIncoming_t *pInMsg )
{
  ZStatus_t stat = ZSuccess;

#if defined ( INTER_PAN ) || defined ( BDB_TL_INITIATOR ) || defined ( BDB_TL_TARGET )
  if ( StubAPS_InterPan( pInMsg->msg->srcAddr.panId, pInMsg->msg->srcAddr.endPoint ) )
    return ( stat ); // Cluster not supported thru Inter-PAN
#endif

  if ( zcl_ClusterCmd( pInMsg->hdr.fc.type ) )
  {
    // Is this a manufacturer specific command?
    if ( pInMsg->hdr.fc.manuSpecific == 0 )
    {
      stat = zclPI_HdlInSpecificCommands( pInMsg );
    }
    else
    {
      // We don't support any manufacturer specific command -- ignore it.
      stat = ZFailure;
    }
  }
  else
  {
    // Handle all the normal (Read, Write...) commands
    stat = ZFailure;
  }

  return ( stat );
}

/*********************************************************************
 * @fn      zclPI_HdlInSpecificCommands
 *
 * @brief   Callback from ZCL to process incoming Commands specific
 *          to this cluster library
 *
 * @param   pInMsg - pointer to the incoming message
 *
 * @return  ZStatus_t
 */
static ZStatus_t zclPI_HdlInSpecificCommands( zclIncoming_t *pInMsg )
{
  ZStatus_t stat;
  zclPI_AppCallbacks_t *pCBs;

  // make sure endpoint exists
  pCBs = zclPI_FindCallbacks( pInMsg->msg->endPoint );
  if ( pCBs == NULL )
    return ( ZFailure );

  switch ( pInMsg->msg->clusterId )
  {
    case ZCL_CLUSTER_ID_PI_GENERIC_TUNNEL:
      if ( zcl_ServerCmd( pInMsg->hdr.fc.direction ) )
        stat = zclPI_ProcessIn_GenericTunneServer( pInMsg, pCBs );
      else
        stat = zclPI_ProcessIn_GenericTunneClient( pInMsg, pCBs );
      break;

    case ZCL_CLUSTER_ID_PI_BACNET_PROTOCOL_TUNNEL:
      stat = zclPI_ProcessIn_BACnetTunnelCmds( pInMsg, pCBs );
      break;

    case ZCL_CLUSTER_ID_PI_ISO7818_PROTOCOL_TUNNEL:
        stat = zclPI_ProcessIn_7818TunnelCmds( pInMsg, pCBs );
     break;

    case ZCL_CLUSTER_ID_PI_11073_PROTOCOL_TUNNEL:
      stat = zclPI_ProcessIn_11073TunnelCmds( pInMsg, pCBs );
      break;

    default:
      stat = ZFailure;
      break;
  }

  return ( stat );
}

/*********************************************************************
 * @fn      zclPI_ProcessIn_GenericTunneServer
 *
 * @brief   Callback from ZCL to process incoming Commands specific
 *          to this cluster library on a command ID basis
 *
 * @param   pInMsg - pointer to the incoming message
 *
 * @return  ZStatus_t
 */
static ZStatus_t zclPI_ProcessIn_GenericTunneServer( zclIncoming_t *pInMsg,
                                                     zclPI_AppCallbacks_t *pCBs )
{
  if  ( pInMsg->hdr.commandID != COMMAND_PI_GENERIC_TUNNEL_MATCH_PROTOCOL_ADDR )
    return (ZFailure);   // Error ignore the command

  if ( pCBs->pfnPI_MatchProtocolAddr )
  {
    zclPIMatchProtocolAddr_t cmd;

    cmd.srcAddr = &(pInMsg->msg->srcAddr);
    cmd.seqNum = pInMsg->hdr.transSeqNum;
    cmd.len = pInMsg->pData[0];
    cmd.protocolAddr = &(pInMsg->pData[1]);

    pCBs->pfnPI_MatchProtocolAddr( &cmd );
  }

  return ( ZSuccess );
}

/*********************************************************************
 * @fn      zclPI_ProcessIn_GenericTunneClient
 *
 * @brief   Callback from ZCL to process incoming Commands specific
 *          to this cluster library on a command ID basis
 *
 * @param   pInMsg - pointer to the incoming message
 *
 * @return  ZStatus_t
 */
static ZStatus_t zclPI_ProcessIn_GenericTunneClient( zclIncoming_t *pInMsg,
                                                     zclPI_AppCallbacks_t *pCBs )
{
  ZStatus_t stat = ZSuccess;

  switch ( pInMsg->hdr.commandID )
  {
    case COMMAND_PI_GENERIC_TUNNEL_MATCH_PROTOCOL_ADDR_RSP:
      if ( pCBs->pfnPI_MatchProtocolAddrRsp )
      {
        zclPIMatchProtocolAddrRsp_t cmd;

        cmd.srcAddr = &(pInMsg->msg->srcAddr);
        cmd.ieeeAddr = pInMsg->pData;
        cmd.len = pInMsg->pData[8];
        cmd.protocolAddr = &(pInMsg->pData[9]);

        pCBs->pfnPI_MatchProtocolAddrRsp( &cmd );
      }
      break;

    case COMMAND_PI_GENERIC_TUNNEL_ADVERTISE_PROTOCOL_ADDR:
      if ( pCBs->pfnPI_AdvertiseProtocolAddr )
      {
        zclPIAdvertiseProtocolAddr_t cmd;

        cmd.srcAddr = &(pInMsg->msg->srcAddr);
        cmd.len = pInMsg->pData[0];
        cmd.protocolAddr = &(pInMsg->pData[1]);

        pCBs->pfnPI_AdvertiseProtocolAddr( &cmd );
      }
      break;

    default:
      // Unknown command
      stat = ZFailure;
      break;
  }

  return ( stat );
}

/*********************************************************************
 * @fn      zclPI_ProcessIn_BACnetTunnelCmds
 *
 * @brief   Callback from ZCL to process incoming Commands specific
 *          to this cluster library on a command ID basis
 *
 * @param   pInMsg - pointer to the incoming message
 *
 * @return  ZStatus_t
 */
static ZStatus_t zclPI_ProcessIn_BACnetTunnelCmds( zclIncoming_t *pInMsg,
                                                   zclPI_AppCallbacks_t *pCBs )
{
  if  ( pInMsg->hdr.commandID != COMMAND_PI_BACNET_TUNNEL_TRANSFER_NPDU )
    return (ZFailure);   // Error ignore the command

  if ( pCBs->pfnPI_BACnetTransferNPDU )
  {
    zclBACnetTransferNPDU_t cmd;

    cmd.srcAddr = &(pInMsg->msg->srcAddr);
    cmd.len = pInMsg->pDataLen;
    cmd.npdu = pInMsg->pData;

    pCBs->pfnPI_BACnetTransferNPDU( &cmd );
  }

  return ( ZSuccess );
}

/*********************************************************************
 * @fn      zclPI_ProcessIn_7818TunnelCmds
 *
 * @brief   Callback from ZCL to process incoming Commands specific
 *          to this cluster library on a command ID basis
 *
 * @param   pInMsg - pointer to the incoming message
 *
 * @return  ZStatus_t
 */
static ZStatus_t zclPI_ProcessIn_7818TunnelCmds( zclIncoming_t *pInMsg,
                                                  zclPI_AppCallbacks_t *pCBs )
{
  ZStatus_t stat = ZSuccess;

  switch ( pInMsg->hdr.commandID )
  {
    case COMMAND_PI_7818_TUNNEL_TRANSFER_APDU:
      if ( pCBs->pfnPI_7818TransferAPDU )
      {
          zcl7818TransferAPDU_t cmd;

        cmd.srcAddr = &(pInMsg->msg->srcAddr);
        cmd.status = 0;

        pCBs->pfnPI_7818TransferAPDU( &cmd );
      }
      break;

    case COMMAND_PI_7818_TUNNEL_INSERT_SMART_CARD:
      if ( pCBs->pfnPI_7818InsertSmartCard )
      {
          zcl7818InsertSmartCard_t cmd;

        cmd.srcAddr = &(pInMsg->msg->srcAddr);

        pCBs->pfnPI_7818InsertSmartCard( &cmd );
      }
      break;

    case COMMAND_PI_7818_TUNNEL_EXTRACT_SMART_CARD:
      if ( pCBs->pfnPI_7818ExtractSmartCard )
      {
          zcl7818ExtractSmartCard_t cmd;

        cmd.srcAddr = &(pInMsg->msg->srcAddr);

        pCBs->pfnPI_7818ExtractSmartCard( &cmd );
      }
      break;

    default:
      // Unknown command
      stat = ZFailure;
      break;
  }

  return ( stat );
}

/*********************************************************************
 * @fn      zclPI_ProcessIn_11073TunnelCmds
 *
 * @brief   Callback from ZCL to process incoming Commands specific
 *          to this cluster library on a command ID basis
 *
 * @param   pInMsg - pointer to the incoming message
 *
 * @return  ZStatus_t
 */
static ZStatus_t zclPI_ProcessIn_11073TunnelCmds( zclIncoming_t *pInMsg,
                                                  zclPI_AppCallbacks_t *pCBs )
{
  ZStatus_t stat = ZSuccess;

  switch ( pInMsg->hdr.commandID )
  {
    case COMMAND_PI_11073_TUNNEL_TRANSFER_APDU:
      if ( pCBs->pfnPI_11073TransferAPDU )
      {
        zcl11073TransferAPDU_t cmd;

        cmd.srcAddr = &(pInMsg->msg->srcAddr);
        cmd.len = BUILD_UINT16( pInMsg->pData[0], pInMsg->pData[1] );
        cmd.apdu = &(pInMsg->pData[2]);

        pCBs->pfnPI_11073TransferAPDU( &cmd );
      }
      break;

    case COMMAND_PI_11073_TUNNEL_CONNECT_REQ:
      if ( pCBs->pfnPI_11073ConnectReq )
      {
        zcl11073ConnectReq_t cmd;

        cmd.srcAddr = &(pInMsg->msg->srcAddr);
        cmd.seqNum = pInMsg->hdr.transSeqNum;
        cmd.connectCtrl = pInMsg->pData[0];
        cmd.idleTimeout = BUILD_UINT16( pInMsg->pData[1], pInMsg->pData[2] );
        cmd.managerAddr = &(pInMsg->pData[3]);
        cmd.managerEP = pInMsg->pData[11];

        pCBs->pfnPI_11073ConnectReq( &cmd );
      }
      break;

    case COMMAND_PI_11073_TUNNEL_DISCONNECT_REQ:
      if ( pCBs->pfnPI_11073DisconnectReq )
      {
        zcl11073DisconnectReq_t cmd;

        cmd.srcAddr = &(pInMsg->msg->srcAddr);
        cmd.seqNum = pInMsg->hdr.transSeqNum;
        cmd.managerAddr = pInMsg->pData;

        pCBs->pfnPI_11073DisconnectReq( &cmd );
      }
      break;

    case COMMAND_PI_11073_TUNNEL_CONNECT_STATUS_NOTI:
      if ( pCBs->pfnPI_11073ConnectStatusNoti )
      {
        zcl11073ConnectStatusNoti_t cmd;

        cmd.srcAddr = &(pInMsg->msg->srcAddr);
        cmd.connectStatus = pInMsg->pData[0];

        pCBs->pfnPI_11073ConnectStatusNoti( &cmd );
      }
      break;

    default:
      // Unknown command
      stat = ZFailure;
      break;
  }

  return ( stat );
}


/****************************************************************************
****************************************************************************/

#endif // ZCL_PI

