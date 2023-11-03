#include <iostream>
#include <vector>
#include <unordered_set>
#include <queue>

using std::cout;
using std::endl;

using std::vector;
using std::unordered_set;
using std::queue;

class Island;
class IslandGroup;


struct Pos2D
{
	int x, y;

	bool operator==(const Pos2D& other) const
	{
		return x == other.x and y == other.y;
	}
};

namespace std 
{
	template<> struct hash<Pos2D>
	{
		std::size_t operator()(const Pos2D& pos) const noexcept
		{
			return pos.x * 1000000 + pos.y;
		}
	};
}


class Island
{
private:

	const vector<vector<int>>& map;
	int max_x, max_y;

	// service members
	unordered_set<Pos2D> visited_positions;
	vector<Pos2D> neighbours;
	queue<Pos2D> positions_to_visit;

	inline bool position_is_land(Pos2D pos) 
	{ 
		return map[pos.y][pos.x] == 1; 
	}

	inline bool position_visited(Pos2D pos)
	{
		return visited_positions.find(pos) != visited_positions.end();
	}

	inline void add_to_neighbours_if_land_and_not_visited(Pos2D pos)
	{
		if (not position_visited(pos) and position_is_land(pos))
			neighbours.push_back(pos);
	}
	vector<Pos2D>& get_unvisited_land_neighbours(Pos2D pos)
	{

		bool not_top_edge    = pos.y > 0;
		bool not_bottom_edge = pos.y < max_y;
		bool not_left_edge   = pos.x > 0;
		bool not_right_edge  = pos.x < max_x;

		if (not_top_edge)
		{
			if (not_left_edge)  // up left
				add_to_neighbours_if_land_and_not_visited({ pos.x - 1, pos.y - 1 });
				
			// up
			add_to_neighbours_if_land_and_not_visited({ pos.x, pos.y - 1 });

			if (not_right_edge)  // up right
			{
				add_to_neighbours_if_land_and_not_visited({ pos.x + 1, pos.y - 1 });
			}
		}

		if (not_left_edge)  // left
			add_to_neighbours_if_land_and_not_visited({ pos.x - 1, pos.y });

		if (not_right_edge)  // right
			add_to_neighbours_if_land_and_not_visited({ pos.x + 1, pos.y });

		if (not_bottom_edge)
		{
			if (not_left_edge)  // bottom left
				add_to_neighbours_if_land_and_not_visited({ pos.x - 1, pos.y + 1 });

			// bottom
			add_to_neighbours_if_land_and_not_visited({ pos.x, pos.y + 1 });

			if (not_right_edge)  // bottom right
				add_to_neighbours_if_land_and_not_visited({ pos.x + 1, pos.y + 1 });
		}

		return neighbours;
	}

	inline Pos2D pop_next_position_to_visit()
	{
		Pos2D location_to_visit = positions_to_visit.front();
		positions_to_visit.pop();
		return location_to_visit;
	}

public:

	Island(const vector<vector<int>>& map, Pos2D island_start) : 
		map(map), 
		max_x(map[0].size() - 1), 
		max_y(map.size() - 1)
	{
		positions_to_visit.push(island_start);

		while (not positions_to_visit.empty())
		{
			Pos2D position_to_visit = pop_next_position_to_visit();

			if (position_visited(position_to_visit))
				continue;

			visited_positions.insert(position_to_visit);

			vector<Pos2D> neighbours = get_unvisited_land_neighbours(position_to_visit);

			for (const Pos2D& neighbour : neighbours)
				positions_to_visit.push(neighbour);
		}
	}

	int size()
	{
		return visited_positions.size();
	}

	bool contains_position(Pos2D pos)
	{
		return position_visited(pos);
	}
};


class IslandGroup
{
private:
	const vector<vector<int>>& map;
	vector<Island> islands;

	inline bool position_is_land(Pos2D pos)
	{
		return map[pos.y][pos.x] == 1;
	}

public:
	IslandGroup(const vector<vector<int>>& map) : map(map) 
	{
		for (int row = 0; row < map.size(); row++)
		{
			for (int col = 0; col < map[0].size(); col++)
			{
				if (position_is_land({ col, row }))
				{
					add_island_from_location({ col, row });
				}
			}
		}
	}

	bool contains_position(Pos2D pos)
	{
		for (Island& island : islands)
		{
			if (island.contains_position(pos))
			{
				return true;
			}
		}
		return false;
	}

	void add_island_from_location(Pos2D pos)
	{
		if (contains_position(pos))
			return;

		islands.emplace_back(map, pos);
	}

	int largest_island_size()
	{
		int result = 0;
		for (Island& island : islands)
		{
			if (island.size() > result)
			{
				result = island.size();
			}
		}
		return result;
	}
};

int largestIsland(vector<vector<int>> map)
{
	return 1;
}

int main()
{
	vector<vector<int>> m = { { 1, 0, 1 },
							  { 1, 0, 0 },
							  { 1, 1, 0 } };

	IslandGroup ig{ m };

    cout << IslandGroup(m).largest_island_size() << " Hello World!\n";
}
