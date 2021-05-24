# Monte Carlo Tree Search

Monte Carlo Tree Search is an algorithm to find the best solution to a problem (usually a game) with a great number states.
It is used in Alpha Go, the first program to ever win against a human player at Go.

The algorithm itself is not that hard to understand and implement, and I show here a slightly improved version, to boost performances.

## Improvements
- Node and path closing: close fully explored path allow for more exploration
- Expanding rollout: keep the results of the explored path during the rollout phase, saving a ton in performances, but trading it for memory consumption
- Monte Carlo Graph Search: (WIP) Transform the tree in a graph, where similar moves with different paths are connected as a single node. Some nodes can thus have multiple parents.  
- Memory efficient Unexplored children: (WIP) store the unexplored children of a node as a binary mask, greatly reducing memory usage when the number of game states is no too high
- Multi thread exploration: (WIP) Uses multiple threads to explore the tree simultaneously 


## How to use
This program is plug and play: You just have to create a class to handle your game, which must inherit form the IGame_State class, and call the MCTS on your class. 
Done !

Two games are given as implementation examples:
- TicTacToe
- Connect 4

More to come
