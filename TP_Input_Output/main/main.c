/****************************************************************************
 * Copyright (C) 2021 by Fabrice Muller                                     *
 *                                                                          *
 * This file is useful for ESP32 Design course.                             *
 *                                                                          *
 ****************************************************************************/

/**
 * @file main.c
 * @author Fabrice Muller
 * @date 31 Oct. 2021
 * @brief File containing the lab1 of Part 4.
 *
 * @see https://github.com/fmuller-pns/esp32-vscode-project-template
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* FreeRTOS.org includes. */
#include "freertos/FreeRTOSConfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_timer.h"
#include "esp_sleep.h"

#include "driver/gpio.h"
#include "driver/dac.h"


/* 
--------------------
Digital to Analog Converter (DAC)

Documentation:
https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/dac.html


--------------------
High Resolution Timer (esp_timer)
 
Documentation:
https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/esp_timer.html

*/

/* 
 * Global variable
 */


uint32_t timeUs;

esp_timer_cb_t MyFunction_cb ();

/**
 * @brief Starting point function
 * 
 */



void app_main(void) {

	/**************************************************/
	/* Configure DAC (Digital to Analog Converter)    */

	/* DAC_CHANNEL_1 = GPIO25 (IO25) */ 
	dac_output_enable(DAC_CHANNEL_1);

	/* Set Value */
	dac_output_voltage(DAC_CHANNEL_1, 0);

	/**************************************************/
	/* Configure Timer                                */
    const esp_timer_create_args_t esp_timer_create_args = {
        .callback = MyFunction_cb,
        .name = "MyFctName"
		};
    esp_timer_handle_t esp_timer_handle;
    esp_timer_create(&esp_timer_create_args, &esp_timer_handle);

	/* Start timer  */
	timeUs = 9804;
    esp_timer_start_periodic(esp_timer_handle, timeUs);

	/* Display timer information */
	esp_timer_dump(stdout);

	vTaskDelay(1500);    // 5000 ms

	/* Stop Timer */
	esp_timer_stop(esp_timer_handle);

	/* Re-Start timer  */
	timeUs = 0.5/255*1000000;
    esp_timer_start_periodic(esp_timer_handle, timeUs);

	vTaskDelay(1500);    // 5000 ms

	/* Stop Timer */
	esp_timer_stop(esp_timer_handle);

	/* Delete Timer */
	esp_timer_delete(esp_timer_handle);

	/* to ensure its exit is clean */
	vTaskDelete(NULL);
}

esp_timer_cb_t MyFunction_cb () {
	static int output = 0;
	static int incr = 0;
	printf("incr : %d\n",incr);
	if (incr == 0) {
		output++;
		dac_output_voltage(DAC_CHANNEL_1, output);
		printf("%d\n",output);
	}
	else {
		output--;
		dac_output_voltage(DAC_CHANNEL_1, output);
	}

	if (output == 255) {
		incr =1;
	};
	if(output == 0){
		incr =0;
	};

	return 0;
}