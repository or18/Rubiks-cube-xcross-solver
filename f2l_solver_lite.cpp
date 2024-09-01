#include <emscripten/bind.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <sstream>

// emcc f2l_solver_lite.cpp -o f2l_solver_lite.js -O3 -s TOTAL_MEMORY=104857600 -s WASM=1 --bind

struct State
{
	std::vector<int> cp;
	std::vector<int> co;
	std::vector<int> ep;
	std::vector<int> eo;

	State(std::vector<int> arg_cp = {0, 1, 2, 3, 4, 5, 6, 7}, std::vector<int> arg_co = {0, 0, 0, 0, 0, 0, 0, 0}, std::vector<int> arg_ep = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, std::vector<int> arg_eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}) : cp(arg_cp), co(arg_co), ep(arg_ep), eo(arg_eo) {}

	State apply_move(State move)
	{
		std::vector<int> new_cp;
		std::vector<int> new_co;
		std::vector<int> new_ep;
		std::vector<int> new_eo;
		for (int i = 0; i < 8; ++i)
		{
			int p = move.cp[i];
			new_cp.emplace_back(cp[p]);
			new_co.emplace_back((co[p] + move.co[i]) % 3);
		}
		for (int i = 0; i < 12; ++i)
		{
			int p = move.ep[i];
			new_ep.emplace_back(ep[p]);
			new_eo.emplace_back((eo[p] + move.eo[i]) % 2);
		}
		return State(new_cp, new_co, new_ep, new_eo);
	}

	State apply_move_edge(State move, int e)
	{
		std::vector<int> new_ep(12, -1);
		std::vector<int> new_eo(12, -1);
		auto it = std::find(ep.begin(), ep.end(), e);
		int index = std::distance(ep.begin(), it);
		it = std::find(move.ep.begin(), move.ep.end(), e);
		int index_next = std::distance(move.ep.begin(), it);
		new_ep[index_next] = e;
		new_eo[index_next] = (eo[index] + move.eo[index_next]) % 2;
		return State(cp, co, new_ep, new_eo);
	}

	State apply_move_corner(State move, int c)
	{
		std::vector<int> new_cp(8, -1);
		std::vector<int> new_co(8, -1);
		auto it = std::find(cp.begin(), cp.end(), c);
		int index = std::distance(cp.begin(), it);
		it = std::find(move.cp.begin(), move.cp.end(), c);
		int index_next = std::distance(move.cp.begin(), it);
		new_cp[index_next] = c;
		new_co[index_next] = (co[index] + move.co[index_next]) % 3;
		return State(new_cp, new_co, ep, eo);
	}
};

std::unordered_map<std::string, State> moves = {
	{"U", State({3, 0, 1, 2, 4, 5, 6, 7}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 2, 3, 7, 4, 5, 6, 8, 9, 10, 11}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})},
	{"U2", State({2, 3, 0, 1, 4, 5, 6, 7}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 2, 3, 6, 7, 4, 5, 8, 9, 10, 11}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})},
	{"U'", State({1, 2, 3, 0, 4, 5, 6, 7}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 2, 3, 5, 6, 7, 4, 8, 9, 10, 11}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})},
	{"D", State({0, 1, 2, 3, 5, 6, 7, 4}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 8}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})},
	{"D2", State({0, 1, 2, 3, 6, 7, 4, 5}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 2, 3, 4, 5, 6, 7, 10, 11, 8, 9}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})},
	{"D'", State({0, 1, 2, 3, 7, 4, 5, 6}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 2, 3, 4, 5, 6, 7, 11, 8, 9, 10}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})},
	{"L", State({4, 1, 2, 0, 7, 5, 6, 3}, {2, 0, 0, 1, 1, 0, 0, 2}, {11, 1, 2, 7, 4, 5, 6, 0, 8, 9, 10, 3}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})},
	{"L2", State({7, 1, 2, 4, 3, 5, 6, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {3, 1, 2, 0, 4, 5, 6, 11, 8, 9, 10, 7}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})},
	{"L'", State({3, 1, 2, 7, 0, 5, 6, 4}, {2, 0, 0, 1, 1, 0, 0, 2}, {7, 1, 2, 11, 4, 5, 6, 3, 8, 9, 10, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})},
	{"R", State({0, 2, 6, 3, 4, 1, 5, 7}, {0, 1, 2, 0, 0, 2, 1, 0}, {0, 5, 9, 3, 4, 2, 6, 7, 8, 1, 10, 11}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})},
	{"R2", State({0, 6, 5, 3, 4, 2, 1, 7}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 2, 1, 3, 4, 9, 6, 7, 8, 5, 10, 11}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})},
	{"R'", State({0, 5, 1, 3, 4, 6, 2, 7}, {0, 1, 2, 0, 0, 2, 1, 0}, {0, 9, 5, 3, 4, 1, 6, 7, 8, 2, 10, 11}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})},
	{"F", State({0, 1, 3, 7, 4, 5, 2, 6}, {0, 0, 1, 2, 0, 0, 2, 1}, {0, 1, 6, 10, 4, 5, 3, 7, 8, 9, 2, 11}, {0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0})},
	{"F2", State({0, 1, 7, 6, 4, 5, 3, 2}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 3, 2, 4, 5, 10, 7, 8, 9, 6, 11}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})},
	{"F'", State({0, 1, 6, 2, 4, 5, 7, 3}, {0, 0, 1, 2, 0, 0, 2, 1}, {0, 1, 10, 6, 4, 5, 2, 7, 8, 9, 3, 11}, {0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0})},
	{"B", State({1, 5, 2, 3, 0, 4, 6, 7}, {1, 2, 0, 0, 2, 1, 0, 0}, {4, 8, 2, 3, 1, 5, 6, 7, 0, 9, 10, 11}, {1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0})},
	{"B2", State({5, 4, 2, 3, 1, 0, 6, 7}, {0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 2, 3, 8, 5, 6, 7, 4, 9, 10, 11}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})},
	{"B'", State({4, 0, 2, 3, 5, 1, 6, 7}, {1, 2, 0, 0, 2, 1, 0, 0}, {8, 4, 2, 3, 0, 5, 6, 7, 1, 9, 10, 11}, {1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0})}};

