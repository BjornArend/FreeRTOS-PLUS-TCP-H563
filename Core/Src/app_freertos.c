/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os2.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS_Sockets.h"
#include "FreeRTOS_IP.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 512 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
  /* creation of defaultTask */


  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}
/* USER CODE BEGIN Header_StartDefaultTask */
/**
* @brief Function implementing the defaultTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN defaultTask */
  /* Infinite loop */

  for(;;)
  {

	  Socket_t xSocket;
	  struct freertos_sockaddr xRemoteAddress;
	  BaseType_t xAlreadyTransmitted = 0, xBytesSent = 0;
	  size_t xLenToSend;
	  char xDataToSend [ 6 ] = "hello!";


	  /* Set the IP address (192.168.0.200) and port (1500) of the remote socket
	  to which this client socket will transmit. */
	  xRemoteAddress.sin_port = FreeRTOS_htons( 12345 );
	  xRemoteAddress.sin_addr = FreeRTOS_inet_addr_quick( 192, 168, 0, 235 );

	  /* Create a socket. */
	  xSocket = FreeRTOS_socket(FREERTOS_AF_INET,
			  	  	  	  	  	FREERTOS_SOCK_STREAM, /* FREERTOS_SOCK_STREAM for TCP. */
	  		                    FREERTOS_IPPROTO_TCP );


	  /* Connect to the remote socket.  The socket has not previously been bound to
	  a local port number so will get automatically bound to a local port inside
	  the FreeRTOS_connect() function. */
	  if( FreeRTOS_connect( xSocket, &xRemoteAddress, sizeof( xRemoteAddress ) ) == 0 )
	  {
	  		/* Keep sending until the entire buffer has been sent. */
	  		while( xAlreadyTransmitted < 6 )
	  		{
	  		      /* How many bytes are left to send? */
	  		      xLenToSend = 6 - xAlreadyTransmitted;
	  		      xBytesSent = FreeRTOS_send( /* The socket being sent to. */
	  		                                 xSocket,
	  		                                 /* The data being sent. */
	  		                                 &( xDataToSend[ xAlreadyTransmitted ] ),
	  		                                 /* The remaining length of data to send. */
	  		                                 xLenToSend,
	  		                                 /* ulFlags. */
	  		                                 0);

	  		     if( xBytesSent >= 0 )
	  		     {
	  		           /* Data was sent successfully. */
	  		           xAlreadyTransmitted += xBytesSent;
	  		     }
	  		     else
	  		     {
	  		          /* Error - break out of the loop for graceful socket close. */
	  		          break;
	  		     }
	  		}
	  }
	  /* The socket has shut down and is safe to close. */
	  FreeRTOS_closesocket( xSocket );
  }
  /* USER CODE END defaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void vApplicationIPNetworkEventHook( eIPCallbackEvent_t eNetworkEvent )
{
uint32_t ulIPAddress, ulNetMask, ulGatewayAddress, ulDNSServerAddress;
static BaseType_t xTasksAlreadyCreated = pdFALSE;
char cBuffer[ 16 ];

    /* Check this was a network up event, as opposed to a network down event. */
    if( eNetworkEvent == eNetworkUp )
    {

    	/* Create the tasks that use the TCP/IP stack if they have not already been
        created. */
        if( xTasksAlreadyCreated == pdFALSE )
        {
            /*
             * Create the tasks here.
             */
        	defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);
            xTasksAlreadyCreated = pdTRUE;
        }

        /* The network is up and configured.  Print out the configuration,
        which may have been obtained from a DHCP server. */
        FreeRTOS_GetAddressConfiguration( &ulIPAddress,
                                          &ulNetMask,
                                          &ulGatewayAddress,
                                          &ulDNSServerAddress );

        /* Convert the IP address to a string then print it out. */
        FreeRTOS_inet_ntoa( ulIPAddress, cBuffer );
        printf( "IP Address: %srn", cBuffer );

        /* Convert the net mask to a string then print it out. */
        FreeRTOS_inet_ntoa( ulNetMask, cBuffer );
        printf( "Subnet Mask: %srn", cBuffer );

        /* Convert the IP address of the gateway to a string then print it out. */
        FreeRTOS_inet_ntoa( ulGatewayAddress, cBuffer );
        printf( "Gateway IP Address: %srn", cBuffer );

        /* Convert the IP address of the DNS server to a string then print it out. */
        FreeRTOS_inet_ntoa( ulDNSServerAddress, cBuffer );
        printf( "DNS server IP Address: %srn", cBuffer );
    }
}
/* USER CODE END Application */

