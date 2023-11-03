#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <queue>
#include <cctype>
#include <sstream>

using std::cout;

using std::vector;
using std::string;
using std::unordered_set;
using std::queue;
using std::stringstream;


template <typename T>
bool is_in_set(const unordered_set<T>& set, const T& item)
{
    typename unordered_set<T>::const_iterator item_iter = set.find(item);
    return item_iter != set.end();
}

template <typename T>
T pop_queue_item(queue<T>& q)
{
    T item = q.front();
    q.pop();
    return item;
}

struct Pos2D { size_t x, y; };


class PlayfairSquare
{
private:
    struct Digraph
    {
        char c1;
        char c2;
        bool complete;

        Digraph() : c1(0), c2(0), complete(false) {}
        Digraph(char c1) : c1(c1), c2(0), complete(false) {}
        Digraph(char c1, char c2) : c1(c1), c2(c2), complete(true) {}

        size_t length() const
        {
            if (c1 == 0 and c2 == 0)
                return 0;
            else if (c1 != 0 and c2 == 0)
                return 1;
            else if (c1 != 0 and c2 != 0)
                return 2;
        }

        void add(char c)
        {
            if (not complete)
            {
                if (c1 == 0)
                    c1 = c;

                else if (c2 == 0)
                {
                    c2 = c;
                    complete = true;
                }
            }
        }
    };

    class DigraphList
    {
    private:
        vector<Digraph> digraphs;

        Digraph& last_digraph()
        {
            return digraphs[digraphs.size() - 1];
        }

    public:
        DigraphList() {}
        DigraphList(const string& str, bool resolve_repeting_chars = true)
        {
            for (char c : str)
                if (isalpha(c))
                    add(tolower(c), resolve_repeting_chars);
        }

        Digraph& operator[](size_t i)
        {
            return digraphs[i];
        }

        size_t size()
        {
            return digraphs.size();
        }

        void add(char c, bool resolve_repeating_chars = true)
        {
            if (digraphs.empty())
            {
                digraphs.emplace_back(c);
            }

            else if (last_digraph().complete)
            {
                digraphs.emplace_back(c);
            }

            else
            {
                if (resolve_repeating_chars and last_digraph().c1 == c)
                {
                    last_digraph().add('x');
                    digraphs.emplace_back(c);
                }
                else
                {
                    last_digraph().add(c);
                }
            }
        }

        string join_to_string()
        {
            stringstream result;
            for (const Digraph& d : digraphs)
                result << d.c1 << d.c2;
            return result.str();
        }

        void print()
        {
            cout << "DigraphList[ ";
            for (const Digraph& dg : digraphs)
                cout << dg.c1 << dg.c2 << " ";
            cout << "]\n";
        }

        
    };

    string keyword;
    vector<vector<char>> square;

    unordered_set<char> letters_in_keyword()
    {
        unordered_set<char> keyword_letters;
        for (char c : keyword)
            keyword_letters.insert(toupper(c));
        return keyword_letters;
    }

    queue<char> letters_not_in_keyword_ordered(const unordered_set<char>& keyword_letters)
    {
        queue<char> letters_left;
        for (char c = 'A'; c <= 'Z'; c++)
        {
            if (c == 'J')
                continue;

            if (not is_in_set(keyword_letters, c))
                letters_left.push(c);
        }
        return letters_left;
    }

    Pos2D find_char_pos(char c)
    {
        c = toupper(c);
        if (c == 'J')
            c = 'I';

        for (size_t row = 0; row < 5; row++)
        {
            for (size_t col = 0; col < 5; col++)
            {
                if (c == square[row][col])
                {
                    return { col, row };
                }
            }
        }
    }

    char get_next_horizontally(const Pos2D& pos, bool reverse = false)
    {
        size_t next_col;

        if (not reverse)
        {
            if (pos.x == 4)
                next_col = 0;
            else
                next_col = pos.x + 1;
        }
        else
        {
            if (pos.x == 0)
                next_col = 4;
            else
                next_col = pos.x - 1;
        }

        return square[pos.y][next_col];
    }

