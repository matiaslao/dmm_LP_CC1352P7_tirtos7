/*
 *  ======== ti_radio_config.c ========
 *  Configured RadioConfig module definitions
 *
 *  DO NOT EDIT - This file is generated for the CC1352P7RGZ
 *  by the SysConfig tool.
 *
 *  Radio Config module version : 1.14
 *  SmartRF Studio data version : 2.27.0
 */

#include "ti_radio_config.h"
#include DeviceFamily_constructPath(rf_patches/rf_patch_cpe_ieee_802_15_4.h)
#include DeviceFamily_constructPath(rf_patches/rf_patch_cpe_prop.h)


// *********************************************************************************
//   RF Frontend configuration
// *********************************************************************************
// RF design based on: LP_CC1352P7-1

// TX Power tables
// The RF_TxPowerTable_DEFAULT_PA_ENTRY and RF_TxPowerTable_HIGH_PA_ENTRY macros are defined in RF.h.
// The following arguments are required:
// RF_TxPowerTable_DEFAULT_PA_ENTRY(bias, gain, boost, coefficient)
// RF_TxPowerTable_HIGH_PA_ENTRY(bias, ibboost, boost, coefficient, ldoTrim)
// See the Technical Reference Manual for further details about the "txPower" Command field.
// The PA settings require the CCFG_FORCE_VDDR_HH = 0 unless stated otherwise.

// 2400 MHz, 5 dBm
RF_TxPowerTable_Entry txPowerTable_2400_pa5[TXPOWERTABLE_2400_PA5_SIZE] =
{
    {-20, RF_TxPowerTable_DEFAULT_PA_ENTRY(8, 3, 0, 2) }, // 0x04C8
    {-18, RF_TxPowerTable_DEFAULT_PA_ENTRY(10, 3, 0, 2) }, // 0x04CA
    {-15, RF_TxPowerTable_DEFAULT_PA_ENTRY(13, 3, 0, 3) }, // 0x06CD
    {-12, RF_TxPowerTable_DEFAULT_PA_ENTRY(16, 3, 0, 5) }, // 0x0AD0
    {-10, RF_TxPowerTable_DEFAULT_PA_ENTRY(19, 3, 0, 5) }, // 0x0AD3
    {-9, RF_TxPowerTable_DEFAULT_PA_ENTRY(20, 3, 0, 6) }, // 0x0CD4
    {-6, RF_TxPowerTable_DEFAULT_PA_ENTRY(19, 2, 0, 11) }, // 0x1693
    {-5, RF_TxPowerTable_DEFAULT_PA_ENTRY(21, 2, 0, 11) }, // 0x1695
    {-3, RF_TxPowerTable_DEFAULT_PA_ENTRY(25, 2, 0, 12) }, // 0x1899
    {0, RF_TxPowerTable_DEFAULT_PA_ENTRY(29, 1, 0, 22) }, // 0x2C5D
    {1, RF_TxPowerTable_DEFAULT_PA_ENTRY(33, 1, 0, 25) }, // 0x3261
    {2, RF_TxPowerTable_DEFAULT_PA_ENTRY(38, 1, 0, 31) }, // 0x3E66
    {3, RF_TxPowerTable_DEFAULT_PA_ENTRY(47, 1, 0, 36) }, // 0x486F
    {4, RF_TxPowerTable_DEFAULT_PA_ENTRY(32, 0, 0, 65) }, // 0x8220
    {5, RF_TxPowerTable_DEFAULT_PA_ENTRY(46, 0, 0, 59) }, // 0x762E
    RF_TxPowerTable_TERMINATION_ENTRY
};


