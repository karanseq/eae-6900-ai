#ifndef CARDS_H_
#define CARDS_H_

// Library includes
#include <stdint.h>
#include <vector>

enum class ECardSuit : uint8_t
{
	Clubs,
	Diamonds,
	Hearts,
	Spades,
	Invalid
};

enum class ECardRank : uint8_t
{
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	Ten,
	Jack,
	Queen,
	King,
	Ace,
	Invalid
};

struct Card
{
	ECardSuit suit;
	ECardRank rank;

	Card(ECardSuit i_suit, ECardRank i_rank) : suit(i_suit),
		rank(i_rank)
	{}

	Card() : suit(ECardSuit::Invalid),
		rank(ECardRank::Invalid)
	{}

	static Card INVALID;
	static Card TWO_OF_CLUBS;

	inline bool operator==(const Card& i_other) const
	{
		return suit == i_other.suit && rank == i_other.rank;
	}

	inline bool operator!=(const Card& i_other) const
	{
		return !(*this == i_other);
	}

	inline bool operator==(const ECardSuit i_suit) const
	{
		return suit == i_suit;
	}

	inline bool operator==(const ECardRank i_rank) const
	{
		return rank == i_rank;
	}

	inline bool operator<(const Card& i_other) const 
	{
		return suit == i_other.suit ? (rank < i_other.rank) : (suit < i_other.suit);
	}
};

class Deck
{
public:
	Deck() = default;
	~Deck() = default;

	void Init();
	void Shuffle();

	void Print() const;

	static const char* GetSuitName(ECardSuit i_suit);
	static const char* GetRankName(ECardRank i_rank);
	static uint8_t GetCardIndex(const Card& i_card);

	static bool HasCard(const std::vector<Card>& i_cards, ECardSuit i_suit, ECardRank i_rank);
	static bool HasCardWithSuit(const std::vector<Card>& i_cards, ECardSuit i_suit);
	static bool HasCardWithRank(const std::vector<Card>& i_cards, ECardRank i_rank);
	
	static uint8_t FindCardWithHighestRank(const std::vector<Card>& i_cards);
	static uint8_t FindCardWithLowestRank(const std::vector<Card>& i_cards);

	static uint8_t FindCard(const std::vector<Card>& i_cards, ECardSuit i_suit, ECardRank i_rank);
	static uint8_t FindHighestCard(const std::vector<Card>& i_cards, ECardSuit i_suit, ECardRank i_lower_than_rank = ECardRank::Ace);
	static uint8_t FindLowestCard(const std::vector<Card>& i_cards, ECardSuit i_suit, ECardRank i_higher_than_rank = ECardRank::Two);

	static void PrintCard(const Card& i_card);

public:
	static constexpr uint8_t					NUM_CARDS_IN_DECK = 52;

	inline const Card& GetCardAt(uint8_t i_index) const { return i_index < NUM_CARDS_IN_DECK ? cards_[i_index] : Card::INVALID; }

private:
	Card										cards_[NUM_CARDS_IN_DECK];

};

#endif // CARDS_H_
