#include "bsp_stream_buffer_interface.h"
#include "main.h"
#include "usbd_cdc_if.h"
#include "task.h"
#include "bsp_base_param.h"
#include "bsp_liquid_read.h"
#include "register_task.h"
#include "FreeRTOS.h"
#include "FreeRTOS_CLI.h"
// static void LL_USART_SendString(USART_TypeDef *USARTx, uint8_t *str, int16_t len)
// {
//     for (int16_t i = 0; i < len; i++)
//     {
//         while (!LL_USART_IsActiveFlag_TXE(USARTx));
//         LL_USART_TransmitData8(USARTx, *str);
//         str++;
//     }
// }

static void FreeRTOS_write(char *data, int16_t len)
{

    while (CDC_Transmit_FS((uint8_t*)data, len) == USBD_BUSY) {
        vTaskDelay(1);
    }
    // LL_USART_SendString(USART1, data, len);
    // vTaskDelay(1);
}

#define MAX_INPUT_LENGTH 50
#define MAX_OUTPUT_LENGTH 200

// del cmd
static BaseType_t prvDelCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    const char *pcFileName;
    BaseType_t xParameterStringLength;

    pcFileName = FreeRTOS_CLIGetParameter(
                        pcCommandString,
                        0,
                        &xParameterStringLength );
    snprintf( pcWriteBuffer, xWriteBufferLen,
              "Deleting file: %s\r\n", pcFileName );
    return pdFALSE;
}

static const CLI_Command_Definition_t xDelCommand =
{
    "del",
    "del <filename>: This cmd is test. It is useless\r\n",
    prvDelCommand,
    1
};
// save Config
static BaseType_t prvSaveConfigCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    const char *pcFileName;
    BaseType_t xParameterStringLength;

    pcFileName = FreeRTOS_CLIGetParameter(
                        pcCommandString,
                        0,
                        &xParameterStringLength );
    bsp_saveconfig();
    snprintf( pcWriteBuffer, xWriteBufferLen,
              "Saving config %s to Flash: %x \r\n", pcFileName,CONFIG_FLASH_ADDE);
    return pdFALSE;
}

static const CLI_Command_Definition_t xSaveConfigCommand =
{
    "saveconfig",
    "saveconfig: Saves the configuration, such as \"liquid level reference\"\r\n",
    prvSaveConfigCommand,
    1
};

// set liquid level base
static BaseType_t prvSetLiquidLvBase( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    const char *pcFileName;
    BaseType_t xParameterStringLength;

    pcFileName = FreeRTOS_CLIGetParameter(
                        pcCommandString,
                        0,
                        &xParameterStringLength );
    uint16_t liquid_value = get_liquid_value();
    bsp_set_liquid_level_base(liquid_value);
    snprintf( pcWriteBuffer, xWriteBufferLen,
              "prvSetLiquidLvBase is : %d\r\n", liquid_value );
    return pdFALSE;
}

static const CLI_Command_Definition_t xSetLiquidLvBaseCommand =
{
    "setlbase",
    "setlbase: set current liquid level as base reference\r\n",
    prvSetLiquidLvBase,
    1
};

// set liquid level max
static BaseType_t prvSetLiquidLvMAX( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    const char *pcFileName;
    BaseType_t xParameterStringLength;

    pcFileName = FreeRTOS_CLIGetParameter(
                        pcCommandString,
                        0,
                        &xParameterStringLength );
    uint16_t liquid_value = get_liquid_value();
    bsp_set_liquid_level_max(liquid_value);
    snprintf( pcWriteBuffer, xWriteBufferLen,
              "prvSetLiquidLvMAX is : %d\r\n", liquid_value );
    return pdFALSE;
}

static const CLI_Command_Definition_t xSetLiquidLvMAXCommand =
{
    "setlmax",
    "setlmax: set maximum liquid level as reference, Note: you should uplift the sensor from the cisterm and input this cmd\r\n",
    prvSetLiquidLvMAX,
    1
};

// get system health status
static BaseType_t prvGetSystemHealthStatus( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    const char *pcFileName;
    BaseType_t xParameterStringLength;

    pcFileName = FreeRTOS_CLIGetParameter(
                        pcCommandString,
                        1,
                        &xParameterStringLength );
    vTaskGetRunTimeStatistics(pcWriteBuffer, sizeof(pcWriteBuffer));
    // snprintf( pcWriteBuffer, xWriteBufferLen,
    //           "Saving config %s to Flash: %x \r\n", pcFileName,CONFIG_FLASH_ADDE);
    return pdFALSE;
}