// 868 MHz, 13 dBm
RF_TxPowerTable_Entry txPowerTable_868_pa13[TXPOWERTABLE_868_PA13_SIZE] =
{
    {-20, RF_TxPowerTable_DEFAULT_PA_ENTRY(0, 3, 0, 2) }, // 0x04C0
    {-15, RF_TxPowerTable_DEFAULT_PA_ENTRY(1, 3, 0, 3) }, // 0x06C1
    {-10, RF_TxPowerTable_DEFAULT_PA_ENTRY(2, 3, 0, 5) }, // 0x0AC2
    {-5, RF_TxPowerTable_DEFAULT_PA_ENTRY(4, 3, 0, 5) }, // 0x0AC4
    {0, RF_TxPowerTable_DEFAULT_PA_ENTRY(8, 3, 0, 8) }, // 0x10C8
    {1, RF_TxPowerTable_DEFAULT_PA_ENTRY(9, 3, 0, 9) }, // 0x12C9
    {2, RF_TxPowerTable_DEFAULT_PA_ENTRY(10, 3, 0, 9) }, // 0x12CA
    {3, RF_TxPowerTable_DEFAULT_PA_ENTRY(11, 3, 0, 10) }, // 0x14CB
    {4, RF_TxPowerTable_DEFAULT_PA_ENTRY(13, 3, 0, 11) }, // 0x16CD
    {5, RF_TxPowerTable_DEFAULT_PA_ENTRY(14, 3, 0, 14) }, // 0x1CCE
    {6, RF_TxPowerTable_DEFAULT_PA_ENTRY(17, 3, 0, 16) }, // 0x20D1
    {7, RF_TxPowerTable_DEFAULT_PA_ENTRY(20, 3, 0, 19) }, // 0x26D4
    {8, RF_TxPowerTable_DEFAULT_PA_ENTRY(24, 3, 0, 22) }, // 0x2CD8
    {9, RF_TxPowerTable_DEFAULT_PA_ENTRY(28, 3, 0, 31) }, // 0x3EDC
    {10, RF_TxPowerTable_DEFAULT_PA_ENTRY(18, 2, 0, 31) }, // 0x3E92
    {11, RF_TxPowerTable_DEFAULT_PA_ENTRY(26, 2, 0, 51) }, // 0x669A
    {12, RF_TxPowerTable_DEFAULT_PA_ENTRY(30, 1, 1, 68) }, // 0x895E
    // The original PA value (12.5 dBm) has been rounded to an integer value.
    {13, RF_TxPowerTable_DEFAULT_PA_ENTRY(36, 0, 0, 89) }, // 0xB224
    // This setting requires CCFG_FORCE_VDDR_HH = 1.
    {14, RF_TxPowerTable_DEFAULT_PA_ENTRY(63, 0, 1, 0) }, // 0x013F
    RF_TxPowerTable_TERMINATION_ENTRY
};

