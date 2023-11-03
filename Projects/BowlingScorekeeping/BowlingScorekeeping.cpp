#include <iostream>
#include <vector>

using std::cout;
using std::endl;

using std::vector;


struct Frame
{
    enum class Type
    {
        INCOMPLETE,
        PARTIAL,
        NORMAL, 
        SPARE,
        STRIKE
    };
    
    int throw1{ -1 };
    int throw2{ -1 };
    Type type{ Type::INCOMPLETE };

    void add_throw(int pins)
    {
        if (throw1 == -1)  // throw 1 not found
        {
            throw1 = pins;
            type = Type::PARTIAL;

            if (pins == 10)
                type = Type::STRIKE;
        }

        else if (type == Type::PARTIAL)
        {
            throw2 = pins;
            if (throw1 + throw2 == 10)
                type = Type::SPARE;
            else
                type = Type::NORMAL;
        }
    }

    bool complete() const
    {
        return type != Type::INCOMPLETE and type != Type::PARTIAL;
    }

    int score() const
    {
        if (type == Type::STRIKE or type == Type::SPARE)
            return 10;
        else if (type == Type::NORMAL)
            return throw1 + throw2;
        else if (type == Type::PARTIAL)
            return throw1;
    }
};


struct GameScore
{
    vector<Frame> frames;
    int score;


    GameScore(vector<int> throws)
    {
        // Separate frames
        
        frames.emplace_back();
        int most_recent_frame = 0;

        for (int throw_ : throws)
        {
            frames[most_recent_frame].add_throw(throw_);

            if (frames[most_recent_frame].complete())
            {
                frames.emplace_back();
                most_recent_frame++;
            }
        }

        if (frames[most_recent_frame].type == Frame::Type::INCOMPLETE)
            frames.pop_back();

        // Compute score

        score = 0;

        for (int i = 0; i < 10; i++)
        {
            if (frames[i].type == Frame::Type::NORMAL)
                score += frames[i].score();

            else if (frames[i].type == Frame::Type::SPARE)
                score += frames[i].score() + frames[i + 1].throw1;

            else if (frames[i].type == Frame::Type::STRIKE)
            {
                score += frames[i].score() + frames[i + 1].throw1;
                if (frames[i + 1].type == Frame::Type::STRIKE)
                    score += frames[i + 2].throw1;
                else
                    score += frames[i + 1].throw2;
            }
        }
    }

    void print()
    {
        cout << "GameScore[ ";
        int i = 1;
        for (const Frame& frame : frames)
        {
            cout << i << "[";
            i++;
            if (frame.type == Frame::Type::NORMAL)
            {
                cout << frame.throw1 << ":" << frame.throw2;
            }
            else if (frame.type == Frame::Type::SPARE)
            {
                cout << frame.throw1 << ":/";
            }
            else
            {
                cout << "X";
            }
            cout << "] ";
        }
        cout << "]\n";
    }
};


int bowling(vector<int> pins) 
{
    GameScore s{ pins };
    s.print();

    return s.score;
}


int main()
{
    cout << bowling({ 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 }) << endl;  // 300

    cout << bowling({ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 }) << endl;  // 80

    cout << bowling({ 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 }) << endl;  // 150

    cout << bowling({ 10, 5, 5, 10, 5, 5, 10, 5, 5, 10, 5, 5, 10, 5, 5, 10 }) << endl;  // 200

    cout << "Hello World!\n";
}
