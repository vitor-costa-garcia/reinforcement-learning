## *Exercise 2.1*
Since E = 0.5, the probability of choosing the optimal action instead of choosing an action randomly is 1 - E = 0.5
In the case of two actions, the probability of choosing the optimal one randomly is 0.5
The probability to choose the optimal action in a E-greedy algorithm in given by the probability of choosing
the optimal action + the chance of choosing an action randomly * the probability of choosing the optimal action randomly
which equals 0.5 + (0.5 * 0.5) = 0.75

## *Exercise 2.2*
When checking each timestep individually:
* t = 1 - POSSIBLY EXPLORATORY
All action value estimates are zero, so there's no way to know if the action taken by the method was greedy or exploratory, since
all the estimates are equal.
The action chosen was A1 = 1.
The reward was R1 = -1.

* t = 2 - POSSIBLY EXPLORATORY
All action value estimates are zero, except action 1, which action value estimate is -1.
The action chosen was A2 = 2, which is one of the three actions with equal highest action value estimates
The reward was R2 = 1.

* t = 3 - GREEDY
The highest action value estimate is action 2.
The action chosen was A3 = 2, which is the action with highest action value estimate.
The reward was R3 = -2.

* t = 4 - EXPLORATORY
The highest action value estimates are action 3 and 4.
The action chosen was A4 = 2, which isn't the one with highest action value estimate.
The reward was R4 = 2.

* t = 5 - EXPLORATORY
The highest action value estimate is action action 2.
The action chosen was A5 = 3, which isn't the one with highest action value estimate.
The reward was R5 = 0.

So we can conclude that E case happened for sure at t = 4 and t = 5, and possibly happened at t = 1 and t = 2.