// 868 MHz, 13 + 20 dBm
RF_TxPowerTable_Entry txPowerTable_868_pa13_20[TXPOWERTABLE_868_PA13_20_SIZE] =
{
    {-20, RF_TxPowerTable_DEFAULT_PA_ENTRY(0, 3, 0, 2) }, // 0x04C0
    {-15, RF_TxPowerTable_DEFAULT_PA_ENTRY(1, 3, 0, 3) }, // 0x06C1
    {-10, RF_TxPowerTable_DEFAULT_PA_ENTRY(2, 3, 0, 5) }, // 0x0AC2
    {-5, RF_TxPowerTable_DEFAULT_PA_ENTRY(4, 3, 0, 5) }, // 0x0AC4
    {0, RF_TxPowerTable_DEFAULT_PA_ENTRY(8, 3, 0, 8) }, // 0x10C8
    {1, RF_TxPowerTable_DEFAULT_PA_ENTRY(9, 3, 0, 9) }, // 0x12C9
    {2, RF_TxPowerTable_DEFAULT_PA_ENTRY(10, 3, 0, 9) }, // 0x12CA
    {3, RF_TxPowerTable_DEFAULT_PA_ENTRY(11, 3, 0, 10) }, // 0x14CB
    {4, RF_TxPowerTable_DEFAULT_PA_ENTRY(13, 3, 0, 11) }, // 0x16CD
    {5, RF_TxPowerTable_DEFAULT_PA_ENTRY(14, 3, 0, 14) }, // 0x1CCE
    {6, RF_TxPowerTable_DEFAULT_PA_ENTRY(17, 3, 0, 16) }, // 0x20D1
    {7, RF_TxPowerTable_DEFAULT_PA_ENTRY(20, 3, 0, 19) }, // 0x26D4
    {8, RF_TxPowerTable_DEFAULT_PA_ENTRY(24, 3, 0, 22) }, // 0x2CD8
    {9, RF_TxPowerTable_DEFAULT_PA_ENTRY(28, 3, 0, 31) }, // 0x3EDC
    {10, RF_TxPowerTable_DEFAULT_PA_ENTRY(18, 2, 0, 31) }, // 0x3E92
    {11, RF_TxPowerTable_DEFAULT_PA_ENTRY(26, 2, 0, 51) }, // 0x669A
    {12, RF_TxPowerTable_DEFAULT_PA_ENTRY(30, 1, 1, 68) }, // 0x895E
    {14, RF_TxPowerTable_DEFAULT_PA_ENTRY(63, 0, 1, 0) }, // 0x013F
    {15, RF_TxPowerTable_HIGH_PA_ENTRY(18, 0, 0, 36, 0) }, // 0x004812
    {16, RF_TxPowerTable_HIGH_PA_ENTRY(24, 0, 0, 43, 0) }, // 0x005618
    {17, RF_TxPowerTable_HIGH_PA_ENTRY(28, 0, 0, 51, 2) }, // 0x02661C
    {18, RF_TxPowerTable_HIGH_PA_ENTRY(34, 0, 0, 64, 4) }, // 0x048022
    {19, RF_TxPowerTable_HIGH_PA_ENTRY(15, 3, 0, 36, 4) }, // 0x0448CF
    {20, RF_TxPowerTable_HIGH_PA_ENTRY(18, 3, 0, 71, 27) }, // 0x1B8ED2
    RF_TxPowerTable_TERMINATION_ENTRY
};



//*********************************************************************************
//  RF Setting:   IEEE 802.15.4-2006, 250 kbps, OQPSK, DSSS = 1:8, 10 dBm
//
//  PHY:          ieee154p10
//  Setting file: setting_ieee_802_15_4_10_dbm.json
//*********************************************************************************

// PARAMETER SUMMARY
// Channel - Frequency (MHz): 2405
// TX Power (dBm): 0

// TI-RTOS RF Mode Object
RF_Mode RF_prop_ieee154 =
{
    .rfMode = RF_MODE_AUTO,
    .cpePatchFxn = &rf_patch_cpe_ieee_802_15_4,
    .mcePatchFxn = 0,
    .rfePatchFxn = 0
};

// Overrides for CMD_RADIO_SETUP_PA
uint32_t pOverrides_ieee154[] =
{
    // override_ieee_802_15_4.json
    // Rx: Set LNA bias current offset to +15 to saturate trim to max (default: 0)
    (uint32_t)0x000F8883,
    // Tx: Set DCDC settings IPEAK=3, dither = off
    (uint32_t)0x00F388D3,
    (uint32_t)0xFFFFFFFF
};



// CMD_RADIO_SETUP_PA
// Radio Setup Command for Pre-Defined Schemes
const rfc_CMD_RADIO_SETUP_PA_t RF_cmdRadioSetup_ieee154 =
{
    .commandNo = 0x0802,
    .status = 0x0000,
    .pNextOp = 0,
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
    .mode = 0x01,
    .loDivider = 0x00,
    .config.frontEndMode = 0x0,
    .config.biasMode = 0x1,
    .config.analogCfgMode = 0x0,
    .config.bNoFsPowerUp = 0x0,
    .config.bSynthNarrowBand = 0x0,
    .txPower = 0x2C5D,
    .pRegOverride = pOverrides_ieee154,
    .pRegOverrideTxStd = 0,
    .pRegOverrideTx20 = 0
};