std::vector<std::string> move_names = {"U", "U2", "U'", "D", "D2", "D'", "L", "L2", "L'", "R", "R2", "R'", "F", "F2", "F'", "B", "B2", "B'"};

std::string AlgToString(std::vector<int> alg)
{
	std::string result = "";
	for (int i : alg)
	{
		result += move_names[i] + " ";
	}
	return result;
}

std::vector<int> StringToAlg(std::string str)
{
	std::vector<int> alg;
	std::istringstream iss(str);
	std::string name;
	while (iss >> name)
	{
		if (!name.empty())
		{
			auto it = std::find(move_names.begin(), move_names.end(), name);
			if (it != move_names.end())
			{
				alg.push_back(std::distance(move_names.begin(), it));
			}
		}
	}
	return alg;
}

std::vector<int> AlgConvertRotation(std::vector<int> alg, std::string rotation)
{
	if (rotation.empty())
	{
		return alg;
	}
	std::vector<int> face_list_0 = {0, 1, 2, 3, 4, 5};
	std::vector<int> face_list;
	if (rotation == "x")
	{
		face_list = {5, 4, 2, 3, 0, 1};
	}
	else if (rotation == "x2")
	{
		face_list = {1, 0, 2, 3, 5, 4};
	}
	else if (rotation == "x'")
	{
		face_list = {4, 5, 2, 3, 1, 0};
	}
	else if (rotation == "y")
	{
		face_list = {0, 1, 5, 4, 2, 3};
	}
	else if (rotation == "y2")
	{
		face_list = {0, 1, 3, 2, 5, 4};
	}
	else if (rotation == "y'")
	{
		face_list = {0, 1, 4, 5, 3, 2};
	}
	else if (rotation == "z")
	{
		face_list = {3, 2, 0, 1, 4, 5};
	}
	else if (rotation == "z2")
	{
		face_list = {1, 0, 3, 2, 4, 5};
	}
	else if (rotation == "z'")
	{
		face_list = {2, 3, 1, 0, 4, 5};
	}
	for (size_t i = 0; i < alg.size(); ++i)
	{
		alg[i] = 3 * face_list[alg[i] / 3] + alg[i] % 3;
	}
	return alg;
}

std::vector<int> AlgRotation(std::vector<int> alg, std::string rotation_algString)
{
	std::istringstream iss(rotation_algString);
	std::string rot;
	while (iss >> rot)
	{
		alg = AlgConvertRotation(alg, rot);
	}
	return alg;
}

std::vector<std::vector<int>> c_array = {{0}, {1, 1, 1, 1, 1}, {1, 2, 4, 8, 16, 32}, {1, 3, 9, 27, 81, 243}};

std::vector<std::vector<int>> base_array = {{0}, {0}, {1, 12, 12 * 11, 12 * 11 * 10, 12 * 11 * 10 * 9}, {1, 8, 8 * 7, 8 * 7 * 6, 8 * 7 * 6 * 5}};

inline int array_to_index(std::vector<int> &a, int n, int c, int pn)
{
	int index_p = 0;
	int index_o = 0;
	int tmp;
	int tmp2 = 24 / pn;
	for (int i = 0; i < n; ++i)
	{
		index_o += (a[i] % c) * c_array[c][n - i - 1];
		a[i] /= c;
	}
	for (int i = 0; i < n; ++i)
	{
		tmp = 0;
		for (int j = 0; j < i; ++j)
		{
			if (a[j] < a[i])
			{
				tmp++;
			}
		}
		index_p += (a[i] - tmp) * base_array[tmp2][i];
	}
	return index_p * c_array[c][n] + index_o;
}

std::vector<int> sorted(5);

std::vector<std::vector<int>> base_array2 = {{0}, {0}, {12, 11, 10, 9, 8}, {8, 7, 6, 5, 4}};

