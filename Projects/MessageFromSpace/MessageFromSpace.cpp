#include <iostream>
#include <string>
#include <sstream>

using std::cout;

using std::string;
using std::stringstream;


struct SequenceIndex
{
    size_t start{ 0 };
    size_t stop{ 0 };
    bool exists{ false };
    bool nested{ false };

    size_t length() { return stop - start + 1; }
};

inline bool is_digit(char c) { return '0' <= c and c <= '9'; }

inline bool is_letter(char c) { return 'A' <= c and c <= 'Z'; }


SequenceIndex find_first_sequence(string msg, size_t start = 0)
{
    SequenceIndex result;

    int open_parens = 0;

    for (size_t i = start; i < msg.size(); i++)
    {
        if (open_parens == 0 and msg[i] == '[')
        {
            result.exists = true;
            result.start = i + 1;  // first real index, exclude the [
            open_parens = 1;
        }

        else if (open_parens > 0 and msg[i] == '[')
        {
            open_parens++;
            result.nested = true;
        }

        else if (open_parens > 1 and msg[i] == ']')
        {
            open_parens--;
        }

        else if (open_parens == 1 and msg[i] == ']')
        {
            result.stop = i - 1;  // last real index, exclude the ]
            break;
        }
    }

    return result;
}


string expand_sequence(string msg, SequenceIndex seq_ind)
{
    string seq = msg.substr(seq_ind.start, seq_ind.length());

    int num = 0;

    size_t i = 0;
    while (is_digit(seq[i]))
    {
        num = num * 10 + (seq[i] - '0');
        i++;
    }

    string seq_text = seq.substr(i, seq.length() - i);

    stringstream decrypted_msg;
    for (int j = 0; j < num; j++)
    {
        decrypted_msg << seq_text;
    }

    return decrypted_msg.str();
}


string decrypt(string msg)
{
    stringstream decrypted_msg;
    bool has_nesting = false;

    SequenceIndex seq_ind = find_first_sequence(msg);

    if (not seq_ind.exists)
        return msg;

    if (seq_ind.nested)
        has_nesting = true;

    size_t plaintext_start = 0;
    size_t plaintext_stop = seq_ind.start - 2;

    while (seq_ind.exists)
    {
        decrypted_msg << msg.substr(plaintext_start, plaintext_stop - plaintext_start + 1);
        decrypted_msg << expand_sequence(msg, seq_ind);

        plaintext_start = seq_ind.stop + 2;

        if (seq_ind.stop + 2 >= msg.length())
            break;
        else
        {
            seq_ind = find_first_sequence(msg, seq_ind.stop + 2);
            plaintext_stop = seq_ind.start - 2;
            if (seq_ind.nested)
                has_nesting = true;
        }
    }

    decrypted_msg << msg.substr(plaintext_start, msg.length() - plaintext_start);

    if (not has_nesting)
        return decrypted_msg.str();
    else
        return decrypt(decrypted_msg.str());
}


int main()
{
    cout << "Hello World!\n";
    cout << decrypt("ABC") << "\n";
    cout << decrypt("ABC[3DE]") << "\n";
    cout << decrypt("ABC[3DE]FG") << "\n";
    cout << decrypt("ABC[3DE]FG[2HIJ]") << "\n";
    cout << decrypt("ABC[3DE]FG[2HIJ]KL") << "\n";
    cout << decrypt("ABC[2DE[2FG]H]IJ[3K]LM") << "\n";
}
