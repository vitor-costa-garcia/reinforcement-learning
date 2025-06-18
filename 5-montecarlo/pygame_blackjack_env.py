import pygame
import blackjack_env
import numpy as np

# pygame setup
pygame.init()
screen_w, screen_h = 1280, 900
card_w, card_h, card_offset = 180, 280, 5
screen = pygame.display.set_mode((screen_w, screen_h))
pygame.display.set_caption("Finding optimal policies for a blackjack game with MC methods!")
clock = pygame.time.Clock()
running = True

card_values = ["Hidden", "Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Queen", "Jack", "King"]
font = pygame.font.SysFont('Arial', 48, bold=True)
card_values_text = dict([(text, font.render(text, True, 'black')) for text in card_values])

class Playing_Card:
    def __init__(self, value, hidden = False):
        self.card_rect = pygame.Rect(0, 0, card_w, card_h)
        self.card_text = card_values_text[value]
        self.hidden = hidden

    def blit_card(self, surface, left, top):
        if not self.hidden:
            card_text_rect = self.card_text.get_rect()
            card_text_rect.center = self.card_rect.center
            pygame.draw.rect(surface, 'white', self.card_rect.move(left, top), border_radius=5)
            surface.blit(self.card_text, card_text_rect.move(left, top))
        else:
            card_text_rect = card_values_text['Hidden'].get_rect()
            card_text_rect.center = self.card_rect.center
            pygame.draw.rect(surface, '#abcbff', self.card_rect.move(left, top), border_radius=5)
            surface.blit(card_values_text['Hidden'], card_text_rect.move(left, top))

def transform_cards(hand, is_dealer = False):
    return [Playing_Card(hand[i].value, True) if i != 0 and is_dealer else Playing_Card(hand[i].value) for i in range(len(hand))]

bj = blackjack_env.BlackJackEnv()

policy = np.zeros(shape=(10,10,2,2))
for player_sum in range(10):
    for dealer_card in range(10):
        for usable_ace in range(2):
            policy[player_sum][dealer_card][usable_ace][0] = 1
action_val = np.zeros(shape=(10,10,2,2))
returns = [[[[[] for _ in range(2)] for _ in range(2)] for _ in range(10)] for _ in range(10)]

dealer_w, player_w, draws, player_profit = 0, 0, 0, 0

while running:
    episode = bj.run_episode(policy, True)
    print(episode)
    for step in range(len(episode)):
        player_hand = transform_cards(episode[step][0]['player_hand'])
        dealer_hand = transform_cards(episode[step][0]['dealer_hand'], True)

        match episode[step][2]:
            case -1:
                dealer_w += 1
                player_profit -= 1
            case 0:
                draws += 1
            case 1:
                player_w += 1
                player_profit += 1
            case 1.5:
                player_w += 1
                player_profit += 1.5

        # poll for events
        # pygame.QUIT event means the user clicked X to close your window
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        # fill the screen with a color to wipe away anything from last frame
        screen.fill("#35654d")

        for i in range(len(player_hand)):
            player_hand[i].blit_card(screen, (screen_w-(len(player_hand)-1*card_offset)-(len(player_hand)*card_w))/2 + card_w*i + i*card_offset, 580)

        for i in range(len(dealer_hand)):
            dealer_hand[i].blit_card(screen, (screen_w-(len(dealer_hand)-1*card_offset)-(len(dealer_hand)*card_w))/2 + card_w*i + i*card_offset, 40)


        c = 0
        for count, desc in zip([player_w, dealer_w, draws, round((player_w+1)/(dealer_w+1),3), player_profit], ["Player Wins: ", "Dealer Wins: ", "Draws: ", "Win/Loss Ratio: ", "Player profit:"]):
            counter = font.render(f"{desc}{count}", True, 'black')
            counter_rect = counter.get_rect()
            screen.blit(counter, counter_rect.move(20, 20 + 100*c))
            c += 1


        # RENDER YOUR GAME HERE

        # flip() the display to put your work on screen
        pygame.display.flip()

        clock.tick(60)  # limits FPS to 60

pygame.quit()