/*
 *  ======== ti_drivers_config.c ========
 *  Configured TI-Drivers module definitions
 *
 *  DO NOT EDIT - This file is generated for the LP_CC1352P7_4
 *  by the SysConfig tool.
 */

#include <stddef.h>
#include <stdint.h>

#ifndef DeviceFamily_CC13X2X7
#define DeviceFamily_CC13X2X7
#endif

#include <ti/devices/DeviceFamily.h>

#include "ti_drivers_config.h"

/*
 *  ============================= Display =============================
 */

#include <ti/display/Display.h>
#include <ti/display/DisplayUart2.h>

#define CONFIG_Display_COUNT 1


#define Display_UART2BUFFERSIZE 1024
static char displayUART2Buffer[Display_UART2BUFFERSIZE];

DisplayUart2_Object displayUart2Object;

const DisplayUart2_HWAttrs displayUart2HWAttrs = {
    .uartIdx      = CONFIG_DISPLAY_UART,
    .baudRate     = 115200,
    .mutexTimeout = (unsigned int)(-1),
    .strBuf       = displayUART2Buffer,
    .strBufLen    = Display_UART2BUFFERSIZE
};

const Display_Config Display_config[CONFIG_Display_COUNT] = {
    /* CONFIG_DISPLAY */
    /* XDS110 UART */
    {
        .fxnTablePtr = &DisplayUart2Min_fxnTable,
        .object      = &displayUart2Object,
        .hwAttrs     = &displayUart2HWAttrs
    },
};

const uint_least8_t Display_count = CONFIG_Display_COUNT;

/*
 *  =============================== AESCBC ===============================
 */

#include <ti/drivers/AESCBC.h>
#include <ti/drivers/aescbc/AESCBCCC26XX.h>

#define CONFIG_AESCBC_COUNT 1


AESCBCCC26XX_Object aescbcCC26XXObjects[CONFIG_AESCBC_COUNT];

/*
 *  ======== aescbcCC26XXHWAttrs ========
 */
const AESCBCCC26XX_HWAttrs aescbcCC26XXHWAttrs[CONFIG_AESCBC_COUNT] = {
    {
        .intPriority = (~0),
    },
};

const AESCBC_Config AESCBC_config[CONFIG_AESCBC_COUNT] = {
    {   /* CONFIG_AESCBC_0 */
        .object  = &aescbcCC26XXObjects[CONFIG_AESCBC_0],
        .hwAttrs = &aescbcCC26XXHWAttrs[CONFIG_AESCBC_0]
    },
};

const uint_least8_t CONFIG_AESCBC_0_CONST = CONFIG_AESCBC_0;
const uint_least8_t AESCBC_count = CONFIG_AESCBC_COUNT;

/*
 *  =============================== AESCCM ===============================
 */

#include <ti/drivers/AESCCM.h>
#include <ti/drivers/aesccm/AESCCMCC26XX.h>

#define CONFIG_AESCCM_COUNT 1
AESCCMCC26XX_Object aesccmCC26XXObjects[CONFIG_AESCCM_COUNT];

/*
 *  ======== aesccmCC26XXHWAttrs ========
 */
const AESCCMCC26XX_HWAttrs aesccmCC26XXHWAttrs[CONFIG_AESCCM_COUNT] = {
    {
        .intPriority = 0x40,
    },
};

const AESCCM_Config AESCCM_config[CONFIG_AESCCM_COUNT] = {
    {   /* CONFIG_AESCCM_0 */
        .object  = &aesccmCC26XXObjects[CONFIG_AESCCM_0],
        .hwAttrs = &aesccmCC26XXHWAttrs[CONFIG_AESCCM_0]
    },
};

const uint_least8_t CONFIG_AESCCM_0_CONST = CONFIG_AESCCM_0;
const uint_least8_t AESCCM_count = CONFIG_AESCCM_COUNT;

/*
 *  =============================== AESECB ===============================
 */

#include <ti/drivers/AESECB.h>
#include <ti/drivers/aesecb/AESECBCC26XX.h>

#define CONFIG_AESECB_COUNT 1


AESECBCC26XX_Object aesecbCC26XXObjects[CONFIG_AESECB_COUNT];

/*
 *  ======== aesecbCC26XXHWAttrs ========
 */
const AESECBCC26XX_HWAttrs aesecbCC26XXHWAttrs[CONFIG_AESECB_COUNT] = {
    {
        .intPriority = 0x20,
    },
};

const AESECB_Config AESECB_config[CONFIG_AESECB_COUNT] = {
    {   /* CONFIG_AESECB_0 */
        .object  = &aesecbCC26XXObjects[CONFIG_AESECB_0],
        .hwAttrs = &aesecbCC26XXHWAttrs[CONFIG_AESECB_0]
    },
};


const uint_least8_t CONFIG_AESECB_0_CONST = CONFIG_AESECB_0;
const uint_least8_t AESECB_count = CONFIG_AESECB_COUNT;

/*
 *  =============================== DMA ===============================
 */

#include <ti/drivers/dma/UDMACC26XX.h>
#include <ti/devices/cc13x2x7_cc26x2x7/driverlib/udma.h>
#include <ti/devices/cc13x2x7_cc26x2x7/inc/hw_memmap.h>

UDMACC26XX_Object udmaCC26XXObject;

const UDMACC26XX_HWAttrs udmaCC26XXHWAttrs = {
    .baseAddr        = UDMA0_BASE,
    .powerMngrId     = PowerCC26XX_PERIPH_UDMA,
    .intNum          = INT_DMA_ERR,
    .intPriority     = (~0)
};

