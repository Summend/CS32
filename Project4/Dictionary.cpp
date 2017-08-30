#include "Dictionary.h"
#include <string>
#include <list>
#include <cctype>
#include <algorithm>  // for swap

using namespace std;

void removeNonLetters(string& s);

const int NUM_BUCKET = 50000;

class DictionaryImpl
{
public:
    DictionaryImpl(){}
    ~DictionaryImpl() {}
    void insert(string word);
    void lookup(string letters, void callback(string));
private:
    list<string> m_buckets[NUM_BUCKET];
    hash<string> hash_fn;
    int hashFunc(string word, unsigned long& num) const
    {
        sort(word.begin(),word.end());
        num = hash_fn(word);
        return (num % NUM_BUCKET);
    }
    int hashFunc(string word) const
    {
        sort(word.begin(),word.end());
        return (hash_fn(word) % NUM_BUCKET);
    }
};

void DictionaryImpl::insert(string word)
{
    removeNonLetters(word);
    if (!word.empty())
        (m_buckets[hashFunc(word)]).push_back(word);
}

void DictionaryImpl::lookup(string letters, void callback(string))
{
    if (callback == nullptr)
        return;
    removeNonLetters(letters);
    if(letters.empty())
        return;
    unsigned long trueHashNum = 0;
    int bucket = hashFunc(letters,trueHashNum);
    if (m_buckets[bucket].empty())
        return;
    for(list<string>::iterator i = m_buckets[bucket].begin(); i != m_buckets[bucket].end(); i++)
    {
        unsigned long trueHashNumS = 0;
        hashFunc((*i),trueHashNumS);
        if (trueHashNum == trueHashNumS)
            callback(*i);
    }
}

void removeNonLetters(string& s)
{
    string::iterator to = s.begin();
    for (string::const_iterator from = s.begin(); from != s.end(); from++)
    {
        if (isalpha(*from))
        {
            *to = tolower(*from);
            to++;
        }
    }
    s.erase(to, s.end());  // chop off everything from "to" to end.
}

Dictionary::Dictionary()
{
    m_impl = new DictionaryImpl;
}

Dictionary::~Dictionary()
{
    delete m_impl;
}

void Dictionary::insert(string word)
{
    m_impl->insert(word);
}

void Dictionary::lookup(string letters, void callback(string)) const
{
    m_impl->lookup(letters, callback);
}
