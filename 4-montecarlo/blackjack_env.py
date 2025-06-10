import random as rnd

SUITS = ["Diamonds", "Spades", "Hearts", "Clubs"]
VALUES = {"Ace":11, "Two":2, "Three":3, "Four":4, "Five":5, "Six":6, "Seven":7, "Eight":8, "Nine":9, "Ten":10, "Queen":10, "Jack":10, "King":10}
CARD_INDX = {"Ace":0, "Two":1, "Three":2, "Four":3, "Five":4, "Six":5, "Seven":6, "Eight":7, "Nine":8, "Ten":9, "Queen":9, "Jack":9, "King":9}

def generate_deck():
    deck = []
    for suit in SUITS:
        for value in VALUES:
            deck.append(Card(suit, value))
    return deck

def access_states(m, hand_sum, dealer_card, usable_aces):
    return m[hand_sum-12][CARD_INDX[dealer_card]][usable_aces]

class Card:
    def __init__(self, suit, value):
        self.suit = suit
        self.value = value
        self.points = VALUES[value]

    def __str__(self):
        return f"{self.value} of {self.suit}"
    
class Person:
    def __init__(self):
        self.hand = []
        self.hand_val = 0
        self.usable_aces = 0
        self.status = 1 # 1 -> still playing | 0 -> busted

    def add_card(self, card):
        self.hand.append(card)
        self.hand_val += card.points

        if card.value == "Ace":
            self.usable_aces += 1

        while self.hand_val > 21:
            if self.usable_aces > 0:
                self.hand_val -= 10
                self.usable_aces -= 1
            else:
                self.status = 0
                break

class BlackJackEnv:
    def __init__(self):
        self.deck = generate_deck()

    def run_episode(self, policy, starting_state = None):
        episode = []
        dealer = Person()
        player = Person()

        if not starting_state:
            for _ in range(2):
                dealer.add_card(self.deck[rnd.randint(0, len(self.deck)-1)])
                player.add_card(self.deck[rnd.randint(0, len(self.deck)-1)])

            current_state = {
                                "hand_sum": player.hand_val,
                                "dealer_card": dealer.hand[0].value,
                                "usable_aces": player.usable_aces
                            }
        else:
            player.hand_val = starting_state['hand_val']
            dealer.add_card(Card("Clubs", starting_state['dealer_card']))
            player.usable_aces = starting_state['usable_aces'] #Melhorar este codigo

        if player.hand_val == 21:
            if dealer.hand_val == 21:
                #print("Draw natural")
                episode.append((current_state, -1, 0))
            else:
                #print("Won natural")
                episode.append((current_state, -1, 1))
            return episode

        #for card in player.hand:
            #print(card)
        while True:
            current_state = {
                             "hand_sum": player.hand_val,
                             "dealer_card": dealer.hand[0].value,
                             "usable_aces": player.usable_aces
                            }

            action = access_states(policy, **current_state)

            if action == 0: #Hit
                card = self.deck[rnd.randint(0, len(self.deck)-1)]
                #print(f"Hit: {card}")
                player.add_card(card)

                if player.status == 0: #Busted
                    #print("Busted")
                    episode.append((current_state, 0, -1))
                    return episode
                episode.append((current_state, 0, 0))

            else: #Stick
                #print("Stick")
                break

        while dealer.hand_val < 17:
            dealer.add_card(self.deck[rnd.randint(0, len(self.deck)-1)])

        if dealer.status == 0 or player.hand_val > dealer.hand_val:
            #print("Won")
            episode.append((current_state, 1, 1))
            return episode

        if player.hand_val == dealer.hand_val:
            #print("Draw")
            episode.append((current_state, 1, 0))
            return episode

        #print("Lost")
        episode.append((current_state, 1, -1))
        return episode