inline void index_to_array(std::vector<int> &p, int index, int n, int c, int pn)
{
	int tmp2 = 24 / pn;
	int p_index = index / c_array[c][n];
	int o_index = index % c_array[c][n];
	for (int i = 0; i < n; ++i)
	{
		p[i] = p_index % base_array2[tmp2][i];
		p_index /= base_array2[tmp2][i];
		std::sort(sorted.begin(), sorted.begin() + i);
		for (int j = 0; j < i; ++j)
		{
			if (sorted[j] <= p[i])
			{
				p[i] += 1;
			}
		}
		sorted[i] = p[i];
	}
	for (int i = 0; i < n; ++i)
	{
		p[n - i - 1] = 18 * (c * p[n - i - 1] + o_index % c);
		o_index /= c;
	}
}

std::vector<int> create_edge_move_table()
{
	std::vector<int> move_table(24 * 18, -1);
	int index;
	for (int i = 0; i < 24; ++i)
	{
		std::vector<int> ep(12, -1);
		std::vector<int> eo(12, -1);
		std::vector<int> cp(8, 0);
		std::vector<int> co(8, 0);
		ep[i / 2] = i / 2;
		eo[i / 2] = i % 2;
		State state(cp, co, ep, eo);
		for (int j = 0; j < 18; ++j)
		{
			State new_state = state.apply_move_edge(moves[move_names[j]], i / 2);
			auto it = std::find(new_state.ep.begin(), new_state.ep.end(), i / 2);
			index = std::distance(new_state.ep.begin(), it);
			move_table[18 * i + j] = 2 * index + new_state.eo[index];
		}
	}
	return move_table;
}

std::vector<int> create_corner_move_table()
{
	std::vector<int> move_table(24 * 18, -1);
	int index;
	for (int i = 0; i < 24; ++i)
	{
		std::vector<int> ep(12, 0);
		std::vector<int> eo(12, 0);
		std::vector<int> cp(8, -1);
		std::vector<int> co(8, -1);
		cp[i / 3] = i / 3;
		co[i / 3] = i % 3;
		State state(cp, co, ep, eo);
		for (int j = 0; j < 18; ++j)
		{
			State new_state = state.apply_move_corner(moves[move_names[j]], i / 3);
			auto it = std::find(new_state.cp.begin(), new_state.cp.end(), i / 3);
			index = std::distance(new_state.cp.begin(), it);
			move_table[18 * i + j] = 3 * index + new_state.co[index];
		}
	}
	return move_table;
}

std::vector<int> create_multi_move_table(int n, int c, int pn, int size, const std::vector<int> &table)
{
	std::vector<int> move_table(size * 18, -1);
	int tmp;
	int tmp_i;
	std::vector<int> a(n);
	std::vector<int> b(n);
	std::vector<int> inv_move = {2, 1, 0, 5, 4, 3, 8, 7, 6, 11, 10, 9, 14, 13, 12, 17, 16, 15};
	for (int i = 0; i < size; ++i)
	{
		index_to_array(a, i, n, c, pn);
		tmp_i = i * 18;
		for (int j = 0; j < 18; ++j)
		{
			if (move_table[tmp_i + j] == -1)
			{
				for (int k = 0; k < n; ++k)
				{
					b[k] = table[a[k] + j];
				}
				tmp = array_to_index(b, n, c, pn);
				move_table[tmp_i + j] = tmp;
				move_table[18 * tmp + inv_move[j]] = i;
			}
		}
	}
	return move_table;
}

std::vector<int> create_multi_move_table2(int n, int c, int pn, int size, const std::vector<int> &table)
{
	std::vector<int> move_table(size * 24, -1);
	int tmp;
	int tmp_i;
	std::vector<int> a(n);
	std::vector<int> b(n);
	std::vector<int> inv_move = {2, 1, 0, 5, 4, 3, 8, 7, 6, 11, 10, 9, 14, 13, 12, 17, 16, 15};
	for (int i = 0; i < size; ++i)
	{
		index_to_array(a, i, n, c, pn);
		tmp_i = i * 24;
		for (int j = 0; j < 18; ++j)
		{
			if (move_table[tmp_i + j] == -1)
			{
				for (int k = 0; k < n; ++k)
				{
					b[k] = table[a[k] + j];
				}
				tmp = 24 * array_to_index(b, n, c, pn);
				move_table[tmp_i + j] = tmp;
				move_table[tmp + inv_move[j]] = tmp_i;
			}
		}
	}
	return move_table;
}

