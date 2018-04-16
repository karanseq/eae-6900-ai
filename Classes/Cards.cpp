#include "Cards.h"

// Library includes
#include <algorithm>

// Engine includes
#include "cocos2d.h"

// Game includes
#include "Hearts.h"

// Static member initialization
Card Card::INVALID;
Card Card::TWO_OF_CLUBS(ECardSuit::Clubs, ECardRank::Two);

void Deck::Init()
{
	constexpr uint8_t max_suit = (uint8_t)ECardSuit::Spades;
	constexpr uint8_t max_rank = (uint8_t)ECardRank::Ace;

	for (uint8_t suit = 0; suit <= max_suit; ++suit)
	{
		for (uint8_t rank = 0; rank <= max_rank; ++rank)
		{
			cards_[suit * (max_rank + 1) + rank].suit = (ECardSuit)suit;
			cards_[suit * (max_rank + 1) + rank].rank = (ECardRank)rank;
		}
	}
}

void Deck::Shuffle()
{
	std::random_shuffle(cards_, cards_ + NUM_CARDS_IN_DECK - 1);
}

void Deck::Print() const
{
	for (uint8_t i = 0; i < NUM_CARDS_IN_DECK; ++i)
	{
		CCLOG("Card:%d Suit:%s Rank:%s", i, GetSuitName(cards_[i].suit), GetRankName(cards_[i].rank));
	}
}

const char* Deck::GetSuitName(ECardSuit i_suit)
{
	switch (i_suit)
	{
	case ECardSuit::Clubs:
		return "Clubs";
	case ECardSuit::Diamonds:
		return "Diamonds";
	case ECardSuit::Hearts:
		return "Hearts";
	case ECardSuit::Spades:
		return "Spades";
	default:
		return "Invalid";
	}
}

const char* Deck::GetRankName(ECardRank i_rank)
{
	switch (i_rank)
	{
	case ECardRank::Two:
		return "Two";
	case ECardRank::Three:
		return "Three";
	case ECardRank::Four:
		return "Four";
	case ECardRank::Five:
		return "Five";
	case ECardRank::Six:
		return "Six";
	case ECardRank::Seven:
		return "Seven";
	case ECardRank::Eight:
		return "Eight";
	case ECardRank::Nine:
		return "Nine";
	case ECardRank::Ten:
		return "Ten";
	case ECardRank::Jack:
		return "Jack";
	case ECardRank::Queen:
		return "Queen";
	case ECardRank::King:
		return "King";
	case ECardRank::Ace:
		return "Ace";
	default:
		return "Invalid";
	}
}

bool Deck::HasCardWithSuit(const std::vector<Card>& i_cards, ECardSuit i_suit)
{
	return std::find(i_cards.begin(), i_cards.end(), i_suit) != i_cards.end();
}

bool Deck::HasCardWithRank(const std::vector<Card>& i_cards, ECardRank i_rank)
{
	return std::find(i_cards.begin(), i_cards.end(), i_rank) != i_cards.end();
}

uint8_t Deck::FindHighestCard(const std::vector<Card>& i_cards, ECardSuit i_suit, ECardRank i_lower_than_or_equal_to_rank /*= ECardRank::Ace*/)
{
	// Find the first card with the matching suit
	auto iter_lowest_card = std::find(i_cards.begin(), i_cards.end(), i_suit);
	if (iter_lowest_card != i_cards.end())
	{
		// This means we've found a card from the given suit
		// Now try to find the highest one possible
		uint8_t found_card_index = iter_lowest_card - i_cards.begin();
		while (iter_lowest_card->suit == i_suit &&
			iter_lowest_card->rank <= i_lower_than_or_equal_to_rank)
		{
			found_card_index = iter_lowest_card - i_cards.begin();

			++iter_lowest_card;
			if (iter_lowest_card == i_cards.end())
			{
				break;
			}
		}
		return found_card_index;
	}

	return Hearts::NUM_CARDS_PER_PLAYER;
}

uint8_t Deck::FindLowestCard(const std::vector<Card>& i_cards, ECardSuit i_suit, ECardRank i_higher_than_or_equal_to_rank /*= ECardRank::Two*/)
{
	// Find the first card with the matching suit
	auto rev_iter_highest_card = std::find(i_cards.rbegin(), i_cards.rend(), i_suit);
	if (rev_iter_highest_card == i_cards.rend())
	{
		return Hearts::NUM_CARDS_PER_PLAYER;
	}

	auto iter_highest_card = rev_iter_highest_card.base() - 1;

	if (iter_highest_card != i_cards.end())
	{
		// This means we've found a card from the given suit
		// Now try to find the lowest one possible
		uint8_t found_card_index = iter_highest_card - i_cards.begin();
		while (iter_highest_card->suit == i_suit &&
			iter_highest_card->rank >= i_higher_than_or_equal_to_rank)
		{
			found_card_index = iter_highest_card - i_cards.begin();

			if (iter_highest_card == i_cards.begin())
			{
				break;
			}
			--iter_highest_card;
		}
		return found_card_index;
	}

	return Hearts::NUM_CARDS_PER_PLAYER;
}

void Deck::PrintCard(const Card& i_card)
{
	CCLOG("Suit:%s Rank:%s", GetSuitName(i_card.suit), GetRankName(i_card.rank));
}