// CMD_FS
// Frequency Synthesizer Programming Command
const rfc_CMD_FS_t RF_cmdFs_ieee154 =
{
    .commandNo = 0x0803,
    .status = 0x0000,
    .pNextOp = 0,
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
    .frequency = 0x0965,
    .fractFreq = 0x0000,
    .synthConf.bTxMode = 0x0,
    .synthConf.refFreq = 0x0,
    .__dummy0 = 0x00,
    .__dummy1 = 0x00,
    .__dummy2 = 0x00,
    .__dummy3 = 0x0000
};

// CMD_IEEE_TX
// IEEE 802.15.4 Transmit Command
const rfc_CMD_IEEE_TX_t RF_cmdIeeeTx_ieee154 =
{
    .commandNo = 0x2C01,
    .status = 0x0000,
    .pNextOp = 0,
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
    .txOpt.bIncludePhyHdr = 0x0,
    .txOpt.bIncludeCrc = 0x0,
    .txOpt.payloadLenMsb = 0x0,
    .payloadLen = 0x1E,
    .pPayload = 0,
    .timeStamp = 0x00000000
};

// CMD_IEEE_RX
// IEEE 802.15.4 Receive Command
const rfc_CMD_IEEE_RX_t RF_cmdIeeeRx_ieee154 =
{
    .commandNo = 0x2801,
    .status = 0x0000,
    .pNextOp = 0,
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
    .channel = 0x00,
    .rxConfig.bAutoFlushCrc = 0x0,
    .rxConfig.bAutoFlushIgn = 0x0,
    .rxConfig.bIncludePhyHdr = 0x0,
    .rxConfig.bIncludeCrc = 0x0,
    .rxConfig.bAppendRssi = 0x1,
    .rxConfig.bAppendCorrCrc = 0x1,
    .rxConfig.bAppendSrcInd = 0x0,
    .rxConfig.bAppendTimestamp = 0x0,
    .pRxQ = 0,
    .pOutput = 0,
    .frameFiltOpt.frameFiltEn = 0x0,
    .frameFiltOpt.frameFiltStop = 0x0,
    .frameFiltOpt.autoAckEn = 0x0,
    .frameFiltOpt.slottedAckEn = 0x0,
    .frameFiltOpt.autoPendEn = 0x0,
    .frameFiltOpt.defaultPend = 0x0,
    .frameFiltOpt.bPendDataReqOnly = 0x0,
    .frameFiltOpt.bPanCoord = 0x0,
    .frameFiltOpt.maxFrameVersion = 0x3,
    .frameFiltOpt.fcfReservedMask = 0x0,
    .frameFiltOpt.modifyFtFilter = 0x0,
    .frameFiltOpt.bStrictLenFilter = 0x0,
    .frameTypes.bAcceptFt0Beacon = 0x1,
    .frameTypes.bAcceptFt1Data = 0x1,
    .frameTypes.bAcceptFt2Ack = 0x1,
    .frameTypes.bAcceptFt3MacCmd = 0x1,
    .frameTypes.bAcceptFt4Reserved = 0x1,
    .frameTypes.bAcceptFt5Reserved = 0x1,
    .frameTypes.bAcceptFt6Reserved = 0x1,
    .frameTypes.bAcceptFt7Reserved = 0x1,
    .ccaOpt.ccaEnEnergy = 0x0,
    .ccaOpt.ccaEnCorr = 0x0,
    .ccaOpt.ccaEnSync = 0x0,
    .ccaOpt.ccaCorrOp = 0x1,
    .ccaOpt.ccaSyncOp = 0x1,
    .ccaOpt.ccaCorrThr = 0x0,
    .ccaRssiThr = 0x64,
    .__dummy0 = 0x00,
    .numExtEntries = 0x00,
    .numShortEntries = 0x00,
    .pExtEntryList = 0,
    .pShortEntryList = 0,
    .localExtAddr = 0x12345678,
    .localShortAddr = 0xABBA,
    .localPanID = 0x0000,
    .__dummy1 = 0x000000,
    .endTrigger.triggerType = 0x1,
    .endTrigger.bEnaCmd = 0x0,
    .endTrigger.triggerNo = 0x0,
    .endTrigger.pastTrig = 0x0,
    .endTime = 0x00000000
};