std::vector<int> create_prune_table(int index1, int index2, int size1, int size2, int depth, const std::vector<int> &table1, const std::vector<int> &table2)
{
	int size = size1 * size2;
	std::vector<int> prune_table(size, -1);
	int start = index1 * size2 + index2;
	int next_i;
	int index1_tmp;
	int index2_tmp;
	int index1_tmp_end;
	int index2_tmp_end;
	int next_d;
	prune_table[start] = 0;
	for (int d = 0; d < depth; ++d)
	{
		next_d = d + 1;
		for (int i = start; i < size; ++i)
		{
			if (prune_table[i] == d)
			{
				index1_tmp = (i / size2) * 18;
				index2_tmp = (i % size2) * 18;
				index1_tmp_end = index1_tmp + 18;
				index2_tmp_end = index2_tmp + 18;
				for (int j = index1_tmp, k = index2_tmp; j < index1_tmp_end && k < index2_tmp_end; ++j, ++k)
				{
					next_i = table1[j] * size2 + table2[k];
					if (prune_table[next_i] == -1)
					{
						prune_table[next_i] = next_d;
					}
				}
			}
		}
		for (int i = 0; i < start; ++i)
		{
			if (prune_table[i] == d)
			{
				index1_tmp = (i / size2) * 18;
				index2_tmp = (i % size2) * 18;
				index1_tmp_end = index1_tmp + 18;
				index2_tmp_end = index2_tmp + 18;
				for (int j = index1_tmp, k = index2_tmp; j < index1_tmp_end && k < index2_tmp_end; ++j, ++k)
				{
					next_i = table1[j] * size2 + table2[k];
					if (prune_table[next_i] == -1)
					{
						prune_table[next_i] = next_d;
					}
				}
			}
		}
	}
	return prune_table;
}

std::vector<int> create_prune_table2(int index1, int index2, int size1, int size2, int depth, const std::vector<int> &table1, const std::vector<int> &table2)
{
	int size = size1 * size2;
	std::vector<int> prune_table(size, -1);
	int start = index1 * size2 + index2;
	int next_i;
	int index1_tmp;
	int index2_tmp;
	int index1_tmp_end;
	int index2_tmp_end;
	int next_d;
	prune_table[start] = 0;
	index1_tmp = index1 * 24;
	index2_tmp = index2 * 18;
	index1_tmp_end = index1_tmp + 18;
	index2_tmp_end = index2_tmp + 18;
	for (int j = index1_tmp, k = index2_tmp; j < index1_tmp_end && k < index2_tmp_end; ++j, ++k)
	{
		next_i = table1[j] + table2[k];
		if (prune_table[next_i] == -1)
		{
			prune_table[next_i] = 1;
		}
	}
	for (int d = 1; d < depth; ++d)
	{
		next_d = d + 1;
		for (int i = start; i < size; ++i)
		{
			if (prune_table[i] == d)
			{
				index1_tmp = (i / size2) * 24;
				index2_tmp = (i % size2) * 18;
				index1_tmp_end = index1_tmp + 18;
				index2_tmp_end = index2_tmp + 18;
				for (int j = index1_tmp, k = index2_tmp; j < index1_tmp_end && k < index2_tmp_end; ++j, ++k)
				{
					next_i = table1[j] + table2[k];
					if (prune_table[next_i] == -1)
					{
						prune_table[next_i] = next_d;
					}
				}
			}
		}
		for (int i = 0; i < start; ++i)
		{
			if (prune_table[i] == d)
			{
				index1_tmp = (i / size2) * 24;
				index2_tmp = (i % size2) * 18;
				index1_tmp_end = index1_tmp + 18;
				index2_tmp_end = index2_tmp + 18;
				for (int j = index1_tmp, k = index2_tmp; j < index1_tmp_end && k < index2_tmp_end; ++j, ++k)
				{
					next_i = table1[j] + table2[k];
					if (prune_table[next_i] == -1)
					{
						prune_table[next_i] = next_d;
					}
				}
			}
		}
	}
	return prune_table;
}

void create_prune_table2_thread(std::vector<int> &prune_table, int index1, int index2, int size1, int size2, int depth, const std::vector<int> &table1, const std::vector<int> &table2)
{
	prune_table = create_prune_table2(index1, index2, size1, size2, depth, table1, table2);
}

std::vector<bool> create_ma_table()
{
	std::vector<bool> ma;
	bool condition;
	for (int prev = 0; prev < 19; ++prev)
	{
		for (int i = 0; i < 18; ++i)
		{
			condition = (prev < 18) &&
						(i / 3 == prev / 3 ||
						 ((i / 3) / 2 == (prev / 3) / 2 && (prev / 3) % 2 > (i / 3) % 2));
			ma.push_back(condition);
		}
	}
	return ma;
}

struct cross_search
{
	std::vector<int> sol;
	std::ostringstream result;
	std::string scramble;
	std::string rotation;
	int max_length;
	int sol_num;
	int count;
	std::vector<int> edge_move_table;
	std::vector<int> multi_move_table;
	std::vector<int> prune_table;
	std::vector<int> alg;
	std::vector<std::string> restrict;
	std::vector<int> move_restrict;
	std::vector<bool> ma;
	int index1;
	int index2;
	int index1_tmp;
	int index2_tmp;
	int prune_tmp;

	cross_search()
	{
		edge_move_table = create_edge_move_table();
		multi_move_table = create_multi_move_table(2, 2, 12, 24 * 22, edge_move_table);
		index1 = 416;
		index2 = 520;
		prune_table = create_prune_table(index1, index2, 24 * 22, 24 * 22, 9, multi_move_table, multi_move_table);
		ma = create_ma_table();
	}

