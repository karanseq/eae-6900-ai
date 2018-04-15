#include "Cards.h"

// Library includes
#include <algorithm>

// Engine includes
#include "cocos2d.h"

void Deck::Init(Deck* io_deck)
{
	uint8_t max_suit = (uint8_t)ECardSuit::Spades;
	uint8_t max_rank = (uint8_t)ECardRank::Ace;

	for (uint8_t suit = 0; suit <= max_suit; ++suit)
	{
		for (uint8_t rank = 0; rank <= max_rank; ++rank)
		{
			io_deck->cards[suit * (max_rank + 1) + rank].suit = (ECardSuit)suit;
			io_deck->cards[suit * (max_rank + 1) + rank].rank = (ECardRank)rank;
		}
	}
}

void Deck::Shuffle(Deck* io_deck)
{
	std::random_shuffle(io_deck->cards, io_deck->cards + NUM_CARDS_IN_DECK - 1);
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
		return "None";
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
		return "None";
	}
}

void Deck::Print(const Deck* i_deck)
{
	for (uint8_t i = 0; i < NUM_CARDS_IN_DECK; ++i)
	{
		CCLOG("Card-%d Suit:%s Rank:%s", i, GetSuitName(i_deck->cards[i].suit), GetRankName(i_deck->cards[i].rank));		
	}
}
