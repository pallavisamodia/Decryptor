#include "provided.h"
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class TokenizerImpl
{
public:
	TokenizerImpl(string separators);
	vector<string> tokenize(const std::string& s) const;
private:


	vector<char> m_sep;
};

TokenizerImpl::TokenizerImpl(string separators)
{
	if (separators.length() > 0)
		for (int i = 0; i < separators.length(); i++)
			m_sep.push_back(separators[i]);
}

vector<string> TokenizerImpl::tokenize(const std::string& s) const
{
	std::vector<string> m_words;

	string temp;
	if (m_sep.empty() == false)
		for (int i = 0; i < s.length(); i++)
		{

			{
				if (std::find(m_sep.begin(), m_sep.end(), s[i]) != m_sep.end())

				{
					if (temp != "")
					{
						m_words.push_back(temp);
						temp = "";
					}


				}
				else
					temp += s[i];
			}

		}

	//If the last character in the string was an alphabet
	if (temp != "") { m_words.push_back(temp); }
	return m_words;
}

//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
	m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
	delete m_impl;
}

vector<string> Tokenizer::tokenize(const std::string& s) const
{
	return m_impl->tokenize(s);
}
