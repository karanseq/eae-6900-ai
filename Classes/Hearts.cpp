#include "Hearts.h"

// Library includes
#include <algorithm>

// Engine includes
#include "cocos2d.h"

// Game includes
#include "HelloWorldScene.h"

void Hearts::Init(HelloWorld* i_scene)
{
	srand(time_t());

	CC_ASSERT(i_scene);
	scene_ = i_scene;

	deck_.Init();

	for (uint8_t i = 0; i < Player::NUM_PLAYERS; ++i)
	{
		players_[i].Init(i);
	}

	turns_.reserve(NUM_CARDS_PER_PLAYER);

	for (uint8_t i = 0; i < 10; ++i)
	{
		CCLOG("Test Iteration %02d", i);
		RunTest();
	}
}

void Hearts::Shutdown()
{}

void Hearts::StartRound()
{
	CCLOG(__FUNCTION__);
	turns_.clear();
	current_turn_id_ = 0;

	deck_.Init();
	DealCards();
}

void Hearts::StartTurn()
{
	CCLOG(__FUNCTION__);

	uint8_t first_player = 0;

	// Is it past the last turn?
	if (current_turn_id_ == NUM_CARDS_PER_PLAYER)
	{
		scene_->FinishedRound();
		return;
	}
	// Is it the first turn?
	else if (current_turn_id_ == 0)
	{
		// Player that has the two of clubs must start
		for (uint8_t i = 0; i < Player::NUM_PLAYERS; ++i)
		{
			if (players_[i].GetCardsInHand()[0] == Card::TWO_OF_CLUBS)
			{
				first_player = i;
				break;
			}
		}
	}
	else
	{
		// Player that lost the previous round must start
		first_player = turns_[current_turn_id_ - 1].GetLoser();
	}

	// Determine turn order
	uint8_t turn_order[Player::NUM_PLAYERS];
	turn_order[0] = first_player;
	for (uint8_t i = 1; i < Player::NUM_PLAYERS; ++i)
	{
		turn_order[i] = turn_order[i - 1] + 1 < Player::NUM_PLAYERS ? (turn_order[i - 1] + 1) : 0;
	}

	// Create the current turn and add it to the list
	Turn current_turn(current_turn_id_, turn_order);
	turns_.push_back(current_turn);

	scene_->FinishedPlayingCard(Card::INVALID);
}

void Hearts::PlayCard()
{
	CCLOG(__FUNCTION__);
	
	Turn& current_turn = turns_[current_turn_id_];

	// Get the number of cards that have been played
	const uint8_t num_cards_played(current_turn.GetNumCardsPlayed());

	// Check if all players have played
	if (num_cards_played < Player::NUM_PLAYERS)
	{
		// Get the id of the player that should play
		const uint8_t player_id = current_turn.GetOrder()[num_cards_played];

		// Ask the player to play
		const Card played_card = players_[player_id].PlayCardForCurrentTurn(current_turn);

		// Add the played card to the current turn
		current_turn.AddCard(played_card);

		scene_->FinishedPlayingCard(played_card);
	}
	else
	{
		current_turn.FindLoser();
		current_turn.Print();

		scene_->FinishedTurn(current_turn);
		++current_turn_id_;
	}
}

void Hearts::DealCards()
{
	CCLOG(__FUNCTION__);

	deck_.Shuffle();

	for (uint8_t i = 0; i < Player::NUM_PLAYERS; ++i)
	{
		std::vector<Card>& cards_in_hand = players_[i].GetCardsInHand();
		cards_in_hand.clear();

		for (uint8_t j = 0; j < NUM_CARDS_PER_PLAYER; ++j)
		{
			cards_in_hand.push_back(deck_.GetCardAt(i * NUM_CARDS_PER_PLAYER + j));
		}

		std::sort(cards_in_hand.begin(), cards_in_hand.end());
		players_[i].Print();
	}

	scene_->FinishedDealingCards();
}

void Hearts::RunTest() const
{
	Deck deck;
	deck.Init();
	deck.Shuffle();

	Player player;
	player.Init(0);
	
	std::vector<Card>& player_hand = player.GetCardsInHand();
	for (uint8_t i = 0; i < NUM_CARDS_PER_PLAYER; ++i)
	{
		player_hand.push_back(deck.GetCardAt(i));
	}
	std::sort(player_hand.begin(), player_hand.end());

	constexpr uint8_t turn_id = 1;
	constexpr uint8_t turn_order[Player::NUM_PLAYERS] = { 0, 1, 2, 3 };
	Turn turn(turn_id, turn_order);

	player.Print();
	Card played_card = player.PlayCardForCurrentTurn(turn);
	CCLOG("Played:");
	Deck::PrintCard(played_card);
}