const UDMACC26XX_Config UDMACC26XX_config[1] = {
    {
        .object         = &udmaCC26XXObject,
        .hwAttrs        = &udmaCC26XXHWAttrs,
    },
};

/*
 *  =============================== ECDH ===============================
 */

#include <ti/drivers/ECDH.h>
#include <ti/drivers/ecdh/ECDHCC26X2.h>

#define CONFIG_ECDH_COUNT 1


ECDHCC26X2_Object ecdhCC26X2Objects[CONFIG_ECDH_COUNT];

/*
 *  ======== ecdhCC26X2HWAttrs ========
 */
const ECDHCC26X2_HWAttrs ecdhCC26X2HWAttrs[CONFIG_ECDH_COUNT] = {
    {
        .intPriority = (~0),
    },
};

const ECDH_Config ECDH_config[CONFIG_ECDH_COUNT] = {
    {   /* CONFIG_ECDH_0 */
        .object         = &ecdhCC26X2Objects[CONFIG_ECDH_0],
        .hwAttrs        = &ecdhCC26X2HWAttrs[CONFIG_ECDH_0]
    },
};

const uint_least8_t CONFIG_ECDH_0_CONST = CONFIG_ECDH_0;
const uint_least8_t ECDH_count = CONFIG_ECDH_COUNT;

/*
 *  =============================== ECDSA ===============================
 */

#include <ti/drivers/ECDSA.h>
#include <ti/drivers/ecdsa/ECDSACC26X2.h>

#define CONFIG_ECDSA_COUNT 1
ECDSACC26X2_Object ecdsaCC26X2Objects[CONFIG_ECDSA_COUNT];

/*
 *  ======== ecdsaCC26X2HWAttrs ========
 */
const ECDSACC26X2_HWAttrs ecdsaCC26X2HWAttrs[CONFIG_ECDSA_COUNT] = {
    {
        .intPriority = (~0),
        .trngIntPriority = (~0),
    },
};

const ECDSA_Config ECDSA_config[CONFIG_ECDSA_COUNT] = {
    {   /* CONFIG_ECDSA_0 */
        .object         = &ecdsaCC26X2Objects[CONFIG_ECDSA_0],
        .hwAttrs        = &ecdsaCC26X2HWAttrs[CONFIG_ECDSA_0]
    },
};

const uint_least8_t CONFIG_ECDSA_0_CONST = CONFIG_ECDSA_0;
const uint_least8_t ECDSA_count = CONFIG_ECDSA_COUNT;

/*
 *  =============================== GPIO ===============================
 */

#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOCC26XX.h>

/* The range of pins available on this device */
const uint_least8_t GPIO_pinLowerBound = 5;
const uint_least8_t GPIO_pinUpperBound = 30;

/*
 *  ======== gpioPinConfigs ========
 *  Array of Pin configurations
 */
GPIO_PinConfig gpioPinConfigs[31] = {
    0, /* Pin is not available on this device */
    0, /* Pin is not available on this device */
    0, /* Pin is not available on this device */
    0, /* Pin is not available on this device */
    0, /* Pin is not available on this device */
    GPIO_CFG_NO_DIR, /* DIO_5 */
    /* Owned by CONFIG_LED_RED as LED GPIO */
    GPIO_CFG_OUTPUT_INTERNAL | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, /* CONFIG_GPIO_RLED */
    /* Owned by CONFIG_LED_GREEN as LED GPIO */
    GPIO_CFG_OUTPUT_INTERNAL | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, /* CONFIG_GPIO_GLED */
    /* Owned by CONFIG_SPI_0 as MISO */
    GPIO_CFG_INPUT_INTERNAL | GPIO_CFG_IN_INT_NONE | GPIO_CFG_PULL_NONE_INTERNAL, /* CONFIG_PIN_SPI_MISO */
    /* Owned by CONFIG_SPI_0 as MOSI */
    GPIO_CFG_OUTPUT_INTERNAL | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, /* CONFIG_PIN_SPI_MOSI */
    /* Owned by CONFIG_SPI_0 as SCLK */
    GPIO_CFG_OUTPUT_INTERNAL | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW, /* CONFIG_PIN_SPI_SCLK */
    GPIO_CFG_NO_DIR, /* DIO_11 */
    /* Owned by CONFIG_DISPLAY_UART as RX */
    GPIO_CFG_INPUT_INTERNAL | GPIO_CFG_IN_INT_NONE | GPIO_CFG_PULL_DOWN_INTERNAL, /* CONFIG_GPIO_DISPLAY_UART_RX */
    /* Owned by CONFIG_DISPLAY_UART as TX */
    GPIO_CFG_OUTPUT_INTERNAL | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH, /* CONFIG_GPIO_DISPLAY_UART_TX */
    /* Owned by CONFIG_BTN_RIGHT as Button GPIO */
    GPIO_CFG_INPUT_INTERNAL | GPIO_CFG_IN_INT_NONE | GPIO_CFG_PULL_UP_INTERNAL, /* CONFIG_GPIO_BTN2 */
    /* Owned by CONFIG_BTN_LEFT as Button GPIO */
    GPIO_CFG_INPUT_INTERNAL | GPIO_CFG_IN_INT_NONE | GPIO_CFG_PULL_UP_INTERNAL, /* CONFIG_GPIO_BTN1 */
    GPIO_CFG_NO_DIR, /* DIO_16 */
    GPIO_CFG_NO_DIR, /* DIO_17 */
    GPIO_CFG_NO_DIR, /* DIO_18 */
    GPIO_CFG_NO_DIR, /* DIO_19 */
    /* Owned by CONFIG_NVS_SPI_0 as Button GPIO */
    GPIO_CFG_OUTPUT_INTERNAL | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH, /* CONFIG_GPIO_0 */
    GPIO_CFG_NO_DIR, /* DIO_21 */
    GPIO_CFG_NO_DIR, /* DIO_22 */
    GPIO_CFG_NO_DIR, /* DIO_23 */
    GPIO_CFG_NO_DIR, /* DIO_24 */
    GPIO_CFG_NO_DIR, /* DIO_25 */
    GPIO_CFG_NO_DIR, /* DIO_26 */
    GPIO_CFG_NO_DIR, /* DIO_27 */
    /* Owned by /ti/drivers/RF as RF Antenna Pin 0 */
    GPIO_CFG_OUTPUT_INTERNAL | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW, /* CONFIG_RF_24GHZ */
    /* Owned by /ti/drivers/RF as RF Antenna Pin 1 */
    GPIO_CFG_OUTPUT_INTERNAL | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW, /* CONFIG_RF_HIGH_PA */
    /* Owned by /ti/drivers/RF as RF Antenna Pin 2 */
    GPIO_CFG_OUTPUT_INTERNAL | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW, /* CONFIG_RF_SUB1GHZ */
};