	bool depth_limited_search(int arg_index1, int arg_index2, int depth, int prev)
	{
		for (int i : move_restrict)
		{
			if (ma[prev + i])
			{
				continue;
			}
			index1_tmp = multi_move_table[arg_index1 + i];
			index2_tmp = multi_move_table[arg_index2 + i];
			prune_tmp = prune_table[index1_tmp * 528 + index2_tmp];
			if (prune_tmp >= depth)
			{
				continue;
			}
			sol.push_back(i);
			if (depth == 1)
			{
				if (prune_tmp == 0)
				{
					count += 1;
					if (rotation == "")
					{
						result << count << ": " << AlgToString(sol) << "\n";
					}
					else
					{
						result << count << ": " << rotation + " " << AlgToString(sol) << "\n";
					}
					if (count == sol_num)
					{
						return true;
					}
				}
			}
			else if (depth_limited_search(index1_tmp * 18, index2_tmp * 18, depth - 1, i * 18))
			{
				return true;
			}
			sol.pop_back();
		}
		return false;
	}

	std::string start_search(std::string arg_scramble = "", std::string arg_rotation = "", int arg_sol_num = 100, int arg_max_length = 8, std::vector<std::string> arg_restrict = move_names)
	{
		scramble = arg_scramble;
		rotation = arg_rotation;
		max_length = arg_max_length;
		sol_num = arg_sol_num;
		restrict = arg_restrict;
		std::vector<int> alg = AlgRotation(StringToAlg(scramble), rotation);
		index1 = 416;
		index2 = 520;
		count = 0;
		for (int m : alg)
		{
			index1 = multi_move_table[index1 * 18 + m];
			index2 = multi_move_table[index2 * 18 + m];
		}
		for (std::string name : restrict)
		{
			auto it = std::find(move_names.begin(), move_names.end(), name);
			move_restrict.push_back(std::distance(move_names.begin(), it));
		}
		prune_tmp = prune_table[index1 * 528 + index2];
		if (prune_tmp == 0)
		{
			result << "already solved\n";
			return result.str();
		}
		index1 *= 18;
		index2 *= 18;
		for (int d = 1; d <= max_length; d++)
		{
			if (depth_limited_search(index1, index2, d, 324))
			{
				break;
			}
		}
		return result.str();
	}
};

struct xcross_search
{
	std::vector<int> sol;
	std::ostringstream result;
	std::string scramble;
	std::string rotation;
	int slot1;
	int max_length;
	int sol_num;
	int count;
	std::vector<int> edge_move_table;
	std::vector<int> corner_move_table;
	std::vector<int> multi_move_table;
	std::vector<int> prune_table1;
	std::vector<int> alg;
	std::vector<std::string> restrict;
	std::vector<int> move_restrict;
	std::vector<bool> ma;
	int edge_solved1;
	int index1;
	int index2;
	int index3;
	int index1_tmp;
	int index2_tmp;
	int index3_tmp;
	int prune1_tmp;

	xcross_search()
	{
		edge_move_table = create_edge_move_table();
		corner_move_table = create_corner_move_table();
		multi_move_table = create_multi_move_table2(4, 2, 12, 24 * 22 * 20 * 18, edge_move_table);
		ma = create_ma_table();
	}

	bool depth_limited_search(int arg_index1, int arg_index2, int arg_index3, int depth, int prev)
	{
		for (int i : move_restrict)
		{
			if (ma[prev + i])
			{
				continue;
			}
			index1_tmp = multi_move_table[arg_index1 + i];
			index2_tmp = corner_move_table[arg_index2 + i];
			index3_tmp = edge_move_table[arg_index3 + i];
			prune1_tmp = prune_table1[index1_tmp + index2_tmp];
			if (prune1_tmp >= depth)
			{
				continue;
			}
			sol.push_back(i);
			if (depth == 1)
			{
				if (prune1_tmp == 0 && index3_tmp == edge_solved1)
				{
					count += 1;
					if (rotation == "")
					{
						result << count << ": " << AlgToString(sol) << "\n";
					}
					else
					{
						result << count << ": " << rotation + " " << AlgToString(sol) << "\n";
					}
					if (count == sol_num)
					{
						return true;
					}
				}
			}
			else if (depth_limited_search(index1_tmp, index2_tmp * 18, index3_tmp * 18, depth - 1, i * 18))
			{
				return true;
			}
			sol.pop_back();
		}
		return false;
	}

	std::string start_search(std::string arg_scramble = "", std::string arg_rotation = "", int arg_slot1 = 0, int arg_sol_num = 100, int arg_max_length = 10, std::vector<std::string> arg_restrict = move_names)
	{
		scramble = arg_scramble;
		rotation = arg_rotation;
		slot1 = arg_slot1;
		max_length = arg_max_length;
		sol_num = arg_sol_num;
		restrict = arg_restrict;
		std::vector<int> alg = AlgRotation(StringToAlg(scramble), rotation);
		std::vector<int> edge_index = {187520, 187520, 187520, 187520};
		std::vector<int> single_edge_index = {0, 2, 4, 6};
		std::vector<int> corner_index = {12, 15, 18, 21};
		index1 = edge_index[slot1];
		index2 = corner_index[slot1];
		index3 = single_edge_index[slot1];
		edge_solved1 = index3;
		prune_table1 = create_prune_table2(index1, index2, 24 * 22 * 20 * 18, 24, 9, multi_move_table, corner_move_table);
		count = 0;
		for (std::string name : restrict)
		{
			auto it = std::find(move_names.begin(), move_names.end(), name);
			move_restrict.push_back(std::distance(move_names.begin(), it));
		}
		index1 *= 24;
		for (int m : alg)
		{
			index1 = multi_move_table[index1 + m];
			index2 = corner_move_table[index2 * 18 + m];
			index3 = edge_move_table[index3 * 18 + m];
		}
		prune1_tmp = prune_table1[index1 + index2];
		if (prune1_tmp == 0 && index3 == edge_solved1)
		{
			result << "already solved\n";
			return result.str();
		}
		index2 *= 18;
		index3 *= 18;
		for (int d = 1; d <= max_length; d++)
		{
			if (depth_limited_search(index1, index2, index3, d, 324))
			{
				break;
			}
		}
		return result.str();
	}
};

