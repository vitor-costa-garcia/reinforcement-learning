#Monte Carlo ES
import numpy as np
import random as rnd
from blackjack_env import BlackJackEnv, CARD_INDX
CARDS = ["Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Queen", "Jack", "King"]
CARDS_L = ["Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten"]
N_ACTIONS = 2

def mc_es(gamma, runs = 10000):
    policy = np.random.randint(0, 2, size=(10,10,2,2))

    for player_sum in range(10):
        for dealer_card in range(10):
            for usable_ace in range(2):
                policy[player_sum][dealer_card][usable_ace][0] = 1

    action_val = np.zeros(shape=(10,10,2,2))
    returns = [[[[[] for _ in range(2)] for _ in range(2)] for _ in range(10)] for _ in range(10)]

    bj = BlackJackEnv()    

    for _ in range(runs):
        episode = bj.run_episode(policy) #Returns a list of tuples (state, action, reward)
        g = 0
        seen_states = []
        for i in reversed(range(len(episode))):
            g = g * gamma + episode[i][2]
            state_action_pair = (episode[i][0], episode[i][1])

            act = episode[i][1]
            aces = episode[i][0]['usable_aces']
            dealer = CARD_INDX[episode[i][0]['dealer_card']]
            player = episode[i][0]['hand_sum'] - 12

            if state_action_pair not in seen_states:
                seen_states.append(state_action_pair)
                returns[player][dealer][aces][act].append(g)
                action_val[player][dealer][aces][act] = np.mean(returns[player][dealer][aces][act])

                q_values_for_state = [action_val[player][dealer][aces][0], action_val[player][dealer][aces][1]]
                opt_action = np.argmax(q_values_for_state)

                for possible_action in range(N_ACTIONS):
                    policy[player][dealer][aces][possible_action] = 0
                    if possible_action == opt_action:
                        policy[player][dealer][aces][possible_action] = 1
                    #         |--------State-------||-----Action----|

    return action_val, policy

def mc_w_es(epsilon, gamma, runs = 10000):
    policy = np.zeros(shape=(10,10,2,2))
    for player_sum in range(10):
        for dealer_card in range(10):
            for usable_ace in range(2):
                policy[player_sum][dealer_card][usable_ace][0] = 1
    action_val = np.zeros(shape=(10,10,2,2))
    returns = [[[[[] for _ in range(2)] for _ in range(2)] for _ in range(10)] for _ in range(10)]

    bj = BlackJackEnv()    

    for _ in range(runs):
        episode = bj.run_episode(policy) #Returns a list of tuples (state, action, reward)
        g = 0
        seen_states = []
        for i in reversed(range(len(episode))):
            g = g * gamma + episode[i][2]
            state_action_pair = (episode[i][0], episode[i][1])

            act = episode[i][1]
            aces = episode[i][0]['usable_aces']
            dealer = CARD_INDX[episode[i][0]['dealer_card']]
            player = episode[i][0]['hand_sum'] - 12

            if state_action_pair not in seen_states:
                seen_states.append(state_action_pair)
                returns[player][dealer][aces][act].append(g)
                action_val[player][dealer][aces][act] = np.mean(returns[player][dealer][aces][act])

                q_values_for_state = [action_val[player][dealer][aces][0], action_val[player][dealer][aces][1]]
                opt_action = np.argmax(q_values_for_state)
                for possible_action in range(N_ACTIONS):
                    policy[player][dealer][aces][possible_action] = epsilon/N_ACTIONS
                    if possible_action == opt_action:
                        policy[player][dealer][aces][possible_action] = 1 - epsilon + (epsilon/N_ACTIONS)

    return action_val, policy