// CMD_IEEE_CSMA
// IEEE 802.15.4 CSMA-CA Command
const rfc_CMD_IEEE_CSMA_t RF_cmdIeeeCsma_ieee154 =
{
    .commandNo = 0x2C02,
    .status = 0x0000,
    .pNextOp = 0,
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x0,
    .condition.nSkip = 0x0,
    .randomState = 0x0000,
    .macMaxBE = 0x00,
    .macMaxCSMABackoffs = 0x00,
    .csmaConfig.initCW = 0x0,
    .csmaConfig.bSlotted = 0x0,
    .csmaConfig.rxOffMode = 0x0,
    .NB = 0x00,
    .BE = 0x00,
    .remainingPeriods = 0x00,
    .lastRssi = 0x00,
    .endTrigger.triggerType = 0x0,
    .endTrigger.bEnaCmd = 0x0,
    .endTrigger.triggerNo = 0x0,
    .endTrigger.pastTrig = 0x0,
    .lastTimeStamp = 0x00000000,
    .endTime = 0x00000000
};

// CMD_IEEE_RX_ACK
// IEEE 802.15.4 Receive Acknowledgement Command
const rfc_CMD_IEEE_RX_ACK_t RF_cmdIeeeRxAck_ieee154 =
{
    .commandNo = 0x2C03,
    .status = 0x0000,
    .pNextOp = 0,
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x0,
    .condition.nSkip = 0x0,
    .seqNo = 0x00,
    .endTrigger.triggerType = 0x0,
    .endTrigger.bEnaCmd = 0x0,
    .endTrigger.triggerNo = 0x0,
    .endTrigger.pastTrig = 0x0,
    .endTime = 0x00000000
};



//*********************************************************************************
//  RF Setting:   100 kbps, 50 kHz Deviation, 2-GFSK, 311 kHz RX Bandwidth (Preview settings)
//
//  PHY:          2gfsk100kbps
//  Setting file: setting_tc900.json
//*********************************************************************************

// PARAMETER SUMMARY
// RX Address Mode: No address check
// Frequency (MHz): 2440.0000
// Deviation (kHz): 50.0
// Packet Length Config: Variable
// Max Packet Length: 255
// Preamble Count: 4 Bytes
// Preamble Mode: Send 0 as the first preamble bit
// RX Filter BW (kHz): 310.8
// Symbol Rate (kBaud): 100.000
// Sync Word: 0x930B51DE
// Sync Word Length: 32 Bits
// TX Power (dBm): 0
// Whitening: No whitening

// TI-RTOS RF Mode Object
RF_Mode RF_prop =
{
    .rfMode = RF_MODE_PROPRIETARY_2_4,
    .cpePatchFxn = &rf_patch_cpe_prop,
    .mcePatchFxn = 0,
    .rfePatchFxn = 0
};

// Overrides for CMD_PROP_RADIO_DIV_SETUP_PA
uint32_t pOverrides[] =
{
    // override_tc900.json
    // DC/DC regulator: In Tx, use DCDCCTL5[3:0]=0x3 (DITHER_EN=0 and IPEAK=3).
    (uint32_t)0x00F388D3,
    // Tx: Configure PA ramp time, PACTL2.RC=0x2 (in ADI0, set PACTL2[4:3]=0x2)
    ADI_2HALFREG_OVERRIDE(0,16,0x8,0x0,17,0x1,0x1),
    // Rx: Set AGC reference level to 0x20 (default: 0x2E)
    HW_REG_OVERRIDE(0x609C,0x0020),
    // Rx: Set anti-aliasing filter bandwidth to 0x9 (in ADI0, set IFAMPCTL3[7:4]=0x9)
    ADI_HALFREG_OVERRIDE(0,61,0xF,0x9),
    // Synth: Increase mid code calibration time to 5 us
    (uint32_t)0x00058683,
    // Synth: Increase mid code calibration time to 5 us
    HW32_ARRAY_OVERRIDE(0x4004,1),
    // Synth: Increase mid code calibration time to 5 us
    (uint32_t)0x38183C30,
    (uint32_t)0xFFFFFFFF
};