/*
 *  ======== gpioCallbackFunctions ========
 *  Array of callback function pointers
 *  Change at runtime with GPIO_setCallback()
 */
GPIO_CallbackFxn gpioCallbackFunctions[31];

/*
 *  ======== gpioUserArgs ========
 *  Array of user argument pointers
 *  Change at runtime with GPIO_setUserArg()
 *  Get values with GPIO_getUserArg()
 */
void* gpioUserArgs[31];

const uint_least8_t CONFIG_GPIO_DISPLAY_UART_TX_CONST = CONFIG_GPIO_DISPLAY_UART_TX;
const uint_least8_t CONFIG_GPIO_DISPLAY_UART_RX_CONST = CONFIG_GPIO_DISPLAY_UART_RX;
const uint_least8_t CONFIG_RF_24GHZ_CONST = CONFIG_RF_24GHZ;
const uint_least8_t CONFIG_RF_HIGH_PA_CONST = CONFIG_RF_HIGH_PA;
const uint_least8_t CONFIG_RF_SUB1GHZ_CONST = CONFIG_RF_SUB1GHZ;
const uint_least8_t CONFIG_GPIO_BTN1_CONST = CONFIG_GPIO_BTN1;
const uint_least8_t CONFIG_GPIO_BTN2_CONST = CONFIG_GPIO_BTN2;
const uint_least8_t CONFIG_GPIO_RLED_CONST = CONFIG_GPIO_RLED;
const uint_least8_t CONFIG_GPIO_GLED_CONST = CONFIG_GPIO_GLED;
const uint_least8_t CONFIG_GPIO_0_CONST = CONFIG_GPIO_0;
const uint_least8_t CONFIG_PIN_SPI_SCLK_CONST = CONFIG_PIN_SPI_SCLK;
const uint_least8_t CONFIG_PIN_SPI_MISO_CONST = CONFIG_PIN_SPI_MISO;
const uint_least8_t CONFIG_PIN_SPI_MOSI_CONST = CONFIG_PIN_SPI_MOSI;

/*
 *  ======== GPIO_config ========
 */
const GPIO_Config GPIO_config = {
    .configs = (GPIO_PinConfig *)gpioPinConfigs,
    .callbacks = (GPIO_CallbackFxn *)gpioCallbackFunctions,
    .userArgs = gpioUserArgs,
    .intPriority = (~0)
};

/*
 *  =============================== NVS ===============================
 */

#include <ti/drivers/NVS.h>
#include <ti/drivers/nvs/NVSCC26XX.h>

/*
 *  NVSCC26XX Internal NVS flash region definitions
 *
 * Place uninitialized char arrays at addresses
 * corresponding to the 'regionBase' addresses defined in
 * the configured NVS regions. These arrays are used as
 * place holders so that the linker will not place other
 * content there.
 *
 * For GCC targets, the char arrays are each placed into
 * the shared ".nvs" section. The user must add content to
 * their GCC linker command file to place the .nvs section
 * at the lowest 'regionBase' address specified in their NVS
 * regions.
 */

#if defined(__TI_COMPILER_VERSION__) || defined(__clang__)

static char flashBuf0[0x4000] __attribute__ ((retain, noinit, location(0xaa000)));

#elif defined(__IAR_SYSTEMS_ICC__)

__no_init static char flashBuf0[0x4000] @ 0xaa000;

#elif defined(__GNUC__)

__attribute__ ((section (".nvs")))
static char flashBuf0[0x4000];

#endif

NVSCC26XX_Object nvsCC26XXObjects[1];

static const NVSCC26XX_HWAttrs nvsCC26XXHWAttrs[1] = {
    /* CONFIG_NVSINTERNAL */
    {
        .regionBase = (void *) flashBuf0,
        .regionSize = 0x4000
    },
};

#include <ti/drivers/nvs/NVSSPI25X.h>

/*
 *  NVSSPI25X External NVS flash region definitions
 */

/*
 * Provide write verification buffer whose size is
 * the largest specified Verification Buffer Size
 */

static uint8_t verifyBuf[64];

NVSSPI25X_Object nvsSPI25XObjects[1];

