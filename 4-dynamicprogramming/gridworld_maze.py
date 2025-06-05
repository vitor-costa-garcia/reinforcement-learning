import numpy as np

REWARDS_CELL = {0:-1, 1:-1, 2:-1, 3:5}
ACTIONS = {'N':(-1, 0), 'S':(1, 0), 'O':(0, -1), 'L':(0, 1)}

class GridWorld_Maze:
    def __init__(self, maze):
        """
        Cria o objeto do labirinto. Deve-se colocar apenas um início e uma chegada.
        0 - Caminho livre
        1 - Parede
        2 - Início
        3 - Chegada
        """

        assert len(maze) == len(maze[0]), "Labirinto inválido! Precisa ser uma matriz quadrada."

        f_inicio, f_fim = False, False
        for i in range(len(maze)):
            for j in range(len(maze)):
                assert maze[i][j] in [0, 1, 2, 3], "Labirinto inválido! Valores devem ser 0, 1, 2, 3."
                
                if maze[i][j] == 2:
                    if f_inicio == False:
                        f_inicio = True
                    else:
                        raise AssertionError("Labirinto inválido! Deve-se colocar apenas um início.")
                    
                if maze[i][j] == 3:
                    if f_fim == False:
                        f_fim = True
                    else:
                        raise AssertionError("Labirinto inválido! Deve-se colocar apenas uma chegada.")
                    
        assert f_inicio and f_fim, "Labirinto inválido! Deve-se colocar um início e uma chegada."

        self.size = len(maze)
        self.n_cells = self.size**2
        self.maze = maze
        self.__policy = [['N','S','O','L'] for _ in range(self.n_cells)]
        self.__state_values = np.zeros(self.n_cells)

    def get_prob_policy(self, state, action):
        if action in self.__policy[state]:
            return 1/len(self.__policy[state])
    
        return 0
    
    def policy_eval(self, theta, gamma):
        v_old = self.__state_values.copy()
        while True:
            delta = 0
            for state in range(self.n_cells):
                r, c = divmod(state, self.size)
                if self.maze[r][c] == 3 or self.maze[r][c] == 1:
                    self.__state_values[state] = REWARDS_CELL[self.maze[r][c]]
                    continue

                v_new = 0
                for action in ACTIONS:
                    r_next = min(max(0, r + ACTIONS[action][0]), self.size-1)
                    c_next = min(max(0, c + ACTIONS[action][1]), self.size-1)

                    next_s = r_next * self.size + c_next
                    if self.maze[r_next][c_next] == 1:
                        next_s = state
                        r_next = r
                        c_next = c

                    v_new += self.get_prob_policy(state, action) * (REWARDS_CELL[self.maze[r_next][c_next]] + gamma * self.__state_values[next_s])
                self.__state_values[state] = v_new
                delta = max(delta, abs(v_old[state] - self.__state_values[state]))

            print(delta)
            if delta >= theta:
                break

    def policy_improv(self, gamma):
        pol_stable = True
        old_pol = [i.copy() for i in self.__policy]
        for state in range(self.n_cells):
            r, c = divmod(state, self.size)
            if self.maze[r][c] == 1 or self.maze[r][c] == 3:
                self.__policy[state] = []
                continue
            action_values = []
            for action in ACTIONS:
                r_next = min(max(0, r + ACTIONS[action][0]), self.size-1)
                c_next = min(max(0, c + ACTIONS[action][1]), self.size-1)
                next_s = r_next * self.size + c_next
                if self.maze[r_next][c_next] == 1:
                    next_s = state
                    r_next = r
                    c_next = c

                action_values.append(REWARDS_CELL[self.maze[r_next][c_next]] + gamma * self.__state_values[next_s])

            self.__policy[state] = [['N','S','O','L'][np.argmax(action_values)]]

        if old_pol != self.__policy:
            pol_stable = False
        
        return pol_stable

    def policy_iter(self,gamma):
        iterations = 0
        while True:
            iterations += 1
            self.policy_eval(1e-2, gamma)
            pol_stable = self.policy_improv(gamma)
            if pol_stable:
                print("Iterations: ", iterations)
                break

    def get_policy(self):
        return self.__policy
    
    def get_state_values(self):
        return self.__state_values

if __name__ == "__main__":
    maze_grid = [[2,1,0,0,0,1,3,0],
                 [0,1,0,1,1,1,1,0],
                 [0,0,0,1,0,0,1,0],
                 [0,1,0,1,0,1,1,0],
                 [1,1,0,1,0,0,0,0],
                 [0,0,0,0,0,1,0,1],
                 [1,1,1,1,1,1,0,1],
                 [1,0,0,0,0,0,0,0],]
    
    maze = GridWorld_Maze(maze_grid) # ←→↑↓
    
    maze.policy_iter(1)
    
    print("\nFinal Optimal Policy (Action for each state):")
    setas = {'N':'↑', 'S':'↓', 'L':'→', 'O':'←'}
    final_policy_display = np.array([setas[p[0]] if p else '-' for p in maze.get_policy()]).reshape(maze.size, maze.size)
    print(final_policy_display)
    
    print("\nFinal Optimal State Values:")
    print(np.array(maze.get_state_values()).reshape(maze.size, maze.size))
