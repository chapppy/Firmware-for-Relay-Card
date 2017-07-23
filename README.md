#  IO controller
This project allows to control GPIO of STM32 microntroller with simple comands send via USB (Virtual Com Port). The project is developed on STM32F072RBT6 microcontroller. The SMT32 library is configured by [STM32Cube Embedded Software ](http://www.st.com/en/embedded-software/stm32cube-embedded-software.html "STM32Cube Embedded Software ") whitch ensure simple portability to another STM32 microcontrollers. The functionality was tested on [32F072BDISCOVERY](http://www.st.com/en/evaluation-tools/32f072bdiscovery.html "32F072BDISCOVERY"). This project can be used for switch of external tranzistors, LEDs, relays or reading of logical signal on GPIO of microcontroller. You can see the example of use in demo application (GUI in QT for control of relays  [IO-Controller-GUI](https://github.com/chapppy/IO-Controller-GUI "IO-Controller-GUI"))

# PINout
| STM32  pin  | Symbolic name  | Init state |
| :------------: | :------------: | :------------: |
| PC6 | 0 | - |
| PC7 | 1 | - |
| PC8 | 2 | - |
| PA5 | 3 | OUT, push-pull, pull down |
| PB3 | 4 | IN, no pullup |
| PB4 | 5 | IN, no pullup |
| PB5 | 6 | IN, no pullup |
| PB6 | 7 | IN, no pullup |

# Command set
The command is passed via emulated serial interface (visible as COMx in Windows). The port has description “STMicroelectronics Virtual COM”. The baud rate is autodetected by STM32 microcontroller. Recommended is 9600.

## Enable pin
It sets pin to logical 1
Command: `Enable x[, n]\r`
x stands for symbolic name of pin.

example: `Enable 1, 2\r`  Sets  PC7 and PC8 to logical 1

## Disable pin
It sets pin to logical 0
Command: `Disable x[, n]\r`
x stands for symbolic name of pin.

example: `Disable 1, 2\r`  sets  PC7 and PC8 to logical 0

## Get state of pins - binary
It returns state of all pins. 
Command: `Report binary\r`
Response: `Channels state 0xZZ`
Where 0xZZ is hex value with binary encoded state of all pins. Bit0 = symbolic name 0

Example of response: `Channels state 0xf9`
1111 1001b means that PC7, PC8 is enabled. All others are disabled.

## Get state of pins
It returns state of all pins. 
Command: `Report\r`
Response: `Enabled channels x[,n]\r`
x stands for symbolic name of pin.

Example of output: `Enabled channels 1, 2\r`
PC7, PC8 is enabled. All others are disabled.

## Change configuration of PIN
It change configuration of single pin.
Command: `Configure x, MOD0, MOD1\r`
x stand for symbolic name of pin. MODx see in tables below. 

### MOD0
| MOD0 ||
| :------------: | :------------: |
| Option |  Meaning |
| IN | Set pin as input |
| OUTOD | Set pins as output open drain |

### MOD1
| MOD1 ||
| :------------: | :------------: |
| Option | Meaning |
| PPNN | Set pin to no push-pull |
| PPUP | Set pin to pull up |
| PPDOWN | Set pin to push-pull |


Example: `Configure 3, OUTPP, PPDOWN\r`
Set PA5 as output pin with push-pull, the state is active pull-down
Example: `Configure 3, IN, PPNO\r`
Set PA5 as input, any of push-up/down is not active