static const NVSSPI25X_HWAttrs nvsSPI25XHWAttrs[1] = {
    /* CONFIG_NVSEXTERNAL */
    /* MX25R8035F SPI Flash */
    {
        .regionBaseOffset = 0x0,
        .regionSize = 0x256000,
        .sectorSize = 0x1000,
        .verifyBuf = verifyBuf,
        .verifyBufSize = 64,
        /* NVS opens SPI */
        .spiHandle = NULL,
        /* SPI driver index */
        .spiIndex = CONFIG_SPI_0,
        .spiBitRate = 4000000,
        /* GPIO driver pin index */
        .spiCsnGpioIndex = CONFIG_GPIO_0,
        .statusPollDelayUs = 100
    },
};

#define CONFIG_NVS_COUNT 2

const NVS_Config NVS_config[CONFIG_NVS_COUNT] = {
    /* CONFIG_NVSINTERNAL */
    {
        .fxnTablePtr = &NVSCC26XX_fxnTable,
        .object = &nvsCC26XXObjects[0],
        .hwAttrs = &nvsCC26XXHWAttrs[0],
    },
    /* CONFIG_NVSEXTERNAL */
    /* MX25R8035F SPI Flash */
    {
        .fxnTablePtr = &NVSSPI25X_fxnTable,
        .object = &nvsSPI25XObjects[0],
        .hwAttrs = &nvsSPI25XHWAttrs[0],
    },
};

const uint_least8_t CONFIG_NVSINTERNAL_CONST = CONFIG_NVSINTERNAL;
const uint_least8_t CONFIG_NVSEXTERNAL_CONST = CONFIG_NVSEXTERNAL;
const uint_least8_t NVS_count = CONFIG_NVS_COUNT;

/*
 *  =============================== Power ===============================
 */
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26X2.h>
#include "ti_drivers_config.h"

extern void PowerCC26XX_standbyPolicy(void);
extern bool PowerCC26XX_calibrate(unsigned int);

const PowerCC26X2_Config PowerCC26X2_config = {
    .enablePolicy             = true,
    .policyInitFxn            = NULL,
    .policyFxn                = PowerCC26XX_standbyPolicy,
    .calibrateFxn             = PowerCC26XX_calibrate,
    .calibrateRCOSC_LF        = true,
    .calibrateRCOSC_HF        = true,
    .enableTCXOFxn            = NULL
};


/*
 *  =============================== RF Driver ===============================
 */
#include <ti/drivers/GPIO.h>
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/ioc.h)
#include <ti/drivers/rf/RF.h>

/*
 * RF driver callback function, called by the driver on global driver events.
 */
static void RF_globalCallbackFunction (RF_Handle client, RF_GlobalEvent events, void* arg);

/*
 * Callback function to handle custom / application specific behavior
 */
extern void __attribute__((weak)) rfDriverCallback (RF_Handle client, RF_GlobalEvent events, void *arg);

/*
 * Callback function to handle antenna switching
 */
extern void __attribute__((weak)) rfDriverCallbackAntennaSwitching (RF_Handle client, RF_GlobalEvent events, void *arg);

/*
 * Platform-specific driver configuration
 */
const RFCC26XX_HWAttrsV2 RFCC26XX_hwAttrs = {
    .hwiPriority        = (~0),
    .swiPriority        = (uint8_t)0,
    .xoscHfAlwaysNeeded = true,
    .globalCallback     = &RF_globalCallbackFunction,
    .globalEventMask    = RF_GlobalEventInit | RF_GlobalEventRadioPowerDown | RF_GlobalEventRadioSetup
};

/*
 *  ======== RF_globalCallbackFunction ========
 *  This function is called by the driver on global driver events.
 *  It will call specific callback functions to further handle the triggering events.
 */
static void RF_globalCallbackFunction(RF_Handle client, RF_GlobalEvent events, void *arg)
{
    rfDriverCallback(client, events, arg);
    rfDriverCallbackAntennaSwitching(client, events, arg);
}

/*
 *  ======== rfDriverCallback ========
 *  Handle events triggered by the RF driver for custom / application specific behavior.
 */
void __attribute__((weak)) rfDriverCallback(RF_Handle client, RF_GlobalEvent events, void *arg)
{
    /* ======== PLEASE READ THIS ========
    *
    * This function is declared weak for the application to override it.
    * A new definition of 'rfDriverCallback' is required if you want to
    * handle the events listed in '.globalEventMask'.
    *
    * Please copy this function definition to create your own, but make
    * sure to remove '__attribute__((weak))' for your definition.
    *
    * According to '.globalEventMask', this function will be triggered by:
    *   - RF_GlobalEventInit
    *   - RF_GlobalEventRadioPowerDown
    *   - RF_GlobalEventRadioSetup
    *
    * An example of how to handle these events would be:
    *
    *   --- Code snippet begin ---
    *
    *   if(events & RF_GlobalEventInit) {
    *       // Perform action for this event
    *   }
    *   else if (events & RF_GlobalEventRadioPowerDown) {
    *       // Perform action for this event
    *   }
    *   else if (events & RF_GlobalEventRadioSetup) {
    *       // Perform action for this event
    *   }
    *
    *   --- Code snippet end ---
    */
}



/*
 * ======== Antenna switching ========
 */
/*
 * ======== rfDriverCallbackAntennaSwitching ========
 * Sets up the antenna switch depending on the current PHY configuration.
 *
 * Truth table:
 *
 * Path       DIO28 DIO29 DIO30
 * ========== ===== ===== ===== 
 * Off        0     0     0
 * 2.4 GHZ    1     0     0
 * HIGH PA    0     1     0
 * SUB1 GHZ   0     0     1
 */
