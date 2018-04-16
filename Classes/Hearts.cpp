#include "Hearts.h"

// Library includes
#include <algorithm>

// Engine includes
#include "cocos2d.h"

// Game includes

void Hearts::Init(HelloWorld* i_scene)
{
	CC_ASSERT(i_scene);
	scene_ = i_scene;

	deck_.Init();

	for (uint8_t i = 0; i < Player::NUM_PLAYERS; ++i)
	{
		players_[i].Init(i);
	}

	turns_.reserve(NUM_CARDS_PER_PLAYER);
	current_state_ = EGameState::None;
}

void Hearts::Shutdown()
{}

void Hearts::DealCards()
{
	if (current_state_ != EGameState::None &&
		current_state_ != EGameState::RoundEnd)
	{
		return;
	}

	current_state_ = EGameState::DealCards;

	deck_.Shuffle();

	for (uint8_t i = 0; i < Player::NUM_PLAYERS; ++i)
	{
		std::vector<Card>& cards_in_hand = players_[i].GetCardsInHand();
		
		for (uint8_t j = 0; j < NUM_CARDS_PER_PLAYER; ++j)
		{
			cards_in_hand[j] = deck_.GetCardAt(i * NUM_CARDS_PER_PLAYER + j);
		}

		std::sort(cards_in_hand.begin(), cards_in_hand.end());
	}
}
