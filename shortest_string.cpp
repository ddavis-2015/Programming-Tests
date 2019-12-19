//
// Copyright (c) 2019 David Davis.  All rights reserved.
//
// Compiled on macOS with:
//
//	clang++ -g -O2 -std=c++14 -o shortest_string shortest_string.cpp
//

static const char copyright[] __attribute((used)) =
	"Copyright (c) 2019 David Davis.  All rights reserved";


#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <tuple>


class MinStringUsingAlphabet
{
    // create a count of each character in the alphabet
    using CountDict = std::map<char, int>;
    const CountDict makeCountDict();
    // create a vector of indexes for each character in string that is also in alphabet
    using IndexDict = std::map<char, std::vector<int>>;
    const IndexDict makeIndexDict();
    using IndexTuple = std::tuple<bool, int, int>;
    const IndexTuple getSubstringIndexes(const int begin, const int end);

	const std::string _input;
	const std::string _alphabet;
	const CountDict _countDict;
	const IndexDict _indexDict;

public:
    // given input and alphabet, return smallest substring containing all characters
	// in alphabet (including copies in alphabet)
    const std::string getMinSubstr();

	MinStringUsingAlphabet(const std::string& input, const std::string& alphabet);
	~MinStringUsingAlphabet();
};


int main()
{
    while (true)
    {
        std::string input_string, alphabet;

        std::cout << "string: ";
        std::getline(std::cin, input_string);
        if (input_string.empty())
            break;
        std::cout << "alphabet: ";
        std::getline(std::cin, alphabet);
        if (alphabet.empty())
            break;

        MinStringUsingAlphabet o(input_string, alphabet);;

        auto result = o.getMinSubstr();
        std::cout << "result: " << result << std::endl;
    }

    return 0;
}

MinStringUsingAlphabet::MinStringUsingAlphabet(
	const std::string& input,
	const std::string& alphabet) :
		_input(input), _alphabet(alphabet),
		_countDict(makeCountDict()),
		_indexDict(makeIndexDict())
{
}

MinStringUsingAlphabet::~MinStringUsingAlphabet()
{
}

const std::string
MinStringUsingAlphabet::getMinSubstr()
{
    std::vector<std::tuple<int, int>> indexes;
    int begin = 0;
    int end = _input.length() - 1;
    int endIndex = end;
    bool hasAll;

    std::tie(hasAll, begin, end) = getSubstringIndexes(begin, end);
    if (!hasAll)
        return std::string();
    else
        indexes.push_back(std::make_tuple(begin, end));

    while (true)
    {
        std::tie(hasAll, begin, end) =
			getSubstringIndexes(begin + 1, endIndex);
        if (!hasAll)
            break;
        else
            indexes.push_back(std::make_tuple(begin, end));
    }

    auto const shortest = std::min_element(indexes.begin(), indexes.end(),
                            [&](auto const& first_tuple, auto const& second_tuple) -> bool
                            {
                                auto first_end = std::get<1>(first_tuple);
                                auto first_begin = std::get<0>(first_tuple);
                                auto second_end = std::get<1>(second_tuple);
                                auto second_begin = std::get<0>(second_tuple);

                                return (first_end - first_begin) < (second_end - second_begin);
                            });
    std::tie(begin, end) = *shortest;

    return _input.substr(begin, (end - begin) + 1);
}

const MinStringUsingAlphabet::CountDict 
MinStringUsingAlphabet::makeCountDict()
{
    CountDict result;

    for (auto c : _alphabet)
    {
        result[c]++;
    }

    return result;
}

const MinStringUsingAlphabet::IndexDict
MinStringUsingAlphabet::makeIndexDict()
{
    IndexDict result;

    for (int i = 0; i < _input.length(); i++)
    {
        // fancy form of: _alphabet.find(_input[i]) != std::string::npos
        if (std::any_of(_alphabet.begin(), _alphabet.end(),
			[&](const char& c)
			{
				return _input[i] == c;
			}))
        {
            result[_input[i]].push_back(i);
        }
    }

    return result;
}

const MinStringUsingAlphabet::IndexTuple
MinStringUsingAlphabet::getSubstringIndexes(const int begin, const int end)
{
    std::vector<int> indexes;
    auto result = std::make_tuple(false, 0, 0);

    for (const auto kv : _countDict)
    {
        const auto k = std::get<0>(kv);
        const auto v = std::get<1>(kv);

        if (_indexDict.count(k) == 0)
            return result;
        auto vec { _indexDict.at(k) };
        auto const it_end = std::remove_if(vec.begin(), vec.end(),
			[&](const int& index)
			{
				return index < begin || index > end;
			});
        if (std::distance(vec.begin(), it_end) < v)
            return result;
        else
            indexes.insert(indexes.end(), vec.begin(), vec.begin() + v);
    }

    std::get<0>(result) = true;
    std::get<1>(result) = *std::min_element(indexes.begin(), indexes.end());
    std::get<2>(result) = *std::max_element(indexes.begin(), indexes.end());

    return result;
}

