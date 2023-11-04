#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::vector;
using std::string;

vector<string> bucketize(string phrase, int n) 
{
    vector<string> result;

    size_t current_bucket_start{ 0 };
    size_t current_word_start{ 0 };
    
    for (size_t i = 0; i < phrase.length()+1; i++)
    {
        char c;
        if (i < phrase.length()) { c = phrase[i]; }
        else { c = ' '; }

        if (c == ' ')
        {
            current_word_start = i + 1;
        } 

        size_t bucket_size{ i - current_bucket_start + 1 };

        if (bucket_size > n or i == phrase.length())
        {
            if (current_word_start == current_bucket_start)
            {
                return {};
            }
            result.push_back(phrase.substr(current_bucket_start, current_word_start - 1 - current_bucket_start));
            current_bucket_start = current_word_start;
            cout << "-[ ";
            for (string& s : result)
            {
                cout << "\"" << s << "\" ";
            }
            cout << "]\n";
        }
    }

    return result;
}


int main()
{
    auto print = [](vector<string> v)
    {
        cout << "[ ";
        for (string& s : v)
        {
            cout << "\"" << s << "\" ";
        }
        cout << "]\n";
    };

    print(bucketize("she sells sea shells by the sea", 10));
    print(bucketize("the mouse jumped over the cheese", 7));
    print(bucketize("fairy dust coated the air", 20));
    print(bucketize("abc de", 2));

    cout << "Hello World!\n";
}