struct xxcross_search
{
	std::vector<int> sol;
	std::ostringstream result;
	std::string scramble;
	std::string rotation;
	int slot1;
	int slot2;
	int max_length;
	int sol_num;
	int count;
	std::vector<int> edge_move_table;
	std::vector<int> corner_move_table;
	std::vector<int> multi_move_table;
	std::vector<int> prune_table1;
	std::vector<int> prune_table2;
	std::vector<int> alg;
	std::vector<std::string> restrict;
	std::vector<int> move_restrict;
	std::vector<bool> ma;
	int edge_solved1;
	int edge_solved2;
	int index1;
	int index2;
	int index3;
	int index4;
	int index5;
	int index6;
	int index1_tmp;
	int index2_tmp;
	int index3_tmp;
	int index4_tmp;
	int index5_tmp;
	int index6_tmp;
	int prune1_tmp;
	int prune2_tmp;

	xxcross_search()
	{
		edge_move_table = create_edge_move_table();
		corner_move_table = create_corner_move_table();
		multi_move_table = create_multi_move_table2(4, 2, 12, 24 * 22 * 20 * 18, edge_move_table);
		ma = create_ma_table();
	}

	bool depth_limited_search(int arg_index1, int arg_index2, int arg_index3, int arg_index4, int arg_index5, int arg_index6, int depth, int prev)
	{
		for (int i : move_restrict)
		{
			if (ma[prev + i])
			{
				continue;
			}
			index1_tmp = multi_move_table[arg_index1 + i];
			index2_tmp = corner_move_table[arg_index2 + i];
			index5_tmp = edge_move_table[arg_index5 + i];
			prune1_tmp = prune_table1[index1_tmp + index2_tmp];
			if (prune1_tmp >= depth)
			{
				continue;
			}
			index3_tmp = multi_move_table[arg_index3 + i];
			index4_tmp = corner_move_table[arg_index4 + i];
			index6_tmp = edge_move_table[arg_index6 + i];
			prune2_tmp = prune_table2[index3_tmp + index4_tmp];
			if (prune2_tmp >= depth)
			{
				continue;
			}
			sol.push_back(i);
			if (depth == 1)
			{
				if (prune1_tmp == 0 && prune2_tmp == 0 && index5_tmp == edge_solved1 && index6_tmp == edge_solved2)
				{
					count += 1;
					if (rotation == "")
					{
						result << ": " << AlgToString(sol) << "\n";
					}
					else
					{
						result << count << ": " << rotation + " " << AlgToString(sol) << "\n";
					}
					if (count == sol_num)
					{
						return true;
					}
				}
			}
			else if (depth_limited_search(index1_tmp, index2_tmp * 18, index3_tmp, index4_tmp * 18, index5_tmp * 18, index6_tmp * 18, depth - 1, i * 18))
			{
				return true;
			}
			sol.pop_back();
		}
		return false;
	}

	std::string start_search(std::string arg_scramble = "", std::string arg_rotation = "", int arg_slot1 = 0, int arg_slot2 = 3, int arg_sol_num = 100, int arg_max_length = 12, std::vector<std::string> arg_restrict = move_names)
	{
		scramble = arg_scramble;
		rotation = arg_rotation;
		slot1 = arg_slot1;
		slot2 = arg_slot2;
		max_length = arg_max_length;
		sol_num = arg_sol_num;
		restrict = arg_restrict;
		std::vector<int> alg = AlgRotation(StringToAlg(scramble), rotation);
		std::vector<int> edge_index = {187520, 187520, 187520, 187520};
		std::vector<int> single_edge_index = {0, 2, 4, 6};
		std::vector<int> corner_index = {12, 15, 18, 21};
		index1 = edge_index[slot1];
		index2 = corner_index[slot1];
		index5 = single_edge_index[slot1];
		edge_solved1 = index5;
		prune_table1 = create_prune_table2(index1, index2, 24 * 22 * 20 * 18, 24, 9, multi_move_table, corner_move_table);
		index3 = edge_index[slot2];
		index4 = corner_index[slot2];
		index6 = single_edge_index[slot2];
		edge_solved2 = index6;
		prune_table2 = create_prune_table2(index3, index4, 24 * 22 * 20 * 18, 24, 9, multi_move_table, corner_move_table);
		count = 0;
		for (std::string name : restrict)
		{
			auto it = std::find(move_names.begin(), move_names.end(), name);
			move_restrict.push_back(std::distance(move_names.begin(), it));
		}
		index1 *= 24;
		index3 *= 24;
		for (int m : alg)
		{
			index1 = multi_move_table[index1 + m];
			index2 = corner_move_table[index2 * 18 + m];
			index3 = multi_move_table[index3 + m];
			index4 = corner_move_table[index4 * 18 + m];
			index5 = edge_move_table[index5 * 18 + m];
			index6 = edge_move_table[index6 * 18 + m];
		}
		prune1_tmp = prune_table1[index1 + index2];
		prune2_tmp = prune_table2[index3 + index4];
		if (prune1_tmp == 0 && prune2_tmp == 0 && index5 == edge_solved1 && index6 == edge_solved2)
		{
			std::cout << "already solved\n";
			return result.str();
		}
		index2 *= 18;
		index4 *= 18;
		index5 *= 18;
		index6 *= 18;
		for (int d = 1; d <= max_length; d++)
		{
			if (depth_limited_search(index1, index2, index3, index4, index5, index6, d, 324))
			{
				break;
			}
		}
		return result.str();
	}
};

