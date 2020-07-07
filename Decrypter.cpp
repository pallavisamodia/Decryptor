#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include"MyHash.h"
using namespace std;

class DecrypterImpl
{
public:
	bool load(string filename);
	vector<string> crack(const string& ciphertext);
private:
	WordList w;
	Translator trans;
	string chosenOne(vector<string> jar);

	void trial(string ciphertext, string chosen, vector<string> candi, vector<string>& output);
	bool arr[1000];
	void initalCheck() {
		for (int i = 0; i < 1000; i++)
			arr[i] = false;
	}
	vector<string> crack(const string& ciphertext, vector<string>& output);
	bool fullytranslated(string translated);
	bool makesSense(string translated);
/*	struct Node {
		string value;
		Node* parent;
		Node** child;
		Node* nex;
		Node(string trans, Node* p) { child = nullptr; value = trans; parent = p; nex = nullptr; }
	};*/


	//Tokenizer t(",;:.!()[]{}-\"#$%^&\""); //-----ASK WHY CANT I DO THIS
	
};

bool DecrypterImpl::load(string filename)
{
	if (w.loadWordList(filename) == true)
		return true;

	return false;
}
vector<string> DecrypterImpl::crack(const string& ciphertext, vector<string>& output)
{
	if (fullytranslated(ciphertext) && makesSense(ciphertext))
	{
		trans.popMapping();
		output.push_back(ciphertext);
		return output;

	}

	string current; string current_trans; vector<string> candidates;
	Tokenizer t(" 0123456789,.;:!()[]{}-\"#$%^&");
	vector<string> words = t.tokenize(ciphertext);
	current = chosenOne(words);
	current_trans = trans.getTranslation(current);
	candidates = w.findCandidates(current, current_trans);
	for (int i = 0; i < candidates.size(); i++)
	{
		if (trans.pushMapping(current, candidates[i]) == true)
		{
			output = crack(trans.getTranslation(ciphertext),output);
		}

	}
	return output;
}
vector<string> DecrypterImpl::crack(const string& ciphertext)
{
	Tokenizer t(" 0123456789,.;:!()[]{}-\"#$%^&");
	vector<string> words = t.tokenize(ciphertext);
	
	vector<string> result;
	if (words.empty() == true)
		return result;
	initalCheck(); //useful for selecting words to work with
	string current = chosenOne(words);
	string current_trans = trans.getTranslation(current);
	vector<string> candidates = w.findCandidates(current, current_trans);
	for (int i = 0; i < candidates.size(); i++)
	{
		if (trans.pushMapping(current, candidates[i]) == true)
		{
			result= crack(trans.getTranslation(ciphertext),result);
		}

	}
	return result;


}




/*
vector<string> DecrypterImpl::crack(const string& ciphertext)
{
	Tokenizer t(" 0123456789,.;:!()[]{}-\"#$%^&");
	vector<string> words = t.tokenize(ciphertext);
	vector<string> result;
	//getting the first most untranslated word in the string & its most updated translation
	if (words.empty() == true)
		return result;
	string current; string current_trans; vector<string> candidates;
	for (int i = 0; i < words.size(); i++)
	{
		current = chosenOne(words);
		current_trans = trans.getTranslation(current);
		candidates = w.findCandidates(current, current_trans);
		tree.associate(current, candidates);
		words.pop_back();
	}

	
	//vector<string>::iterator it;
	Node* prev;
	//creating first level of tree
	Node* root = new Node(ciphertext, nullptr);
	while (words.empty() == false)
	{
		Node* level1= new Node(current, root);
		root->child = level1;
		prev = level1;
		//creating second level
		for(int i=0;i<candidates.size();i++)
		{
			Node* level2= new Node(candidates.front(), level1);
			level1->child[i] = level2;
		}
		words.pop_back();
		if (words.empty() == true)
			break;
		
		current = chosenOne(words);

	}



	while (words.empty() == false)
	{
		trial(ciphertext,current, candidates, result);
		
		
		current = chosenOne(words);
		current_trans = trans.getTranslation(current);
		candidates = w.findCandidates(current, current_trans);
	}

	//calling our recursive function


	return result;  // This compiles, but may not be correct
}*/

//helper functions
bool DecrypterImpl::makesSense(string translated)
{
	Tokenizer t(" 0123456789,.;:!()[]{}-\"#$%^&");
	vector<string> trans = t.tokenize(translated);
	for (int i = 0; i < trans.size(); i++)
		if (!w.contains(trans[i]))
			return false;

	return true;
}

bool DecrypterImpl::fullytranslated(string translated)
{
	Tokenizer t(" 0123456789,.;:!()[]{}-\"#$%^&");
	vector<string> trans=t.tokenize(translated);
	for (int i = 0; i < trans.size(); i++)
	{
		string curr = trans[i];
		for (int j = 0; j < curr.length(); j++)
			if (curr[i] == '?')
				return false;
	}
	return true;
}
void DecrypterImpl::trial(string ciphertext,string chosen, vector<string> candi, vector<string>& output)
{
	if (candi.empty() == true)  //base case
	{
		return;
	}

	if (trans.pushMapping(chosen, candi.front()) == true)
	{
		output.push_back(trans.getTranslation(ciphertext));
		cerr << "latest translation is " << output.back() << endl;
	}

	candi.pop_back();

}


string DecrypterImpl::chosenOne(vector<string> jar)
{
	
	string smallest;
	int smallestC = 0;
	int pos = 0;
	for (int i = 0; i < jar.size(); i++)
	{
		int count = 0;
		string curr = jar[i];
		string curTrans = trans.getTranslation(curr);
		for (int j = 0; j < curTrans.length(); j++)
			if (curTrans[j] == '?')
				count++;
		if (count > smallestC && (arr[i]==false))
		{
			smallestC = count;
			smallest = curr;
			pos = i;
		
		}
	}
	arr[pos] = true;
	cerr << "the chosen word is " << smallest << endl;
	return smallest;
}




//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter()
{
	m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
	delete m_impl;
}

bool Decrypter::load(string filename)
{
	return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
	return m_impl->crack(ciphertext);
}