void __attribute__((weak)) rfDriverCallbackAntennaSwitching(RF_Handle client, RF_GlobalEvent events, void *arg)
{

    if (events & RF_GlobalEventRadioSetup) {
        bool    sub1GHz   = false;
        uint8_t loDivider = 0;

        /* Switch off all paths. */
        GPIO_write(CONFIG_RF_24GHZ, 0);
        GPIO_write(CONFIG_RF_HIGH_PA, 0);
        GPIO_write(CONFIG_RF_SUB1GHZ, 0);

        /* Decode the current PA configuration. */
        RF_TxPowerTable_PAType paType = (RF_TxPowerTable_PAType)RF_getTxPower(client).paType;

        /* Decode the generic argument as a setup command. */
        RF_RadioSetup* setupCommand = (RF_RadioSetup*)arg;

        switch (setupCommand->common.commandNo) {
            case (CMD_RADIO_SETUP):
            case (CMD_BLE5_RADIO_SETUP):
                    loDivider = RF_LODIVIDER_MASK & setupCommand->common.loDivider;

                    /* Sub-1GHz front-end. */
                    if (loDivider != 0) {
                        sub1GHz = true;
                    }
                    break;
            case (CMD_PROP_RADIO_DIV_SETUP):
                    loDivider = RF_LODIVIDER_MASK & setupCommand->prop_div.loDivider;

                    /* Sub-1GHz front-end. */
                    if (loDivider != 0) {
                        sub1GHz = true;
                    }
                    break;
            default:break;
        }

        if (sub1GHz) {
            /* Sub-1 GHz */
            if (paType == RF_TxPowerTable_HighPA) {
                /* PA enable --> HIGH PA
                 * LNA enable --> Sub-1 GHz
                 */
                GPIO_setMux(CONFIG_RF_24GHZ, IOC_PORT_GPIO);
                /* Note: RFC_GPO3 is a work-around because the RFC_GPO1 (PA enable signal) is sometimes not
                         de-asserted on CC1352 Rev A. */
                GPIO_setMux(CONFIG_RF_HIGH_PA, IOC_PORT_RFC_GPO3);
                GPIO_setMux(CONFIG_RF_SUB1GHZ, IOC_PORT_RFC_GPO0);
            } else {
                /* RF core active --> Sub-1 GHz */
                GPIO_setMux(CONFIG_RF_24GHZ, IOC_PORT_GPIO);
                GPIO_setMux(CONFIG_RF_HIGH_PA, IOC_PORT_GPIO);
                GPIO_setMux(CONFIG_RF_SUB1GHZ, IOC_PORT_GPIO);
                GPIO_write(CONFIG_RF_SUB1GHZ, 1);
            }
        } else {
            /* 2.4 GHz */
            if (paType == RF_TxPowerTable_HighPA)
            {
                /* PA enable --> HIGH PA
                 * LNA enable --> 2.4 GHz
                 */
                GPIO_setMux(CONFIG_RF_24GHZ, IOC_PORT_RFC_GPO0);
                /* Note: RFC_GPO3 is a work-around because the RFC_GPO1 (PA enable signal) is sometimes not
                         de-asserted on CC1352 Rev A. */
                GPIO_setMux(CONFIG_RF_HIGH_PA, IOC_PORT_RFC_GPO3);
                GPIO_setMux(CONFIG_RF_SUB1GHZ, IOC_PORT_GPIO);
            } else {
                /* RF core active --> 2.4 GHz */
                GPIO_setMux(CONFIG_RF_24GHZ, IOC_PORT_GPIO);
                GPIO_setMux(CONFIG_RF_HIGH_PA, IOC_PORT_GPIO);
                GPIO_setMux(CONFIG_RF_SUB1GHZ, IOC_PORT_GPIO);
                GPIO_write(CONFIG_RF_24GHZ, 1);
            }
        }
    }
    else if (events & RF_GlobalEventRadioPowerDown) {
        /* Switch off all paths. */
        GPIO_write(CONFIG_RF_24GHZ, 0);
        GPIO_write(CONFIG_RF_HIGH_PA, 0);
        GPIO_write(CONFIG_RF_SUB1GHZ, 0);

        /* Reset the IO multiplexer to GPIO functionality */
        GPIO_setMux(CONFIG_RF_24GHZ, IOC_PORT_GPIO);
        GPIO_setMux(CONFIG_RF_HIGH_PA, IOC_PORT_GPIO);
        GPIO_setMux(CONFIG_RF_SUB1GHZ, IOC_PORT_GPIO);
    }
}

/*
 *  =============================== SHA2 ===============================
 */

#include <ti/drivers/SHA2.h>
#include <ti/drivers/sha2/SHA2CC26X2.h>

#define CONFIG_SHA2_COUNT 1


SHA2CC26X2_Object sha2CC26X2Objects[CONFIG_SHA2_COUNT];

/*
 *  ======== sha2CC26X2HWAttrs ========
 */
const SHA2CC26X2_HWAttrs sha2CC26X2HWAttrs[CONFIG_SHA2_COUNT] = {
    {
        .intPriority = (~0),
    },
};

const SHA2_Config SHA2_config[CONFIG_SHA2_COUNT] = {
    {   /* CONFIG_SHA2_0 */
        .object         = &sha2CC26X2Objects[CONFIG_SHA2_0],
        .hwAttrs        = &sha2CC26X2HWAttrs[CONFIG_SHA2_0]
    },
};

const uint_least8_t CONFIG_SHA2_0_CONST = CONFIG_SHA2_0;
const uint_least8_t SHA2_count = CONFIG_SHA2_COUNT;

