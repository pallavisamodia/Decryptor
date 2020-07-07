#include "provided.h"
#include <string>
#include<unordered_map>
#include<iostream>
#include<stack>
#include<set>
using namespace std;

class TranslatorImpl
{
public:
	TranslatorImpl();
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;
private:
	
	
	stack<unordered_map<char,char>> timeline;
	
};
TranslatorImpl::TranslatorImpl()
{
	
	unordered_map<char, char> hm;
	for (int i = 0; i < 26; i++)
	{
		hm[char(97 + i)] = '?';
	}
	timeline.push(hm);

}
bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
	bool conflict = false;
	if (ciphertext.length() != plaintext.length())
		return false;

	for (int i = 0; i < ciphertext.length(); i++)
	{
		if (!(isalpha(ciphertext[i])) || !(isalpha(plaintext[i])))
			return false;
	}
	unordered_map<char, char> hm = timeline.top();
		unordered_map<char, char>::iterator it;
		for (int i = 0; i < ciphertext.length(); i++)
		{
			it = hm.find(tolower(ciphertext[i]));
			if (it != hm.end())
			{
				if (it->second == '?')
					it->second = tolower(plaintext[i]);
				else
					if(it->second != tolower(plaintext[i]))
						conflict = true;
			}
		
		}
		timeline.push(hm);
		if (conflict == true)
		{
			popMapping();
			return false;
		}


	//check for duplicates
	
	set<char>dupe;
	unordered_map<char, char>::iterator dt;
	for (dt = hm.begin(); dt != hm.end(); dt++)
	{
		
		if (dupe.find((dt->second)) == dupe.end())
		{
			if (dt->second != '?')
				dupe.insert((dt->second));
			else
				continue;
		}

		else
		{
			popMapping();
			return false;
		}
	}
	
	return true;
}

bool TranslatorImpl::popMapping()
{
	
	if (timeline.empty() == false)
	{
		if (timeline.size() == 1)
			return false;
		else
			timeline.pop();
	}
	else
		return false;
	
		
	return true;
	
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
	string temp;
	
	unordered_map<char, char> hm = timeline.top();
	for (int i = 0; i < ciphertext[i]; i++)
	{
		if (!isalpha(ciphertext[i]))
			temp+= ciphertext[i];
		else
		{if(isupper(ciphertext[i]))
			temp += toupper((hm.find(tolower(ciphertext[i]))->second));
		else
			temp += tolower((hm.find(tolower(ciphertext[i]))->second));
		}
	}
	cerr << "translation is " << temp << endl;
	return temp;
}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
    m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
    delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
    return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
    return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
    return m_impl->getTranslation(ciphertext);
}
