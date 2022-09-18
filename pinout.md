RJ50 connector: 10 conductors + shield

1: Unconnected to remote ??
2: +5V always
3: Unconnected to remote ??
4: +5V when remote display is off ??
5: Connect GND to move desk down, not connected through remote
6: Connect GND to move desk up, not connected through remote
7: GND
8: RX
9: TX
10: ??

Eq RJ 45:

1: +5V
2: -
3: -
4: - 
5: -
6: GND
7: RX -> Rx2 -> GPIO16
8: TX -> TX2 -> GPIO17


Next step:

connect unknown pins to logic analyzer, GND to GND, and try to decode the (assuming serial UART) signal.

One bit looks like 103us, or 9.71k Hz == 9710 baud?

Decoding with 9600 baud:

Pressed down then released, desk ended up at 27.7:

TX: D8 D8 78 80 80 <- Initialization?

> RX: 18 E6 02 00 ; 00 00 00 00 ; 00 00 98 98 ; 00 00 4A 4A
TX: D8 D8 78 ; 80 80



> RX: 99 40 01 4A 24                                                         <- I think this is the desk hight somehow. It ranged from 22140 to 5178 for 49.0in to 22.9in
> RX: 18 E6 02 00 ; 00 00 00 00 ; 00 00 98 98 ; 00 00 4A 4A
TX: D8 D8 78 ; 01 01                                                         <- TX changed


> RX: 99 40 01 4A 24
> RX: 18 E6 02 00 ; 00 00 00 00 ; 00 00 98 98 ; 00 00 4A 4A
TX: D8 D8 78 ; 01 01


> RX: 99 40 01 4A 24
> RX: 18 E6 02 00 ; 00 00 00 00 ; 00 00 98 98 ; 00 00 4A 4A
TX: D8 D8 78 ; 01 01


> RX: 99 41 01 4A 25                                                        <- RX changed
> RX: 18 E6 02 00 ; 00 00 00 00 ; 00 00 98 98 ; 03 03 4A 4A                 <- RX changed
TX: D8 D8 78 ; 01 01


> RX: 99 41 01 4A 25
> RX: 18 E6 02 00 ; 00 00 00 00 ; 00 00 98 98 ; 03 03 4A 4A
TX: D8 D8 78 ; 01 01


....

> RX: 99 41 01 49 24
> RX: 21 DD 02 00 ; 00 00 00 00 ; 00 00 98 98 ; 03 03 49 49
TX: D8 D8 78 ; 01 01




AT TOP: 99 40 01 7C 56
AT BOT: 99 40 01 3A 14

99 (State) 01 (Height)

State:
40: Stopped
41: Moving Down
42: Moving Up

Height is int16, little endian. Ranges 5178 (22.9 in) to 22140 (49 in)


WAKE: TX: D8 D8 78 80 80

HELD DOWN: TX: D8 D8 78 ; 01 01
HELD UP:   TX: D8 D8 78 ; 02 02
NO BUTTON: TX: D8 D8 78 ; 00 00

MEM 1: TX: D8 D8 78 ; 04 04
MEM 2: TX: D8 D8 78 ; 08 08
MEM 3: TX: D8 D8 78 ; 10 10
MEM 4: TX: D8 D8 78 ; 20 20




READ by esp32:

D8:D8:78:80:80:D8:D8:78:08:08

D8:D8:78:08:08:D8:D8:78:08:08:D8:D8:78:08:08:D8:D8:78:08:08:D8:D8:78:08:08:D8:D8:78:08:08:D8:D8:78:08:08:D8:D8:78:08:08:D8:D8:78:08:08:D8:D8:78:00:00

D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00

D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00:D8:D8:78:00:00