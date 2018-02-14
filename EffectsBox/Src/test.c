#include <stdint.h>
#include "device.h"
#include "EffectsBox_Test.h"
#include "Flash_S25FL.h"
#include "HAL_OLED.h"

void setup(void);
void loop(void);

void setup(void){
  EffectsBox_Init();
}

void loop(void)
{
//		Flash_S25FL_Test();
		EffectsBox_Main();	
}