struct xxxcross_search
{
	std::vector<int> sol;
	std::ostringstream result;
	std::string scramble;
	std::string rotation;
	int slot1;
	int slot2;
	int slot3;
	int max_length;
	int sol_num;
	int count;
	std::vector<int> edge_move_table;
	std::vector<int> corner_move_table;
	std::vector<int> multi_move_table;
	std::vector<int> prune_table1;
	std::vector<int> prune_table2;
	std::vector<int> prune_table3;
	std::vector<int> alg;
	std::vector<std::string> restrict;
	std::vector<int> move_restrict;
	std::vector<bool> ma;
	int edge_solved1;
	int edge_solved2;
	int edge_solved3;
	int index1;
	int index2;
	int index3;
	int index4;
	int index5;
	int index6;
	int index7;
	int index8;
	int index9;
	int index1_tmp;
	int index2_tmp;
	int index3_tmp;
	int index4_tmp;
	int index5_tmp;
	int index6_tmp;
	int index7_tmp;
	int index8_tmp;
	int index9_tmp;
	int prune1_tmp;
	int prune2_tmp;
	int prune3_tmp;

	xxxcross_search()
	{
		edge_move_table = create_edge_move_table();
		corner_move_table = create_corner_move_table();
		multi_move_table = create_multi_move_table2(4, 2, 12, 24 * 22 * 20 * 18, edge_move_table);
		ma = create_ma_table();
	}

	bool depth_limited_search(int arg_index1, int arg_index2, int arg_index3, int arg_index4, int arg_index5, int arg_index6, int arg_index7, int arg_index8, int arg_index9, int depth, int prev)
	{
		for (int i : move_restrict)
		{
			if (ma[prev + i])
			{
				continue;
			}
			index1_tmp = multi_move_table[arg_index1 + i];
			index2_tmp = corner_move_table[arg_index2 + i];
			index7_tmp = edge_move_table[arg_index7 + i];
			prune1_tmp = prune_table1[index1_tmp + index2_tmp];
			if (prune1_tmp >= depth)
			{
				continue;
			}
			index3_tmp = multi_move_table[arg_index3 + i];
			index4_tmp = corner_move_table[arg_index4 + i];
			index8_tmp = edge_move_table[arg_index8 + i];
			prune2_tmp = prune_table2[index3_tmp + index4_tmp];
			if (prune2_tmp >= depth)
			{
				continue;
			}
			index5_tmp = multi_move_table[arg_index5 + i];
			index6_tmp = corner_move_table[arg_index6 + i];
			index9_tmp = edge_move_table[arg_index9 + i];
			prune3_tmp = prune_table3[index5_tmp + index6_tmp];
			if (prune3_tmp >= depth)
			{
				continue;
			}
			sol.push_back(i);
			if (depth == 1)
			{
				if (prune1_tmp == 0 && prune2_tmp == 0 && prune3_tmp == 0 && index7_tmp == edge_solved1 && index8_tmp == edge_solved2 && index9_tmp == edge_solved3)
				{
					count += 1;
					if (rotation == "")
					{
						result << count << ": " << AlgToString(sol) << "\n";
					}
					else
					{
						result << count << ": " << rotation + " " << AlgToString(sol) << "\n";
					}
					if (count == sol_num)
					{
						return true;
					}
				}
			}
			else if (depth_limited_search(index1_tmp, index2_tmp * 18, index3_tmp, index4_tmp * 18, index5_tmp, index6_tmp * 18, index7_tmp * 18, index8_tmp * 18, index9_tmp * 18, depth - 1, i * 18))
			{
				return true;
			}
			sol.pop_back();
		}
		return false;
	}

