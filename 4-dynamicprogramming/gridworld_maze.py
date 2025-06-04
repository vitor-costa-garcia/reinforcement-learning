import numpy as np

REWARDS_CELL = {0:-1, 1:-1, 2:-1, 3:5}
ACTIONS = {'N':(1, 0), 'S':(-1, 0), 'O':(0, -1), 'L':(0, 1)}

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
                        f_inicio == True
                    else:
                        raise AssertionError("Labirinto inválido! Deve-se colocar apenas um início.")
                    
                if maze[i][j] == 3:
                    if f_fim == False:
                        f_fim == True
                    else:
                        raise AssertionError("Labirinto inválido! Deve-se colocar apenas uma chegada.")
                    
        assert f_inicio and f_fim, "Labirinto inválido! Deve-se colocar um início e uma chegada."

        self.size = len(maze)
        self.n_cells = self.size**2
        self.maze = maze
        self.__policy = np.array()
        self.__state_values = np.zeros(self.size)

    def get_prob_policy(self, state, action):
        if action in self.__policy[state]:
            return 1/len(self.__policy)
    
        return 0
    
    def policy_eval(self, theta, gamma):
        v_old = self.__state_values
        while True:
            delta = 0
            for state in range(self.n_cells):
                v_old[state] = self.__state_values[state]
                v_new = 0
                for action in ACTIONS:
                    r, c = divmod(state, self.size)
                    r = min(max(0, r + ACTIONS[action][0]), self.size-1)
                    c = min(max(0, c + ACTIONS[action][1]), self.size-1)

                    next_s = r * self.size + c
                    v_new += self.get_prob_policy(state, action) * (REWARDS_CELL[self.maze[r][c]] + gamma * self.__state_values[next_s])
                self.__state_values[state] = v_new
                delta = max(delta, abs(v_old[state] - self.__state_values[state]))

            if delta >= theta:
                break

    def policy_improv(self, gamma):
        pol_stable = True
        old_pol = self.__policy
        for state in range(self.n_cells):
            action_values = []
            for action in ACTIONS:
                r, c = divmod(state, self.size)
                r = min(max(0, r + ACTIONS[action][0]), self.size-1)
                c = min(max(0, c + ACTIONS[action][1]), self.size-1)
                next_s = r * self.size + c

                action_values.append(self.get_prob_policy(state, action) * (REWARDS_CELL[self.maze[r][c]] + gamma * self.__state_values[next_s]))

            self.__policy = ['N','S','O','L'][np.argmax(action_values)]

        if old_pol != self.__policy:
            pol_stable = False
        
        return pol_stable

    def _policy_iter():
        pass