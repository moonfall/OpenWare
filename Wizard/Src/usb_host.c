/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file            : usb_host.c
  * @version         : v1.0_Cube
  * @brief           : This file implements the USB Host
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

#include "usb_host.h"
#include "usbh_core.h"

/* USER CODE BEGIN Includes */
#include "usbh_midi.h"
#include "device.h"
#include "errorhandlers.h"
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#ifdef OWL_USBD_FS
#define HUSB_HOST hUsbHostHS
#define HUSB_HOST_HSFS HOST_HS
#else
#define HUSB_HOST hUsbHostFS
#define HUSB_HOST_HSFS HOST_FS
#endif
/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USB Host core handle declaration */
USBH_HandleTypeDef HUSB_HOST;
ApplicationTypeDef Appli_state = APPLICATION_IDLE;

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*
 * user callback declaration
 */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);

/*
 * -- Insert your external function declaration here --
 */
/* USER CODE BEGIN 1 */

/*
 * Background task
*/ 
void MX_USB_HOST_Process() 
{
  /* USB Host Background task */
  USBH_Process(&HUSB_HOST);
}

/* USER CODE END 1 */

/**
  * Init USB host library, add supported class and start the library
  * @retval None
  */
void MX_USB_HOST_Init(void)
{
  /* USER CODE BEGIN USB_HOST_Init_PreTreatment */
  
  /* USER CODE END USB_HOST_Init_PreTreatment */
  
  /* Init host Library, add supported class and start the library. */
  if (USBH_Init(&HUSB_HOST, USBH_UserProcess, HUSB_HOST_HSFS) != USBH_OK)
  {
    Error_Handler();
  }
  if (USBH_RegisterClass(&HUSB_HOST, USBH_MIDI_CLASS) != USBH_OK)
  {
    Error_Handler();
  }
  if (USBH_Start(&HUSB_HOST) != USBH_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_HOST_Init_PostTreatment */
  
  /* USER CODE END USB_HOST_Init_PostTreatment */
}

/*
 * user callback definition
 */
static void USBH_UserProcess  (USBH_HandleTypeDef *phost, uint8_t id)
{
  /* USER CODE BEGIN CALL_BACK_1 */
  switch(id)
  {
  case HOST_USER_SELECT_CONFIGURATION:
    break;

  case HOST_USER_DISCONNECTION:
    Appli_state = APPLICATION_DISCONNECT;
    midi_host_reset();
    break;

  case HOST_USER_CLASS_ACTIVE:
    if(Appli_state == APPLICATION_START){
      midi_host_begin();
      Appli_state = APPLICATION_READY;
    }
    break;

  case HOST_USER_CONNECTION:
    Appli_state = APPLICATION_START;
    break;

  case HOST_USER_UNRECOVERED_ERROR:
    midi_host_reset(); // reset and hope for the best
    error(USB_ERROR, "USB Host unrecovered error");
    break;

  default:
    break;
  }
  /* USER CODE END CALL_BACK_1 */
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