/*
 *  =============================== SPI DMA ===============================
 */
#include <ti/drivers/SPI.h>
#include <ti/drivers/spi/SPICC26X2DMA.h>
#include <ti/drivers/dma/UDMACC26XX.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/ioc.h)

#define CONFIG_SPI_COUNT 1

/*
 *  ======== spiCC26X2DMAObjects ========
 */
SPICC26X2DMA_Object spiCC26X2DMAObjects[CONFIG_SPI_COUNT];

/*
 * ======== spiCC26X2DMA uDMA Table Entries  ========
 */
ALLOCATE_CONTROL_TABLE_ENTRY(dmaSpi0TxControlTableEntry, UDMA_CHAN_SSI0_TX);
ALLOCATE_CONTROL_TABLE_ENTRY(dmaSpi0RxControlTableEntry, UDMA_CHAN_SSI0_RX);
ALLOCATE_CONTROL_TABLE_ENTRY(dmaSpi0TxAltControlTableEntry, (UDMA_CHAN_SSI0_TX | UDMA_ALT_SELECT));
ALLOCATE_CONTROL_TABLE_ENTRY(dmaSpi0RxAltControlTableEntry, (UDMA_CHAN_SSI0_RX | UDMA_ALT_SELECT));


/*
 *  ======== spiCC26X2DMAHWAttrs ========
 */
const SPICC26X2DMA_HWAttrs spiCC26X2DMAHWAttrs[CONFIG_SPI_COUNT] = {
    /* CONFIG_SPI_0 */
    /* LaunchPad SPI Bus */
    {
        .baseAddr = SSI0_BASE,
        .intNum = INT_SSI0_COMB,
        .intPriority = (~0),
        .swiPriority = 0,
        .powerMngrId = PowerCC26XX_PERIPH_SSI0,
        .defaultTxBufValue = ~0,
        .rxChannelBitMask = 1<<UDMA_CHAN_SSI0_RX,
        .txChannelBitMask = 1<<UDMA_CHAN_SSI0_TX,
        .dmaTxTableEntryPri = &dmaSpi0TxControlTableEntry,
        .dmaRxTableEntryPri = &dmaSpi0RxControlTableEntry,
        .dmaTxTableEntryAlt = &dmaSpi0TxAltControlTableEntry,
        .dmaRxTableEntryAlt = &dmaSpi0RxAltControlTableEntry,
        .minDmaTransferSize = 10,
        .txPinMux   = IOC_PORT_MCU_SSI0_TX,
        .rxPinMux   = IOC_PORT_MCU_SSI0_RX,
        .clkPinMux  = IOC_PORT_MCU_SSI0_CLK,
        .csnPinMux  = IOC_PORT_MCU_SSI0_FSS,
        .mosiPin = CONFIG_PIN_SPI_MOSI,
        .misoPin = CONFIG_PIN_SPI_MISO,
        .clkPin  = CONFIG_PIN_SPI_SCLK,
        .csnPin  = GPIO_INVALID_INDEX
    },
};

/*
 *  ======== SPI_config ========
 */
const SPI_Config SPI_config[CONFIG_SPI_COUNT] = {
    /* CONFIG_SPI_0 */
    /* LaunchPad SPI Bus */
    {
        .fxnTablePtr = &SPICC26X2DMA_fxnTable,
        .object = &spiCC26X2DMAObjects[CONFIG_SPI_0],
        .hwAttrs = &spiCC26X2DMAHWAttrs[CONFIG_SPI_0]
    },
};

const uint_least8_t CONFIG_SPI_0_CONST = CONFIG_SPI_0;
const uint_least8_t SPI_count = CONFIG_SPI_COUNT;

/*
 *  =============================== TRNG ===============================
 */

#include <ti/drivers/TRNG.h>
#include <ti/drivers/trng/TRNGCC26XX.h>

#define CONFIG_TRNG_COUNT 1


TRNGCC26XX_Object trngCC26XXObjects[CONFIG_TRNG_COUNT];

/*
 *  ======== trngCC26XXHWAttrs ========
 */
static const TRNGCC26XX_HWAttrs trngCC26XXHWAttrs[CONFIG_TRNG_COUNT] = {
    {
        .intPriority = (~0),
        .swiPriority = 0,
        .samplesPerCycle = 240000
    },
};

const TRNG_Config TRNG_config[CONFIG_TRNG_COUNT] = {
    {   /* CONFIG_TRNG_0 */
        .object         = &trngCC26XXObjects[CONFIG_TRNG_0],
        .hwAttrs        = &trngCC26XXHWAttrs[CONFIG_TRNG_0]
    },
};

const uint_least8_t CONFIG_TRNG_0_CONST = CONFIG_TRNG_0;
const uint_least8_t TRNG_count = CONFIG_TRNG_COUNT;

/*
 *  =============================== UART2 ===============================
 */

#include <ti/drivers/UART2.h>
#include <ti/drivers/uart2/UART2CC26X2.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/dma/UDMACC26XX.h>
#include <ti/drivers/power/PowerCC26X2.h>
#include <ti/devices/cc13x2x7_cc26x2x7/driverlib/ioc.h>
#include <ti/devices/cc13x2x7_cc26x2x7/inc/hw_memmap.h>
#include <ti/devices/cc13x2x7_cc26x2x7/inc/hw_ints.h>

#define CONFIG_UART2_COUNT 1

UART2CC26X2_Object uart2CC26X2Objects[CONFIG_UART2_COUNT];

