#  General description
bla bla bla lba

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
The command is passed via emulated serial interface (visible as COMx in Windows). The port has description “STMicroelectronics Virtual COM”. The baud rate is autodetected by STM kit. Recommended is 9600.

## Enable pin
It set pin to logical 1
Command: `Enable x[, n]\r`
x stands for symbolic name of pin.

example: `Enable 1, 2\r`  Sets  PC7 and PC8 to logical 1

## Disable pin
It set pin to logical 0
Command: `Disable x[, n]\r`
x stands for symbolic name of pin.

example: `Disable 1, 2\r`  sets  PC7 and PC8 to logical 0

## Get state of pins - binary
It returns state of all pins. 
Command: `Report binary\r`
Response: `Channels state 0xZZ`
Where 0xZZ is hex value with binary encoded state of all pins. Bit0 = symbolic name 0

Example of response: `Channels state 0xf9`
PC7, PC8 is enabled. All others are disabled.

## Get state of pins
It returns state of all pins. 
Command: `Report\r`
Response: `Enabled channels x[,n]\r`
x stands for symbolic name of pin.

Example of output: `Enabled channels 1, 2\r`
PC7, PC8 is enabled. All others are disabled.

## Change configuration of PIN
Change configuration of single pin.
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
Set PA5 as input, either of push-up/down is not active

