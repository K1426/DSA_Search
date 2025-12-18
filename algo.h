//sort_algo
#include "Lexicon_Forward.h"
#include "inverted_index.h"

//ranking criteria
float getrank(std::pair<int, float>& a) {return a.second;}

//sort according to criteria
void quickSort(std::vector<std::pair<int, float>>& a, int start, int end, float (*f)(std::pair<int, float>&))
{
    //ensure the numbers to be sorted are more than one
    if (start >= end) return;

    //select pivot element (here it is center element)
    float pivot = f(a[(start + end) / 2]);
    int i = start - 1, j = end + 1;

    while (true)
    {
        //decrease i until larger or equal element is found
        do i++; while (f(a[i]) < pivot);
        //decrease j until smaller or equal element is found
        do j--; while (f(a[j]) > pivot);
        //if all elements have been traversed break
        if (i >= j) break;
        //swap the elements that were on wrong side
        a[i].swap(a[j]);
    }

    //do the same with array on left of pivot
    quickSort(a, start, j, f);
    //and then with array on right of pivot
    quickSort(a, j + 1, end, f);
}

//rank each word's list
void rank(std::vector<std::pair<int, float>>& ranks, int n)
{
    std::unordered_map<int, std::pair<int, float>> temp;

    //make a vector for each word
    std::vector<std::pair<int, float>>* res = new std::vector<std::pair<int, float>>[n];

    //count the occurrences of the documents
    //ie how many required words in each document
    for (auto& [docID, rank] : ranks)
    {
        temp[docID].first++;
        temp[docID].second+=rank;
    }
    ranks.clear();

    //add to respective list
    for (auto& [docID, rank_crit] : temp)
    {
        res[n - rank_crit.first].push_back({docID, rank_crit.second});
    }

    //add to result vector
    for (int i = 0; i < n; i++)
    {
        quickSort(res[i], 0, res[i].size() - 1, getrank);
        for (auto& e : res[i]) ranks.push_back(e);
    }

    delete[] res;
    temp.clear();
}

//get query results
std::vector<std::pair<int, float>> multi_search(std::string& query)
{
    //n is the number of words in the query
    int wordID = 0, n = 0;
    std::string word = "";
    std::vector<std::pair<int, float>> ranks{}, temp{};
    std::stringstream ss(query);

    //for every word
    while (ss >> word)
    {
        //get the documents it is associated with
        clean_token(word);
        if (word == "") continue;
        wordID = lexicon.search(word);
        if (wordID == 0) continue;
        temp = barrel_inverted_index->get_word(wordID)->ranks;
        for (auto& i : temp) ranks.push_back(i);
        temp.clear();
        n++;
    }

    //if only one word, then simple ranking
    if (n == 1)
    {
        quickSort(ranks, 0, ranks.size() - 1, getrank);
        return ranks;
    }

    //otherwise more elaborate ranking
    rank(ranks, n);
    return ranks;
}

