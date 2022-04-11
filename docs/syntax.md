# Syntax

This project uses the syntax from Turing which he used in his 1936 paper "ON COMPUTABLE NUMBERS, WITH AN APPLICATION TO
THE ENTSCHEIDUNGSPROBLEM".

Each transition is desribed as ![transition-formula](https://render.githubusercontent.com/render/math?math=%5Cdelta%28q_i%2CS_i%29%3D%28S_%7Bij%7D%2Cd_%7Bij%7D%2Cq_%7Bij%7D%29)
where:
* ![q-i](https://render.githubusercontent.com/render/math?math=q_i) is the current state of the turing machine,
* ![S-i](https://render.githubusercontent.com/render/math?math=S_i) is the current symbol on the tape,
* ![S-ij](https://render.githubusercontent.com/render/math?math=S_%7Bij%7D) is the new symbol to be written on the
tape,
* ![d-ij](https://render.githubusercontent.com/render/math?math=d_%7Bij%7D) is the the direction for the tape head to
move and
* ![q-ij](https://render.githubusercontent.com/render/math?math=q_%7Bij%7D) is the new state of the turing machine
after the transition.

The transition ![transition-formula](https://render.githubusercontent.com/render/math?math=%5Cdelta%28q_1%2CS_0%29%3D%28S_0%2CR%2Cq_2%29)
can also be written as a quinttuple by using ![transition-quintuple](https://render.githubusercontent.com/render/math?math=%28q_i%2CS_i%2CS_%7Bij%7D%2Cd_%7Bij%7D%2Cq_%7Bij%7D%29).
In Turing's paper, he encodes these quintuples using the following scheme:
* any state ![q-n](https://render.githubusercontent.com/render/math?math=q_n) is encoded by writing `DAA...A`, where
`A` is repeated ![n](https://render.githubusercontent.com/render/math?math=n) times
* any symbol ![S-n](https://render.githubusercontent.com/render/math?math=S_n) is encoded by writing `DCC...C`, where
`C` is repeated ![n](https://render.githubusercontent.com/render/math?math=n) times
* the movement of the tape head is either `R` (right) or `L` (left)

For the transition ![transition-quintuple](https://render.githubusercontent.com/render/math?math=%28q_1%2CS_0%2CS_0%2CR%2Cq_2%29)
this encoding results in `DADDRDAA`. Each transition will then be separated by a `;` symbol.

## Example

Let's look at a simple turing machine which writes alternating ![S-0](https://render.githubusercontent.com/render/math?math=S_0)
and ![S-1](https://render.githubusercontent.com/render/math?math=S_1). The required transitions are given by the
following table:
|   | ![S-0](https://render.githubusercontent.com/render/math?math=S_0) | ![S-1](https://render.githubusercontent.com/render/math?math=S_1) |
|---|---|---|
| ![q-1](https://render.githubusercontent.com/render/math?math=q_1) | ![S0 r q2](https://render.githubusercontent.com/render/math?math=%28S_0%2CR%2Cq_2%29) | ![S0 r q2](https://render.githubusercontent.com/render/math?math=%28S_0%2CR%2Cq_2%29) |
| ![q-2](https://render.githubusercontent.com/render/math?math=q_2) | ![S1 r q1](https://render.githubusercontent.com/render/math?math=%28S_1%2CR%2Cq_1%29) | ![S1 r q1](https://render.githubusercontent.com/render/math?math=%28S_1%2CR%2Cq_1%29) |

The encoding would result in 
```
DADDRDAA;DADCDRDAA;DAADDCRDA;DAADCDCRDA
```

## Extensions

### Tape initialization

Theoretically, each turing machine could initialize its own tape simply by prepending some amount of states which write
the next symbol onto the tape, move to the right and switch to the next state. After the tape has been written, the
turing machine would then need to move left until it reaches the first symbol on the tape. This requires a lot of
states to implement.

To simplify this initialization, you can pass a sequence of symbols to the application before the first transition.
These symbols will then automatically be placed on the tape and the head is placed to the first symbol.

Initializing the tape from the previous example with ![S0 S1 S2 S0 S1 S2](https://render.githubusercontent.com/render/math?math=S_0S_1S_2S_0S_1S_2)
results in

```
DDCDCCDDCDCC;DADDRDAA;DADCDRDAA;DAADDCRDA;DAADCDCRDA
```

__Note:__ if you do not want to initialize the tape, you have to prepend your transitions with a `;`, so that the
application knows that the tape should remain empty.

### States

The turing machine will always start in state ![q-1](https://render.githubusercontent.com/render/math?math=q_1) and 
only ![q-0](https://render.githubusercontent.com/render/math?math=q_0) is accepting.
