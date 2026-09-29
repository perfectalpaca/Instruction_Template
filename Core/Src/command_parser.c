
#include "main.h"
#include <string.h>

extern UART_HandleTypeDef huart1;
#define COMMAND_SIZE 64
uint8_t msg[] = "Hello from STM32!\r\n";

char command[COMMAND_SIZE];
char receivedCommand[COMMAND_SIZE];
uint8_t commandIndex = 0;
uint8_t clear[] = "\033[2J\033[H";
uint8_t verificationMsg[] = "Command Received. Command is:\r\n";
uint8_t overflowMsg[] = "Overflow reached, resetting string\r\n";


//clears terminal
void Clear_Terminal(void){

	HAL_UART_Transmit(&huart1, clear, sizeof(clear) - 1, HAL_MAX_DELAY);

}

//Functions similarly to the previous byte processor, taking / and ? as special characters to clear and submit a command, respectively
void Process_Byte(uint8_t rxMsg){
	  if(rxMsg == '/'){
		  commandIndex = 0;
		  command[0] = '\0';
	  }
	  // Submit the command to receiveCommand, then clear command for a new word
	  else if(rxMsg == '?'){
		  strcpy(receivedCommand, command);
		  command[0] = '\0';
	  }
	  else{


		  if(commandIndex < COMMAND_SIZE - 1){
			  command[commandIndex] = rxMsg;
			  commandIndex++;
			  command[commandIndex] = '\0';
		  }
		  else{
		        HAL_UART_Transmit(
		            &huart1,
		            overflowMsg,
		            sizeof(overflowMsg) - 1,
		            HAL_MAX_DELAY
		        );

		        commandIndex = 0;
		        command[0] = '\0';
		  }
	  }
	  // Print the command
	  if(rxMsg == '?'){

		  HAL_UART_Transmit(&huart1, verificationMsg, sizeof(verificationMsg) - 1, HAL_MAX_DELAY);
		  HAL_UART_Transmit(&huart1, (uint8_t *)receivedCommand, commandIndex, HAL_MAX_DELAY);

		  commandIndex = 0;
	  }
	  // If the string is above a limit, set the index to 0 to prevent overflow and accessing undefined indices


	  else{
		  HAL_UART_Transmit(&huart1, (uint8_t *)command, commandIndex, HAL_MAX_DELAY);
	  }

}