// CMD_PROP_RADIO_DIV_SETUP_PA
// Proprietary Mode Radio Setup Command for All Frequency Bands
rfc_CMD_PROP_RADIO_DIV_SETUP_PA_t RF_cmdPropRadioDivSetup =
{
    .commandNo = 0x3807,
    .status = 0x0000,
    .pNextOp = 0,
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
    .modulation.modType = 0x1,
    .modulation.deviation = 0xC8,
    .modulation.deviationStepSz = 0x0,
    .symbolRate.preScale = 0xF,
    .symbolRate.rateWord = 0x10000,
    .symbolRate.decimMode = 0x0,
    .rxBw = 0x59,
    .preamConf.nPreamBytes = 0x4,
    .preamConf.preamMode = 0x0,
    .formatConf.nSwBits = 0x20,
    .formatConf.bBitReversal = 0x0,
    .formatConf.bMsbFirst = 0x1,
    .formatConf.fecMode = 0x0,
    .formatConf.whitenMode = 0x0,
    .config.frontEndMode = 0x0,
    .config.biasMode = 0x1,
    .config.analogCfgMode = 0x0,
    .config.bNoFsPowerUp = 0x0,
    .config.bSynthNarrowBand = 0x0,
    .txPower = 0x2C5D,
    .pRegOverride = pOverrides,
    .centerFreq = 0x0988,
    .intFreq = 0x0800,
    .loDivider = 0x00,
    .pRegOverrideTxStd = 0,
    .pRegOverrideTx20 = 0
};

// CMD_FS
// Frequency Synthesizer Programming Command
rfc_CMD_FS_t RF_cmdFs =
{
    .commandNo = 0x0803,
    .status = 0x0000,
    .pNextOp = 0,
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
    .frequency = 0x0988,
    .fractFreq = 0x0000,
    .synthConf.bTxMode = 0x0,
    .synthConf.refFreq = 0x0,
    .__dummy0 = 0x00,
    .__dummy1 = 0x00,
    .__dummy2 = 0x00,
    .__dummy3 = 0x0000
};

// CMD_PROP_TX
// Proprietary Mode Transmit Command
rfc_CMD_PROP_TX_t RF_cmdPropTx =
{
    .commandNo = 0x3801,
    .status = 0x0000,
    .pNextOp = 0,
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
    .pktConf.bFsOff = 0x0,
    .pktConf.bUseCrc = 0x1,
    .pktConf.bVarLen = 0x1,
    .pktLen = 0x14,
    .syncWord = 0x930B51DE,
    .pPkt = 0
};

// CMD_PROP_RX
// Proprietary Mode Receive Command
rfc_CMD_PROP_RX_t RF_cmdPropRx =
{
    .commandNo = 0x3802,
    .status = 0x0000,
    .pNextOp = 0,
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
    .pktConf.bFsOff = 0x0,
    .pktConf.bRepeatOk = 0x0,
    .pktConf.bRepeatNok = 0x0,
    .pktConf.bUseCrc = 0x1,
    .pktConf.bVarLen = 0x1,
    .pktConf.bChkAddress = 0x0,
    .pktConf.endType = 0x0,
    .pktConf.filterOp = 0x0,
    .rxConf.bAutoFlushIgnored = 0x0,
    .rxConf.bAutoFlushCrcErr = 0x0,
    .rxConf.bIncludeHdr = 0x1,
    .rxConf.bIncludeCrc = 0x0,
    .rxConf.bAppendRssi = 0x0,
    .rxConf.bAppendTimestamp = 0x0,
    .rxConf.bAppendStatus = 0x1,
    .syncWord = 0x930B51DE,
    .maxPktLen = 0xFF,
    .address0 = 0xAA,
    .address1 = 0xBB,
    .endTrigger.triggerType = 0x1,
    .endTrigger.bEnaCmd = 0x0,
    .endTrigger.triggerNo = 0x0,
    .endTrigger.pastTrig = 0x0,
    .endTime = 0x00000000,
    .pQueue = 0,
    .pOutput = 0
};