static unsigned char uart2RxRingBuffer0[32];
/* TX ring buffer allocated to be used for nonblocking mode */
static unsigned char uart2TxRingBuffer0[32];

ALLOCATE_CONTROL_TABLE_ENTRY(dmaUart1RxControlTableEntry, UDMA_CHAN_UART1_RX);
ALLOCATE_CONTROL_TABLE_ENTRY(dmaUart1TxControlTableEntry, UDMA_CHAN_UART1_TX);

static const UART2CC26X2_HWAttrs uart2CC26X2HWAttrs[CONFIG_UART2_COUNT] = {
  {
    .baseAddr           = UART1_BASE,
    .intNum             = INT_UART1_COMB,
    .intPriority        = (~0),
    .rxPin              = CONFIG_GPIO_DISPLAY_UART_RX,
    .txPin              = CONFIG_GPIO_DISPLAY_UART_TX,
    .ctsPin             = GPIO_INVALID_INDEX,
    .rtsPin             = GPIO_INVALID_INDEX,
    .flowControl        = UART2_FLOWCTRL_NONE,
    .powerId            = PowerCC26XX_PERIPH_UART1,
    .rxBufPtr           = uart2RxRingBuffer0,
    .rxBufSize          = sizeof(uart2RxRingBuffer0),
    .txBufPtr           = uart2TxRingBuffer0,
    .txBufSize          = sizeof(uart2TxRingBuffer0),
    .txPinMux           = IOC_PORT_MCU_UART1_TX,
    .rxPinMux           = IOC_PORT_MCU_UART1_RX,
    .ctsPinMux          = IOC_PORT_MCU_UART1_CTS,
    .rtsPinMux          = IOC_PORT_MCU_UART1_RTS,
    .dmaTxTableEntryPri = &dmaUart1TxControlTableEntry,
    .dmaRxTableEntryPri = &dmaUart1RxControlTableEntry,
    .rxChannelMask      = 1 << UDMA_CHAN_UART1_RX,
    .txChannelMask      = 1 << UDMA_CHAN_UART1_TX,
    .txIntFifoThr       = UART2CC26X2_FIFO_THRESHOLD_1_8,
    .rxIntFifoThr       = UART2CC26X2_FIFO_THRESHOLD_4_8
  },
};

const UART2_Config UART2_config[CONFIG_UART2_COUNT] = {
    {   /* CONFIG_DISPLAY_UART */
        .object      = &uart2CC26X2Objects[CONFIG_DISPLAY_UART],
        .hwAttrs     = &uart2CC26X2HWAttrs[CONFIG_DISPLAY_UART]
    },
};

const uint_least8_t CONFIG_DISPLAY_UART_CONST = CONFIG_DISPLAY_UART;
const uint_least8_t UART2_count = CONFIG_UART2_COUNT;


/*
 *  =============================== Button ===============================
 */
#include <ti/drivers/apps/Button.h>

#define CONFIG_BUTTON_COUNT 2
Button_Object ButtonObjects[CONFIG_BUTTON_COUNT];

static const Button_HWAttrs ButtonHWAttrs[CONFIG_BUTTON_COUNT] = {
    /* CONFIG_BTN_LEFT */
    /* LaunchPad Button BTN-1 (Left) */
    {
        .gpioIndex = CONFIG_GPIO_BTN1,
        .pullMode = Button_PULL_UP,
        .internalPullEnabled = 1,
    },
    /* CONFIG_BTN_RIGHT */
    /* LaunchPad Button BTN-2 (Right) */
    {
        .gpioIndex = CONFIG_GPIO_BTN2,
        .pullMode = Button_PULL_UP,
        .internalPullEnabled = 1,
    },
};

const Button_Config Button_config[CONFIG_BUTTON_COUNT] = {
    /* CONFIG_BTN_LEFT */
    /* LaunchPad Button BTN-1 (Left) */
    {
        .object = &ButtonObjects[CONFIG_BTN_LEFT],
        .hwAttrs = &ButtonHWAttrs[CONFIG_BTN_LEFT]
    },
    /* CONFIG_BTN_RIGHT */
    /* LaunchPad Button BTN-2 (Right) */
    {
        .object = &ButtonObjects[CONFIG_BTN_RIGHT],
        .hwAttrs = &ButtonHWAttrs[CONFIG_BTN_RIGHT]
    },
};

const uint_least8_t CONFIG_BTN_LEFT_CONST = CONFIG_BTN_LEFT;
const uint_least8_t CONFIG_BTN_RIGHT_CONST = CONFIG_BTN_RIGHT;
const uint_least8_t Button_count = CONFIG_BUTTON_COUNT;

/*
 *  =============================== LED ===============================
 */
#include <ti/drivers/apps/LED.h>

#define CONFIG_LED_COUNT 2
LED_Object LEDObjects[CONFIG_LED_COUNT];

static const LED_HWAttrs LEDHWAttrs[CONFIG_LED_COUNT] = {
    /* CONFIG_LED_RED */
    /* LaunchPad LED Red */
    {
        .type = LED_GPIO_CONTROLLED,
        .index = CONFIG_GPIO_RLED,
    },
    /* CONFIG_LED_GREEN */
    /* LaunchPad LED Green */
    {
        .type = LED_GPIO_CONTROLLED,
        .index = CONFIG_GPIO_GLED,
    },
};

