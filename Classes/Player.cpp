#include "Player.h"

// Engine includes
#include "cocos2d.h"

// Game includes
#include "Cards.h"
#include "Hearts.h"
#include "Simulation.h"

//============================================================================
// Player

void Player::Init(uint8_t i_id, bool i_simulation_enabled, Hearts* i_hearts)
{
	id_ = i_id;
	hand_.reserve(Hearts::NUM_CARDS_PER_PLAYER);
	simulation_enabled_ = i_simulation_enabled;
	hearts_ = i_hearts;
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
			if (simulation_enabled_ == true)
			{
				playing_card_index = FindCardUsingSimulation(i_turn);
			}
			else
			{
				playing_card_index = FindCardWhenNoCardsPlayed(i_turn);
			}
		}
	}
	else
	{
		if (simulation_enabled_ == true)
		{
			playing_card_index = FindCardUsingSimulation(i_turn);
		}
		else
		{
			// Get the cards that have been played
			const std::vector<Card>& cards_played_this_turn = i_turn.GetCardsPlayed();
		
			// What was the leading card?
			ECardSuit leading_card_suit = cards_played_this_turn[0].suit;
			ECardRank leading_card_rank = cards_played_this_turn[0].rank;

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
	}

	if (playing_card_index >= Hearts::NUM_CARDS_PER_PLAYER)
	{
		playing_card_index = uint8_t(hand_.size() - 1);
		CCLOGERROR("Player-%d couldn't find a suitable card for the following turn!", id_);
		i_turn.Print();
	}

	// Copy the card before discarding it
	Card playing_card = hand_[playing_card_index];
	// Discard this card
	hand_.erase(hand_.begin() + playing_card_index);

	return playing_card;
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

uint8_t Player::FindCardUsingSimulation(const Turn& i_turn) const
{
	Simulation simulation;

	const std::vector<Card>& played_cards = hearts_->GetPlayedCards();
	std::vector<Card> opponent_cards;
	{
		constexpr uint8_t max_suit = (uint8_t)ECardSuit::Spades;
		constexpr uint8_t max_rank = (uint8_t)ECardRank::Ace;

		for (uint8_t suit = 0; suit <= max_suit; ++suit)
		{
			for (uint8_t rank = 0; rank <= max_rank; ++rank)
			{
				if (Deck::HasCard(hand_, ECardSuit(suit), ECardRank(rank)) ||
					Deck::HasCard(played_cards, ECardSuit(suit), ECardRank(rank)))
				{
					continue;
				}

				opponent_cards.push_back(Card(ECardSuit(suit), ECardRank(rank)));
			}
		}
	}

	constexpr double execution_time = 0.5;
	simulation.Start(this,
		i_turn,
		played_cards,
		opponent_cards,
		execution_time);

	uint8_t card_index = Hearts::NUM_CARDS_PER_PLAYER;
	{
		const std::vector<CardScorePair>& card_scores = simulation.GetCardScores();
		Card card_with_lowest_score = Card::TWO_OF_CLUBS;
		uint8_t lowest_score = Deck::NUM_CARDS_IN_DECK;

		for (const CardScorePair& card_score : card_scores)
		{
			if (card_score.average_score < lowest_score)
			{
				card_with_lowest_score = card_score.card;
				lowest_score = card_score.average_score;
			}
			else if (card_score.average_score == lowest_score)
			{
				card_with_lowest_score = card_score.card.rank < card_with_lowest_score.rank ? card_score.card : card_with_lowest_score;
			}
		}

		const auto card_iter = std::find(hand_.begin(), hand_.end(), card_with_lowest_score);
		card_index = card_iter - hand_.begin();

		CCLOG("Card chosen after running simulation:");
		Deck::PrintCard(hand_[card_index]);
	}

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
	CCLOG("Score:%d", score_);
	for (const auto& card : cards_)
	{
		Deck::PrintCard(card);
	}
}

void Turn::EndTurn()
{
	if (cards_.size() < Player::NUM_PLAYERS)
	{
		CCLOGERROR("FindLoser has been called but the turn hasn't ended yet!");
		return;
	}

	// Prepare parameters to find the loser of this turn
	const ECardSuit leading_suit = cards_[0].suit;
	ECardRank highest_ranking_card = cards_[0].rank;
	loser_id_ = order_[0];

	// Find the loser of this turn
	for (uint8_t i = 1; i < Player::NUM_PLAYERS; ++i)
	{
		if (cards_[i].suit == leading_suit &&
			cards_[i].rank > highest_ranking_card)
		{
			loser_id_ = order_[i];
			highest_ranking_card = cards_[i].rank;
		}
	}

	// Calculate the score that will be added to the loser
	for (const Card& card : cards_)
	{
		score_ += card.suit == ECardSuit::Hearts ? 1 : 0;
		score_ += card.suit == ECardSuit::Spades && card.rank == ECardRank::Queen ? 13 : 0;
	}
}