static const CLI_Command_Definition_t xGetSystemHealthStatusCommand =
{
    "getsyshealth",
    "getsyshealth: Gets the system health status\r\n",
    prvGetSystemHealthStatus,
    1
};

// get system remain stack
static BaseType_t prvGetSystemRemainStack( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    const char *pcFileName;
    BaseType_t xParameterStringLength;

    pcFileName = FreeRTOS_CLIGetParameter(
                        pcCommandString,
                        0,
                        &xParameterStringLength );
    vTaskList(pcWriteBuffer);
    // snprintf( pcWriteBuffer, xWriteBufferLen,
    //           "Saving config %s to Flash: %x \r\n", pcFileName,CONFIG_FLASH_ADDE);
    return pdFALSE;
}

static const CLI_Command_Definition_t xGetSystemRemainStackCommand =
{
    "getsysstack",
    "getsysstack: Gets the system remain stack\r\n",
    prvGetSystemRemainStack,
    1
};

static const int8_t *const pcWelcomeMessage =
    "FreeRTOS command server.rnType Help to view a list of registered commands\r\n";

void vCLITask(void *pvParameters)
{
    StreamBufferHandle_t stream_buffer = bsp_get_stream_buffer();
    int8_t cRxedChar, cInputIndex = 0;
    BaseType_t xMoreDataToFollow;
    /* The input and output buffers are declared static to keep them off the stack. */
    static int8_t pcOutputString[MAX_OUTPUT_LENGTH], pcInputString[MAX_INPUT_LENGTH];

    /* Send a welcome message to the user knows they are connected. */
    FreeRTOS_CLIRegisterCommand( &xDelCommand );
    FreeRTOS_CLIRegisterCommand( &xSaveConfigCommand );
    FreeRTOS_CLIRegisterCommand( &xSetLiquidLvBaseCommand );
    FreeRTOS_CLIRegisterCommand( &xSetLiquidLvMAXCommand );
    FreeRTOS_CLIRegisterCommand( &xGetSystemHealthStatusCommand );
    FreeRTOS_CLIRegisterCommand( &xGetSystemRemainStackCommand );
    FreeRTOS_write(pcWelcomeMessage,strlen( pcWelcomeMessage));

    for (;;)
    {
        /* This implementation reads a single character at a time. Wait in the
           Blocked state until a character is received. */
        xStreamBufferReceive(stream_buffer,&cRxedChar,1,portMAX_DELAY);

        if (cRxedChar == '\n')
        {
            /* A newline character was received, so the input command string is
               complete and can be processed. Transmit a line separator, just to
               make the output easier to read. */
            FreeRTOS_write("\r\n",strlen( "\r\n" ));

            /* The command interpreter is called repeatedly until it returns
               pdFALSE. See the "Implementing a command" documentation for an
               exaplanation of why this is. */
            do
            {
                /* Send the command string to the command interpreter. Any
                   output generated by the command interpreter will be placed in the
                   pcOutputString buffer. */
                xMoreDataToFollow = FreeRTOS_CLIProcessCommand(
                    pcInputString,    /* The command string.*/
                    pcOutputString,   /* The output buffer. */
                    MAX_OUTPUT_LENGTH /* The size of the output buffer. */
                );

                /* Write the output generated by the command interpreter to the
                   console. */
                FreeRTOS_write(pcOutputString, strlen(pcOutputString));

            } while( xMoreDataToFollow != pdFALSE );

            /* All the strings generated by the input command have been sent.
               Processing of the command is complete. Clear the input string ready
               to receive the next command. */
            cInputIndex = 0;
            memset( pcInputString, 0x00, MAX_INPUT_LENGTH );
        }
        else
        {
            /* The if() clause performs the processing after a newline character
               is received. This else clause performs the processing if any other
               character is received. */

            if (cRxedChar == '\r')
            {
                /* Ignore carriage returns. */
            }
            else if (cRxedChar == '\b')
            {
                /* Backspace was pressed. Erase the last character in the input
                   buffer - if there are any. */
                if (cInputIndex > 0)
                {
                    cInputIndex--;
                    pcInputString[cInputIndex] = '\0';
                }
            }
            else
            {
                /* A character was entered. It was not a new line, backspace
                   or carriage return, so it is accepted as part of the input and
                   placed into the input buffer. When a n is entered the complete
                   string will be passed to the command interpreter. */
                if (cInputIndex < MAX_INPUT_LENGTH)
                {
                    pcInputString[cInputIndex] = cRxedChar;
                    cInputIndex++;
                }
            }
        }
    }
}

RTOS_TASK_REGISTER("CLITask",vCLITask, 330, 3);