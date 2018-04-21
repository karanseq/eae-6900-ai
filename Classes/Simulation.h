#ifndef SIMULATION_H_
#define SIMULATION_H_

// Library includes
#include <vector>
#include <stdint.h>

// Engine includes

// Game includes
#include "Cards.h"

// Forward declarations
class Player;
class Turn;

struct CardScorePair
{
	CardScorePair(const Card& i_card) :
		card(i_card)
	{}

	Card card = Card::INVALID;
	uint8_t min_score = Deck::NUM_CARDS_IN_DECK;
	uint8_t max_score = 0;
	size_t average_score = 0;

}; // struct CardScorePair

class Simulation
{
public:
	Simulation() = default;
	~Simulation() = default;

	void Start(const Player* i_player_to_simulate,
		const Turn& i_turn_to_simulate,
		const std::vector<Card>& i_played_cards,
		const std::vector<Card>& i_opponent_cards,
		double i_execution_time);

	inline const std::vector<CardScorePair>& GetCardScores() const { return card_scores_; }
	inline size_t GetNumIterations() const { return num_iterations_; }

private:
	uint8_t Run(const Card& i_card_to_simulate,
		const Turn& i_turn_to_simulate) const;

	void PrepareSimuationInputs(std::vector<Card>& o_player_hand,
		std::vector<Card>& o_played_cards,
		std::vector<Card>& o_opponent_cards) const;

	void GetLegalCardsForTurn(std::vector<Card>& o_legal_cards,
		const Turn& i_turn) const;

	Card GetCardForTurn(std::vector<Card>& io_cards, const Turn& i_turn) const;

	void PrintResults() const;

private:
	double execution_time_limit_;
	double execution_time_spent_;
	size_t num_iterations_ = 0;

	const Player* player_ = nullptr;

	std::vector<Card> cards_played_before_simulation_;
	std::vector<Card> opponent_cards_;

	std::vector<CardScorePair> card_scores_;

}; // class Simulation

#endif // SIMULATION_H_