	std::string start_search(std::string arg_scramble = "", std::string arg_rotation = "", int arg_slot1 = 0, int arg_slot2 = 3, int arg_slot3 = 1, int arg_sol_num = 100, int arg_max_length = 14, std::vector<std::string> arg_restrict = move_names)
	{
		scramble = arg_scramble;
		rotation = arg_rotation;
		slot1 = arg_slot1;
		slot2 = arg_slot2;
		slot3 = arg_slot3;
		max_length = arg_max_length;
		sol_num = arg_sol_num;
		restrict = arg_restrict;
		std::vector<int> alg = AlgRotation(StringToAlg(scramble), rotation);
		std::vector<int> edge_index = {187520, 187520, 187520, 187520};
		std::vector<int> single_edge_index = {0, 2, 4, 6};
		std::vector<int> corner_index = {12, 15, 18, 21};
		index1 = edge_index[slot1];
		index2 = corner_index[slot1];
		index7 = single_edge_index[slot1];
		edge_solved1 = index7;
		prune_table1 = create_prune_table2(index1, index2, 24 * 22 * 20 * 18, 24, 9, multi_move_table, corner_move_table);
		index3 = edge_index[slot2];
		index4 = corner_index[slot2];
		index8 = single_edge_index[slot2];
		edge_solved2 = index8;
		prune_table2 = create_prune_table2(index3, index4, 24 * 22 * 20 * 18, 24, 9, multi_move_table, corner_move_table);
		index5 = edge_index[slot3];
		index6 = corner_index[slot3];
		index9 = single_edge_index[slot3];
		edge_solved3 = index9;
		prune_table3 = create_prune_table2(index5, index6, 24 * 22 * 20 * 18, 24, 9, multi_move_table, corner_move_table);
		count = 0;
		for (std::string name : restrict)
		{
			auto it = std::find(move_names.begin(), move_names.end(), name);
			move_restrict.push_back(std::distance(move_names.begin(), it));
		}
		index1 *= 24;
		index3 *= 24;
		index5 *= 24;
		for (int m : alg)
		{
			index1 = multi_move_table[index1 + m];
			index2 = corner_move_table[index2 * 18 + m];
			index3 = multi_move_table[index3 + m];
			index4 = corner_move_table[index4 * 18 + m];
			index5 = multi_move_table[index5 + m];
			index6 = corner_move_table[index6 * 18 + m];
			index7 = edge_move_table[index7 * 18 + m];
			index8 = edge_move_table[index8 * 18 + m];
			index9 = edge_move_table[index9 * 18 + m];
		}
		prune1_tmp = prune_table1[index1 + index2];
		prune2_tmp = prune_table2[index3 + index4];
		prune3_tmp = prune_table3[index5 + index6];
		if (prune1_tmp == 0 && prune2_tmp == 0 && prune3_tmp == 0 && index7 == edge_solved1 && index8 == edge_solved2 && index9 == edge_solved3)
		{
			std::cout << "already solved\n";
			return result.str();
		}
		index2 *= 18;
		index4 *= 18;
		index6 *= 18;
		index7 *= 18;
		index8 *= 18;
		index9 *= 18;
		for (int d = 1; d <= max_length; d++)
		{
			if (depth_limited_search(index1, index2, index3, index4, index5, index6, index7, index8, index9, d, 324))
			{
				break;
			}
		}
		return result.str();
	}
};

std::vector<bool> F2L_option_array(const std::string &input)
{
	std::vector<bool> result(4, false);
	std::istringstream iss(input);
	std::string token;

	while (iss >> token)
	{
		if (token == "BL")
		{
			result[0] = true;
		}
		else if (token == "BR")
		{
			result[1] = true;
		}
		else if (token == "FR")
		{
			result[2] = true;
		}
		else if (token == "FL")
		{
			result[3] = true;
		}
	}
	return result;
}

std::string solve_F2L(std::string scramble, std::string rotation, std::string option)
{
	std::string input = "scramble: " + scramble + "\nrotation: " + rotation + "\nslot option: " + option + "\n";
	std::vector<bool> option_list = F2L_option_array(option);
	int count = 0;
	std::vector<int> slot;
	int sol_num = 10;
	int max_length = 15;
	if (option_list[0])
	{
		count += 1;
		slot.push_back(0);
	}
	if (option_list[1])
	{
		count += 1;
		slot.push_back(1);
	}
	if (option_list[2])
	{
		count += 1;
		slot.push_back(2);
	}
	if (option_list[3])
	{
		count += 1;
		slot.push_back(3);
	}

	if (count == 0)
	{
		cross_search search;
		return input + search.start_search(scramble, rotation, sol_num, max_length, move_names);
	}
	else if (count == 1)
	{
		xcross_search search;
		return input + search.start_search(scramble, rotation, slot[0], sol_num, max_length, move_names);
	}
	else if (count == 2)
	{
		xxcross_search search;
		return input + search.start_search(scramble, rotation, slot[0], slot[1], sol_num, max_length, move_names);
	}
	else if (count == 3)
	{
		xxxcross_search search;
		return input + search.start_search(scramble, rotation, slot[0], slot[1], slot[2], sol_num, max_length, move_names);
	}
}

EMSCRIPTEN_BINDINGS(my_module)
{
	emscripten::function("solve", &solve_F2L);
}
