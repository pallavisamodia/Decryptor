#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include<algorithm>
#include<unordered_map>
#include "MyHash.h" //---see again later
using namespace std;
//template<typename KeyType,typename ValueType>
class WordListImpl
{
public:
	WordListImpl()
	{
		m_words.clear();

	}
	bool loadWordList(string filename);
	bool contains(string word) const;
	vector<string> findCandidates(string cipherWord, string currTranslation) const;
private:
	vector<string>m_words;
	string pattern(string ciphertext) const;
	//void allocation();
	MyHash<string, vector<string>> dict; //am i ok?

};


bool WordListImpl::loadWordList(string filename)
{

	ifstream infile(filename);    // infile is a name of our choosing
	if (!infile)		        // To check if opening of the file failed
	{
		cerr << "Error: Cannot open data.txt!" << endl;
		return false;
	}
	while (infile)
	{
		//-------------------TAKINF TIME HERE!!!!1
		std::string s;
		getline(infile, s);
		for (int i = 0; i < s.length(); i++)
			s[i] = tolower(s[i]);
		if (find(m_words.begin(), m_words.end(), s) == m_words.end())
		{
			m_words.push_back(s);
			cerr << s << endl;
			string pat = pattern(s);
		
			if (dict.find(pat) != nullptr)
			{
				cerr << "similar pattern found" << endl;
				vector<string> temp = *(dict.find(pattern(s)));
				temp.push_back(s);
				dict.associate(pat, temp);
			}
			else
			{
				cerr << "similar pattern not found" << endl;
				vector<string> temp;
				temp.push_back(s);
				dict.associate(pat, temp);
			}
		}

	}
	cerr << "word list loaded";
	//allocation(); //once our file is loaded, we can so separation
	return true;
}

bool WordListImpl::contains(string word) const
{//-----------------SEE THE LOWERCASE AND STUFF----------------------
	for (int i = 0; i < word.length(); i++)
		word[i] = tolower(word[i]);

	if (std::find(m_words.begin(), m_words.end(), word) != m_words.end())
		return true;
	return false;
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
	bool track = true;

	//iterating through our the vector with same pattern

	vector<string> it;
	vector<string> temp; //for returning our new list of probables

	if (cipherWord.length() != currTranslation.length())
		return it; //returning empty string if the lengths do not match

	string pat = pattern(cipherWord);
	
	if ((dict.find(pat)) != nullptr)
	{
		
		cerr << "pattern found" << endl;
		it = *(dict.find(pat));
		for (int i = 0; i < it.size(); i++)
		{
			track = true;
			string curWord = it[i];
			cerr << curWord << endl;
			for (int j = 0; j < currTranslation.length(); j++)
			{
				if (currTranslation[j] == '\'' && curWord[j] != '\'')
				{
					cerr << "apostrophe alpha case" << endl;
					track = false; break;
				}
				else if (currTranslation[j] == '?' && (!isalpha(curWord[j])))
				{
					cerr << "question mark case" << endl;
					track = false; break;
				}
				else if (isalpha(currTranslation[j]))
				{
					cerr << "entered alpha case" << endl;
					if (currTranslation[j] != curWord[j])
					{
						track = false;
						cerr << "false time" << endl; break;
					}
				}
				else
					continue;
			

			}

			if (track == true && curWord.length() > 0)
			{
				cerr << "your pattern is " << pat << endl;
				temp.push_back(curWord); cerr << "truth time" << endl;
			}
		}
	}
	else
	{
		return it; //returning an empty string
	}
	if (track== false)
	{
		vector<string>emp; return emp; //returning an empty string
	}
	else
		return temp;

}

//Helper function
string WordListImpl::pattern(string ciphertext) const
{
	int j = 0;
	std::string pat;

	MyHash<char, char> h(0.5);
	for (int i = 0; i < ciphertext.length(); i++)
	{

		if (h.find(tolower(ciphertext[i])) != nullptr)
		{
			pat += *(h.find(tolower(ciphertext[i])));
		}
		else
		{
			h.associate(tolower(ciphertext[i]), char(65 + j));
			j++;
			pat += *(h.find(tolower(ciphertext[i])));
		}
	}
	h.reset();
	
	return pat;

}
//Helper function
/*
void WordListImpl::allocation()
{


	for (int i = 0; i < m_words.size(); i++)
	{//if a similar pattern is in our file
		string pat = pattern(m_words[i]);
		if (dict.find(pat) != nullptr)
		{
			vector<string> temp = *(dict.find(pattern(m_words[i])));
			temp.push_back(m_words[i]);
			dict.associate(pat, temp);
		}
		else
		{
			vector<string> temp;
			temp.push_back(m_words[i]);
			dict.associate(pat, temp);

		}
	}


}*/
//WordList::~WordList() {} //=-----------------------ADD IT IF YOU NEED TO

//***** hash functions for string, int, and char *****

unsigned int hash(const std::string& s)
{
	return std::hash<std::string>()(s);
}

unsigned int hash(const int& i)
{
	return std::hash<int>()(i);
}

unsigned int hash(const char& c)
{
	return std::hash<char>()(c);
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
	m_impl = new WordListImpl;
}

WordList::~WordList()
{
	delete m_impl;
}

bool WordList::loadWordList(string filename)
{
	return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
	return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
	return m_impl->findCandidates(cipherWord, currTranslation);
}
