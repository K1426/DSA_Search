//auto
#include <string>
#include <vector>


//26 alphabets, 10 numbers, 1 for dash or space
const int charsize = 37;


//the "hashing" function
int getindex(char c)
{
    if (c == '-' || c == ' ') return charsize - 1;
    if (c > 47 && c < 58) return c - 22;
    return c - 'a';
}


//node of a trie
struct TrieNode
{
    TrieNode* children[charsize];
    int endofWord;
    bool isLastNode;
    TrieNode()
    {
        endofWord = 0;
        isLastNode = true;
        for(int i = 0; i < charsize; i++) children[i] = nullptr;
    }
};


//the structure of trie
class Trie
{
    private:
        TrieNode* root;
        int size = 0;
    public:
        //constructor
        Trie()
        {
            root = new TrieNode();
        }

        //insert a word
        void insert(std::string word, int wordID)
        {
            int index = -1;
            TrieNode* node = root;
            for (char& c : word)
            {
                index = getindex(c);
                if (node->children[index] == nullptr)
                {
                    node->children[index] = new TrieNode();
                    node->isLastNode = false;
                }
                node = node->children[index];
            }
            node->endofWord = wordID;
            size++;
        }

        //get the size of this
        int getSize() {return size;}

        //search for a word
        int search(std::string word)
        {
            int index = -1;
            TrieNode* node = root;
            for (char& c : word)
            {
                index = getindex(c);
                if (node->children[index] == nullptr) return 0;
                node = node->children[index];
            }
            return node->endofWord;
        }

        //check if a word is in
        bool startsWith(std::string prefix)
        {
            int index = -1;
            TrieNode* node = root;
            for (char& c : prefix)
            {
                index = getindex(c);
                if (node->children[index] == nullptr) return false;
                node = node->children[index];
            }
            return true;
        }

        //printing for autocompletion
        void print(TrieNode* node, std::string prefix, std::vector<std::string>& res)
        {
            if (res.size() > 5) return;
            if (node->endofWord > 0) res.push_back(prefix);
            for (int i = 0; i < charsize; i++)
                if (node->children[i] != nullptr)
                    print(node->children[i], prefix + char('a' + i), res);
        }

        //autocompletion
        std::vector<std::string> printAutoSuggestions(std::string query)
        {
            int index = -1;
            std::vector<std::string> res;
            TrieNode* node = root;
            for (char& c : query)
            {
                index = getindex(c);
                //if the word is not in, return empty
                if (node->children[index] == nullptr) return {};

                //if word is in so far, move to the next
                node = node->children[index];
            }
            if (node->isLastNode)
            {
                //if no children
                return {query};
            }

            //move on
            print(node, query, res);
            return res;
        }
};
