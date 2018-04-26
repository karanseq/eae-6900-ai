#include "Simulation.h"

// Library includes
#include <chrono>

// Engine includes
#include "cocos2d.h"

// Game includes
#include "Hearts.h"
#include "Player.h"

void Simulation::Start(const Player* i_player_to_simulate,
	const Turn& i_turn_to_simulate,
	const std::vector<Card>& i_played_cards,
	const std::vector<Card>& i_opponent_cards,
	double i_execution_time)
{
	// Initialize time counters
	execution_time_limit_ = i_execution_time;
	execution_time_spent_ = 0.0;

	// Save off simulation initialization information
	player_ = i_player_to_simulate;
	cards_played_before_simulation_ = i_played_cards;
	opponent_cards_ = i_opponent_cards;

	// Gather the legal cards that can be played
	std::vector<Card> legal_cards;
	GetLegalCardsForTurn(legal_cards, i_turn_to_simulate);

	// Prepare simulation outputs
	card_scores_.reserve(legal_cards.size());
	for (const Card& card : legal_cards)
	{
		card_scores_.push_back({ card });
	}

	// Cache simulation start time
	auto timestamp = std::chrono::steady_clock::now();

	// Run the simulation till time runs out
	while (execution_time_spent_ < execution_time_limit_)
	{
		++num_iterations_;

		// Run a simulation for each legal card in the player's hand
		for (uint8_t i = 0; i < legal_cards.size(); ++i)
		{
			// Save off the score from this simulation
			const uint8_t score = Run(legal_cards[i], i_turn_to_simulate);
			card_scores_[i].min_score = score < card_scores_[i].min_score ? score : card_scores_[i].min_score;
			card_scores_[i].max_score = score > card_scores_[i].max_score ? score : card_scores_[i].max_score;
			card_scores_[i].average_score += score;
		}

		// Update simulation execution time
		const auto now = std::chrono::steady_clock::now();
		execution_time_spent_ += std::chrono::duration<double>(now - timestamp).count();
		timestamp = now;
	}

	for (CardScorePair& card_score : card_scores_)
	{
		card_score.average_score /= num_iterations_;
	}

	PrintResults();
}

uint8_t Simulation::Run(const Card& i_card_to_simulate,
	const Turn& i_turn_to_simulate) const
{
	// Prepare simulation inputs and outputs
	uint8_t score = 0;
	std::vector<Card> player_hand;
	std::vector<Card> played_cards;
	std::vector<Card> opponent_cards;
	PrepareSimuationInputs(player_hand, played_cards, opponent_cards);

	// Start simulating from the turn provided
	Turn current_turn = i_turn_to_simulate;
	uint8_t current_turn_id = i_turn_to_simulate.GetId();

	// Start the simulation by playing the card provided
	{
		const auto iter_starting_card = std::find(player_hand.begin(), player_hand.end(), i_card_to_simulate);
		// Remove the card from the player's hand
		player_hand.erase(iter_starting_card);
		// Add the card to the turn & played cards
		current_turn.AddCard(i_card_to_simulate);
		played_cards.push_back(i_card_to_simulate);
	}

	// Simulate one round
	while (1)
	{
		// Simulate one turn
		while (1)
		{
			// Get the number of cards that have been played
			const uint8_t num_cards_played(current_turn.GetNumCardsPlayed());

			// Check if all players have played
			if (num_cards_played < Player::NUM_PLAYERS)
			{
				// Get the id of the player that should play
				const uint8_t player_id = current_turn.GetOrder()[num_cards_played];

				// Ask the player/opponents to play a card
				const Card played_card = (player_id == player_->GetId()) ? GetCardForTurn(player_hand, current_turn) : GetCardForTurn(opponent_cards, current_turn);

				// Add the card to the turn & played cards
				current_turn.AddCard(played_card);
				played_cards.push_back(played_card);
			}
			else
			{
				// End this turn
				break;
			}

		} // Simulate one turn

		++current_turn_id;

		// Check if the round has ended
		if (current_turn_id < Hearts::NUM_CARDS_PER_PLAYER)
		{
			// Save off the simulating player's score
			current_turn.EndTurn();
			score += current_turn.GetLoser() == player_->GetId() ? current_turn.GetScore() : 0;

			// Determine turn order
			uint8_t turn_order[Player::NUM_PLAYERS];
			turn_order[0] = current_turn.GetLoser();

			for (uint8_t i = 1; i < Player::NUM_PLAYERS; ++i)
			{
				turn_order[i] = turn_order[i - 1] + 1 < Player::NUM_PLAYERS ? (turn_order[i - 1] + 1) : 0;
			}

			// Initialize the next turn
			Turn next_turn(current_turn_id, turn_order);
			current_turn = next_turn;
		}
		else
		{
			// End this round
			break;
		}

	} // Simulate one round

	return score;
}

