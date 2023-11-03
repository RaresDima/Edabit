#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::endl;
using std::vector;
using std::string;


class Card
{
public:

    class Suit
    {
    public:
        enum Value : uint8_t { Diamonds, Hearts, Spades, Clubs };
    private:
        Value value;

    public:
        Suit(const Suit& suit) : value(suit.value) {}
        Suit(const string& suit)
        {
            switch (suit[0])
            {
            case 'd':
                value = Value::Diamonds;
                break;
            case 'h':
                value = Value::Hearts;
                break;
            case 's':
                value = Value::Spades;
                break;
            case 'c':
                value = Value::Clubs;
                break;
            default:
                cout << "[ERROR] bad suit string: \"" << suit << "\"" << endl;
                exit(1);
            }
        }
        Suit(const char suit) : Suit(string(&suit)) {}
        bool operator == (const Suit& other) const { return value == other.value; }
    };

    class Rank
    {
    public:
        enum Value : uint8_t { _1 = 1, _2 = 2, _3 = 3, _4 = 4, _5 = 5, _6 = 6, _7 = 7, _8 = 8, _9 = 9, _10 = 10, J = 11, Q = 12, K = 13, A = 14 };
    private:
        Value value;

    public:
        Rank(const Rank& rank) : value(rank.value) {}
        Rank(uint8_t rank) : value(static_cast<Value>(rank)) {}
        Rank(const string& rank)
        {
            if (rank.size() == 2)
            {
                value = Value::_10;
                return;
            }

            if (rank.size() == 1)
            {
                if ('1' <= rank[0] and rank[0] <= '9')
                {
                    value = static_cast<Value>(rank[0] - '0');
                    return;
                }

                switch (rank[0])
                {
                case 'J':
                    value = Value::J;
                    return;
                case 'Q':
                    value = Value::Q;
                    return;
                case 'K':
                    value = Value::K;
                    return;
                case 'A':
                    value = Value::A;
                    return;
                default:
                    break;
                }
            }

            cout << "[ERROR] bad rank string: \"" << rank << "\"" << endl;
        }
        bool operator == (const Rank& other) { return value == other.value; }
        bool operator == (uint8_t other) { return value == static_cast<Value>(other); }
    };

    Suit suit;
    Rank rank;

    Card(const string& card) : suit(card[card.size() - 1]), rank(card.substr(0, card.size() - 1)) {}
};


string pokerHandRanking(vector<string> hand)
{
    return "";
}

int main()
{
    std::cout << "Hello World!\n";
}