    char get_next_vertically(const Pos2D& pos, bool reverse = false)
    {
        size_t next_row;

        if (not reverse)
        {
            if (pos.y == 4)
                next_row = 0;
            else
                next_row = pos.y + 1;
        }
        else
        {
            if (pos.y == 0)
                next_row = 4;
            else
                next_row = pos.y - 1;
        }

        return square[next_row][pos.x];
    }

    Digraph process_digraph_same_line(const Pos2D& pos1, const Pos2D& pos2, bool reverse = false)
    {
        return { get_next_horizontally(pos1, reverse) , get_next_horizontally(pos2, reverse) };
    }

    Digraph process_digraph_same_column(const Pos2D& pos1, const Pos2D& pos2, bool reverse = false)
    {
        return { get_next_vertically(pos1, reverse) , get_next_vertically(pos2, reverse) };
    }

    Digraph process_digraph_diagonal(const Pos2D& pos1, const Pos2D& pos2)
    {
        size_t new_pos1_x = pos2.x;
        size_t new_pos2_x = pos1.x;
        return { square[pos1.y][new_pos1_x], square[pos2.y][new_pos2_x] };
    }

    Digraph process_digraph(const Digraph& digraph, bool reverse = false)
    {
        Pos2D pos1 = find_char_pos(digraph.c1);
        Pos2D pos2 = find_char_pos(digraph.c2);

        if (pos1.y == pos2.y)
            return process_digraph_same_line(pos1, pos2, reverse);

        if (pos1.x == pos2.x)
            return process_digraph_same_column(pos1, pos2, reverse);

        return process_digraph_diagonal(pos1, pos2);
    }

    bool is_plaintext(const string& str)
    {
        for (size_t i = 0; i < str.length(); i++)
            if (not ('A' <= str[i] and str[i] <= 'Z'))
                return true;
        return false;
    }

    string process_plaintext(const string& str)
    {
        DigraphList digraphs(str);

        for (size_t i = 0; i < digraphs.size(); i++)
            digraphs[i] = process_digraph(digraphs[i]);

        return digraphs.join_to_string();
    }


    string process_ciphertext(const string& str)
    {
        DigraphList digraphs(str, false);

        for (size_t i = 0; i < digraphs.size(); i++)
            digraphs[i] = process_digraph(digraphs[i], true);

        return digraphs.join_to_string();
    }


public:
    PlayfairSquare(const string& keyword) : keyword(keyword)
    {
        unordered_set<char> word_letters = letters_in_keyword();
        queue<char> letters_left = letters_not_in_keyword_ordered(word_letters);

        size_t word_pos = 0;
        size_t word_len = keyword.length();
        bool word_done = false;
        unordered_set<char> word_letters_used;

        for (size_t row = 0; row < 5; row++)
        {
            square.emplace_back();

            for (size_t col = 0; col < 5; col++)
            {
                if (not word_done)
                {
                    if (not is_in_set(word_letters_used, keyword[word_pos]))
                    {
                        square[row].push_back(toupper(keyword[word_pos]));
                        word_letters_used.insert(keyword[word_pos]);
                    }
                    else
                    {
                        col--;
                    }
                    word_pos++;

                    if (word_pos == word_len)
                        word_done = true;
                }
                else
                {
                    square[row].push_back(pop_queue_item(letters_left));
                }
            }
        }
    }

    string process(const string& str)
    {
        if (is_plaintext(str))
            return process_plaintext(str);
        else
            return process_ciphertext(str);
    }

    void print_square()
    {
        static string line = "+---+---+---+---+---+";

        cout << line << "\n";

        for (size_t row = 0; row < 5; row++)
        {
            cout << "|";

            for (size_t col = 0; col < 5; col++)
            {
                if (square[row][col] != 'I')
                    cout << " " << square[row][col] << " ";
                else
                    cout << "I/J";

                cout << "|";
            }

            cout << "\n" << line << "\n";
        }
    }
};


string playfair(string str, string keyword) 
{
    return PlayfairSquare(keyword).process(str);
}



int main()
{
    string str = "The eagle has landed.";
    
    PlayfairSquare s("rhythm");
    s.print_square();

    cout << "|" << s.process(str) << "|\n";
    cout << "===\n";
    cout << s.process(s.process(str)) << "\n";

    cout << "Hello World!\n";
}