void Simulation::PrepareSimuationInputs(std::vector<Card>& o_player_hand,
	std::vector<Card>& o_played_cards,
	std::vector<Card>& o_opponent_cards) const
{
	o_player_hand = player_->GetCardsInHand();
	o_played_cards = cards_played_before_simulation_;
	o_opponent_cards = opponent_cards_;
}

void Simulation::GetLegalCardsForTurn(std::vector<Card>& o_legal_cards,
	const Turn& i_turn) const
{
	o_legal_cards.reserve(player_->GetCardsInHand().size());

	// Get the cards that have been played this turn
	const std::vector<Card>& cards_played_this_turn = i_turn.GetCardsPlayed();

	// Were there any cards played?
	if (cards_played_this_turn.empty() == false)
	{
		// What was the leading card?
		ECardSuit leading_card_suit = cards_played_this_turn[0].suit;

		// Does the player have a card that matches the leading suit?
		const bool have_leading_suit = Deck::HasCardWithSuit(player_->GetCardsInHand(), leading_card_suit);
		if (have_leading_suit)
		{
			// Copy all the cards that match the leading suit
			for (const Card& card : player_->GetCardsInHand())
			{
				if (card.suit == leading_card_suit)
				{
					o_legal_cards.push_back(card);
				}
			}
		}
		else
		{
			// When the player doesn't have the leading suit,
			// all cards are legal
			o_legal_cards = player_->GetCardsInHand();
		}
	}
	else
	{
		// If there are no cards, all cards are legal when hearts have been broken
		if (player_->GetHearts()->GetHeartsBroken())
		{
			o_legal_cards = player_->GetCardsInHand();
		}
		// If not, all but hearts are legal
		else
		{
			// Copy all the cards that match the leading suit
			for (const Card& card : player_->GetCardsInHand())
			{
				if (card.suit != ECardSuit::Hearts)
				{
					o_legal_cards.push_back(card);
				}
			}
		}
	}
}

Card Simulation::GetCardForTurn(std::vector<Card>& io_cards, const Turn& i_turn) const
{
	// Get the cards that have been played this turn
	const std::vector<Card>& cards_played_this_turn = i_turn.GetCardsPlayed();

	// Were there any cards played?
	if (cards_played_this_turn.empty() == false)
	{
		// What was the leading card?
		ECardSuit leading_card_suit = cards_played_this_turn[0].suit;

		// Is there a matching card?
		const bool have_leading_suit = Deck::HasCardWithSuit(io_cards, leading_card_suit);
		if (have_leading_suit)
		{
			// Find the first card of the leading suit
			const uint8_t lowest_card_index = Deck::FindLowestCard(io_cards, leading_card_suit);
			// Save the card before discarding it
			const Card card_leading_suit = io_cards[lowest_card_index];
			// Discard the card
			io_cards.erase(io_cards.begin() + lowest_card_index);
			return card_leading_suit;
		}
	}

	// This means either no card was played this turn OR
	// a card matching the leading suit wasn't found
	// Find the card with the lowest rank
	const uint8_t lowest_rank_index = Deck::FindCardWithLowestRank(io_cards);
	// Save the card before discarding it
	const Card card_lowest_rank = io_cards[lowest_rank_index];
	// Discard the card
	io_cards.erase(io_cards.begin() + lowest_rank_index);
	return card_lowest_rank;
}

void Simulation::PrintResults() const
{
	CCLOG("Printing results of simulation on player-%d:", player_->GetId());
	CCLOG("NumIterations:%d", num_iterations_);
	for (const CardScorePair& card_score : card_scores_)
	{
		CCLOG("Card:");
		Deck::PrintCard(card_score.card);
		CCLOG("MinScore:%d MaxScore:%d AverageScore:%zu", card_score.min_score, card_score.max_score, card_score.average_score);
	}
}
