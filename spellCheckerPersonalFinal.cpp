#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <cctype>
#include <climits>
#include <algorithm>

//Function to read words from a dictionary into a set and a list//
void loadDictionary(
    const std::string &filename,
    std::unordered_set<std::string> &dictSet,
    std::vector<std::string> &dictList
) {
    std::ifstream file(filename);
    std::string word;

    while (file >> word) {

        dictSet.insert(word);
        dictList.push_back(word);

         }

    file.close();
}
//Function to change the cases of each letter to lower case for easier reading of the function//
std::string tolowerCase(std::string &word) 
{
    std::string result = word;
    for (char &c : result ) 
    {

    c = tolower(c);
 
    }
 return result;
}

//Edit Distance Function
int editDistance(const std::string &a, const std::string &b) {
      int n = a.length();
      int m = b.length();

std::vector<std::vector<int>> dp(n+1, std::vector<int>(m+1));

for(int i = 0; i <= n; i++)
       dp[i][0] = i;     //Inserting all numbers

for(int j = 0; j <= m; j++)
       dp[0][j] = j;    //Deleting all numbers

for (int i = 1; i <= n; i++)
{
  for (int j = 1; j <= m; j++)
  {
   if(a[i-1] == b[j - 1])
   {
    dp[i][j] = dp[i-1][j-1];
   }
   else
   {
    dp[i][j] = 1 + std::min({
        dp[i-1][j],    //delete
        dp[i][j-1],     //insert
        dp[i-1][j-1]   //replace
       });
     }
   }
}

return dp[n][m];
}

//Best Suggestions Function 
std::unordered_set<std::string> dictionary;
std::vector<std::string> candidates;

std::vector<std::string>bestSuggestions(const std::string &word, const std::vector<std::string>&dictList)
{
  
  std::vector<std::pair<int, std::string>> matches;

  for(const std::string &candidate : dictList)
  {
    if (candidate[0] != word[0])
      continue;
    if (std::abs((int)candidate.length() -(int)word.length()) > 2)
      continue;
      int distance = editDistance(word, candidate);

      if (distance < 3)
      {
         matches.push_back({distance, candidate});
      }
  }

//sort according to distance then order of alphabet
std::sort(matches.begin(), matches.end(), [](
          const std::pair<int,std::string>& a,
          const std::pair<int,std::string>& b)
          {
            if(a.first != b.first)
            return a.first < b.first;
            return a.second < b.second;
          });

    std::vector<std::string> top3;

    for(int i =0; i < matches.size() && i < 3; i++)
    {
       top3.push_back(matches[i].second);
    }

return top3;
  }
   
int main() 
{
   std::unordered_set<std::string> dictionarySet;
   std::vector<std::string> dictionaryList;

    loadDictionary("dictionary.txt", dictionarySet, dictionaryList);

   std::ifstream inputFile("input.txt");
   std::ofstream outputFile("output.txt");

      if(!inputFile.is_open())
      {
          std::cout << "error in opening input file!" << std::endl;
          return 1;
      }
      if(!outputFile.is_open())
      {
          std::cout << "error in opening output file" << std::endl;
          return 1;
      }

    std::string word;
    std::vector<std::string> correctedWords;

  while(inputFile >> word)
{
    std::string loweredWord = tolowerCase(word);

    if(dictionarySet.find(loweredWord) != dictionarySet.end())
    {
      outputFile << "The word [" << word << "] is correct" << std::endl;
      correctedWords.push_back(word);
    }

  else
  {
    std::vector<std::string>suggestions = bestSuggestions(loweredWord, dictionaryList);
  if (!suggestions.empty())
    {
    outputFile << "[" << word << "s]is incorrect! Did you mean: ";
    for(const std::string& s : suggestions)
    {
    outputFile << s << " ";
    }
    outputFile << "\n\n";

    correctedWords.push_back(suggestions[0]);
  
    }
  else
     {
      outputFile << word << " is incorrect! No suggestions found\n";
      correctedWords.push_back(word);
     }
  }
}
outputFile << std::endl;
outputFile << "suggested corrected sentence: ";
for(const std::string &w : correctedWords)
{
    outputFile << w << " ";
}

outputFile << std::endl;
inputFile.close();
outputFile.close();

std::cout << "SPELL CHECKING COMPLETE.RESULTS IN output.txt" << std::endl;
return 0;

}