#include "Player.h"

// Engine includes
#include "cocos2d.h"

// Game includes
#include "Cards.h"
#include "Hearts.h"

//============================================================================
// Player

void Player::Init(uint8_t i_id)
{
	id_ = i_id;
	hand_.reserve(Hearts::NUM_CARDS_PER_PLAYER);
}

void Player::Print() const
{
	CCLOG("Player-%d", id_);
	CCLOG("Score:%d", score_);
	for (const auto& card : hand_)
	{
		Deck::PrintCard(card);
	}
}

Card Player::PlayCardForCurrentTurn(const Turn& i_turn)
{
	uint8_t playing_card_index = Hearts::NUM_CARDS_PER_PLAYER;

	// Am I leading this turn?
	if (i_turn.GetNumCardsPlayed() == 0)
	{
		// Is this the first turn?
		if (i_turn.GetId() == 0)
		{
			// I must have the two of clubs
			playing_card_index = 0;
		}
		else
		{
			playing_card_index = FindCardWhenNoCardsPlayed(i_turn);
		}
	}
	else
	{
		// Get the cards that have been played
		const std::vector<Card>& cards_played = i_turn.GetCardsPlayed();
		
		// What was the leading card?
		ECardSuit leading_card_suit = cards_played[0].suit;
		ECardRank leading_card_rank = cards_played[0].rank;

		// Do I have a card that matches the leading suit?
		bool have_leading_suit = Deck::HasCardWithSuit(hand_, leading_card_suit);
		if (have_leading_suit)
		{
			playing_card_index = FindCardWhenHaveLeadingSuit(i_turn);
		}
		else
		{
			playing_card_index = FindCardWhenDontHaveLeadingSuit(i_turn);
		}
	}

	if (playing_card_index >= Hearts::NUM_CARDS_PER_PLAYER)
	{
		CCLOGERROR("Player-%d couldn't find a suitable card for the following turn!", id_);
		i_turn.Print();
		return Card::INVALID;
	}
	else
	{
		// Copy the card before discarding it
		Card playing_card = hand_[playing_card_index];

		// Discard this card
		hand_.erase(hand_.begin() + playing_card_index);

		return playing_card;
	}
}

uint8_t Player::FindCardWhenNoCardsPlayed(const Turn& i_turn) const
{
	uint8_t card_index = Hearts::NUM_CARDS_PER_PLAYER;
	ECardRank lowest_rank = ECardRank::Invalid;

	for (uint8_t i = 0; i < uint8_t(ECardSuit::Invalid); ++i)
	{
		// Find the lowest card for each suit
		uint8_t lowest_card_index = Deck::FindLowestCard(hand_, ECardSuit(i));
		if (lowest_card_index < Hearts::NUM_CARDS_PER_PLAYER &&
			hand_[lowest_card_index].rank < lowest_rank)
		{
			lowest_rank = hand_[lowest_card_index].rank;
			card_index = lowest_card_index;
		}
	}

	return card_index;
}

uint8_t Player::FindCardWhenHaveLeadingSuit(const Turn& i_turn) const
{
	uint8_t card_index = Hearts::NUM_CARDS_PER_PLAYER;

	// Find the rank of the highest card of the leading suit that has been played
	const ECardSuit leading_suit = i_turn.GetLeadingCardSuit();
	uint8_t index_highest_played_card = Deck::FindHighestCard(i_turn.GetCardsPlayed(), leading_suit);

	// Sanity check
	if (index_highest_played_card < Hearts::NUM_CARDS_PER_PLAYER)
	{
		// Find highest card in hand that's lower than the highest played card
		const ECardRank highest_rank = i_turn.GetCardsPlayed()[index_highest_played_card].rank;
		card_index = Deck::FindHighestCard(hand_, leading_suit, highest_rank);
	}
	else
	{
		// Fallback in case sanity check fails
		card_index = Deck::FindLowestCard(hand_, leading_suit);
	}

	return card_index;
}

uint8_t Player::FindCardWhenDontHaveLeadingSuit(const Turn& i_turn) const
{
	uint8_t card_index = Hearts::NUM_CARDS_PER_PLAYER;

	// Do I have the queen of spades?
	const uint8_t queen_of_spades_index = Deck::FindCard(hand_, ECardSuit::Spades, ECardRank::Queen);
	if (queen_of_spades_index < Hearts::NUM_CARDS_PER_PLAYER)
	{
		card_index = queen_of_spades_index;
		return card_index;
	}

	// What's the highest hearts card that I have?
	const uint8_t highest_hearts_card_index = Deck::FindHighestCard(hand_, ECardSuit::Hearts);
	if (highest_hearts_card_index < Hearts::NUM_CARDS_PER_PLAYER)
	{
		card_index = highest_hearts_card_index;
		return card_index;
	}

	// What's the highest ranking card that I have?
	const uint8_t highest_ranking_card = Deck::FindCardWithHighestRank(hand_);
	card_index = highest_ranking_card;

	return card_index;
}

//============================================================================
// Turn

Turn::Turn(uint8_t i_id, const uint8_t* i_order) :
	id_(i_id)
{
	memcpy_s(order_, sizeof(uint8_t) * Player::NUM_PLAYERS, i_order, sizeof(uint8_t) * Player::NUM_PLAYERS);
	cards_.reserve(Player::NUM_PLAYERS);
}

void Turn::Print() const
{
	CCLOG("Turn-%d", id_);
	CCLOG("Order:%d, %d, %d, %d", order_[0], order_[1], order_[2], order_[3]);
	CCLOG("Loser:%d", loser_id_);
	for (const auto& card : cards_)
	{
		Deck::PrintCard(card);
	}
}

void Turn::FindLoser()
{
	if (cards_.size() < Player::NUM_PLAYERS)
	{
		CCLOGERROR("FindLoser has been called but the turn hasn't ended yet!");
		return;
	}
}