const LED_Config LED_config[CONFIG_LED_COUNT] = {
    /* CONFIG_LED_RED */
    /* LaunchPad LED Red */
    {
        .object = &LEDObjects[CONFIG_LED_RED],
        .hwAttrs = &LEDHWAttrs[CONFIG_LED_RED]
    },
    /* CONFIG_LED_GREEN */
    /* LaunchPad LED Green */
    {
        .object = &LEDObjects[CONFIG_LED_GREEN],
        .hwAttrs = &LEDHWAttrs[CONFIG_LED_GREEN]
    },
};

const uint_least8_t CONFIG_LED_RED_CONST = CONFIG_LED_RED;
const uint_least8_t CONFIG_LED_GREEN_CONST = CONFIG_LED_GREEN;
const uint_least8_t LED_count = CONFIG_LED_COUNT;

#include <stdbool.h>

#include <ti/devices/cc13x2x7_cc26x2x7/driverlib/ioc.h>
#include <ti/devices/cc13x2x7_cc26x2x7/driverlib/cpu.h>

#include <ti/drivers/GPIO.h>

/* Board GPIO defines */
#define BOARD_EXT_FLASH_SPI_CS      20
#define BOARD_EXT_FLASH_SPI_CLK     10
#define BOARD_EXT_FLASH_SPI_MOSI    9
#define BOARD_EXT_FLASH_SPI_MISO    8


/*
 *  ======== Board_sendExtFlashByte ========
 */
void Board_sendExtFlashByte(uint8_t byte)
{
    uint8_t i;

    /* SPI Flash CS */
    GPIO_write(BOARD_EXT_FLASH_SPI_CS, 0);

    for (i = 0; i < 8; i++) {
        GPIO_write(BOARD_EXT_FLASH_SPI_CLK, 0); /* SPI Flash CLK */

        /* SPI Flash MOSI */
        GPIO_write(BOARD_EXT_FLASH_SPI_MOSI, (byte >> (7 - i)) & 0x01);
        GPIO_write(BOARD_EXT_FLASH_SPI_CLK, 1);  /* SPI Flash CLK */

        /*
         * Waste a few cycles to keep the CLK high for at
         * least 45% of the period.
         * 3 cycles per loop: 8 loops @ 48 Mhz = 0.5 us.
         */
        CPUdelay(8);
    }

    GPIO_write(BOARD_EXT_FLASH_SPI_CLK, 0);  /* CLK */
    GPIO_write(BOARD_EXT_FLASH_SPI_CS, 1);  /* CS */

    /*
     * Keep CS high at least 40 us
     * 3 cycles per loop: 700 loops @ 48 Mhz ~= 44 us
     */
    CPUdelay(700);
}

/*
 *  ======== Board_wakeUpExtFlash ========
 */
void Board_wakeUpExtFlash(void)
{
    /* SPI Flash CS*/
    GPIO_setConfig(BOARD_EXT_FLASH_SPI_CS, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_HIGH | GPIO_CFG_OUT_STR_MED);

    /*
     *  To wake up we need to toggle the chip select at
     *  least 20 ns and ten wait at least 35 us.
     */

    /* Toggle chip select for ~20ns to wake ext. flash */
    GPIO_write(BOARD_EXT_FLASH_SPI_CS, 0);
    /* 3 cycles per loop: 1 loop @ 48 Mhz ~= 62 ns */
    CPUdelay(1);
    GPIO_write(BOARD_EXT_FLASH_SPI_CS, 1);
    /* 3 cycles per loop: 560 loops @ 48 Mhz ~= 35 us */
    CPUdelay(560);
}

/*
 *  ======== Board_shutDownExtFlash ========
 */
void Board_shutDownExtFlash(void)
{
    /*
     *  To be sure we are putting the flash into sleep and not waking it,
     *  we first have to make a wake up call
     */
    Board_wakeUpExtFlash();

    /* SPI Flash CS*/
    GPIO_setConfig(BOARD_EXT_FLASH_SPI_CS, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_HIGH | GPIO_CFG_OUT_STR_MED);
    /* SPI Flash CLK */
    GPIO_setConfig(BOARD_EXT_FLASH_SPI_CLK, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_LOW | GPIO_CFG_OUT_STR_MED);
    /* SPI Flash MOSI */
    GPIO_setConfig(BOARD_EXT_FLASH_SPI_MOSI, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_LOW | GPIO_CFG_OUT_STR_MED);
    /* SPI Flash MISO */
    GPIO_setConfig(BOARD_EXT_FLASH_SPI_MISO, GPIO_CFG_IN_PD);

    uint8_t extFlashShutdown = 0xB9;

    Board_sendExtFlashByte(extFlashShutdown);

    GPIO_resetConfig(BOARD_EXT_FLASH_SPI_CS);
    GPIO_resetConfig(BOARD_EXT_FLASH_SPI_CLK);
    GPIO_resetConfig(BOARD_EXT_FLASH_SPI_MOSI);
    GPIO_resetConfig(BOARD_EXT_FLASH_SPI_MISO);
}


#include <ti/drivers/Board.h>

/*
 *  ======== Board_initHook ========
 *  Perform any board-specific initialization needed at startup.  This
 *  function is declared weak to allow applications to override it if needed.
 */
void __attribute__((weak)) Board_initHook(void)
{
}

/*
 *  ======== Board_init ========
 *  Perform any initialization needed before using any board APIs
 */
void Board_init(void)
{
    /* ==== /ti/drivers/Power initialization ==== */
    Power_init();

    /* ==== /ti/devices/CCFG initialization ==== */

    /* ==== /ti/drivers/GPIO initialization ==== */
    /* Setup GPIO module and default-initialise pins */
    GPIO_init();

    /* ==== /ti/drivers/RF initialization ==== */

    Board_shutDownExtFlash();

    Board_initHook();
}

