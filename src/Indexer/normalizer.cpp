#include "Indexer/normalizer.h"
#include <cctype>

Normalizer::Normalizer()
{
    load_stopwords();
}


bool Normalizer::has_alpha(const std::string& word)
{
    std::string token = word;
    for(auto &i:token)
    {
        if(std::isalpha(i))
        {
            return true;
        }
    }
    return false;
}
std::string Normalizer::normalize(const std::string& word)
{   
    std::string token = to_lowercase(word);
    token = trim_punctuation(token);
    if(token.empty())
    {
        return "";
    }
    if(token.length() < 2)
    {
        return "";
    }
    if(!has_alpha(token))
    {
        return "";
    }
    if(is_stopWord(token))
    {
        return "";
    }
    return token;
}

std::string Normalizer::to_lowercase(const std::string& word)
{
    std::string result = word;
    for(char &i:result)
    {
        i = std::tolower(i);
    }
    return result;
}

std::string Normalizer::trim_punctuation(const std::string& word)
{
    int i = 0;
    while(i < word.size() && std::ispunct(word[i]))
    {
        i++;
    }
    int j = word.size();
    while(j > i && std::ispunct(word[j-1]))
    {
        j--;
    }
    return word.substr(i,j-i);
}

bool Normalizer::is_stopWord(const std::string& word)
{
    return stopwords.contains(word);
}
void Normalizer::load_stopwords()
{
    // Articles
    stopwords.insert("a", true);
    stopwords.insert("an", true);
    stopwords.insert("the", true);

    // Conjunctions
    stopwords.insert("and", true);
    stopwords.insert("or", true);
    stopwords.insert("but", true);
    stopwords.insert("nor", true);
    stopwords.insert("so", true);
    stopwords.insert("yet", true);

    // Prepositions
    stopwords.insert("about", true);
    stopwords.insert("above", true);
    stopwords.insert("across", true);
    stopwords.insert("after", true);
    stopwords.insert("against", true);
    stopwords.insert("along", true);
    stopwords.insert("among", true);
    stopwords.insert("around", true);
    stopwords.insert("at", true);
    stopwords.insert("before", true);
    stopwords.insert("behind", true);
    stopwords.insert("below", true);
    stopwords.insert("beneath", true);
    stopwords.insert("beside", true);
    stopwords.insert("between", true);
    stopwords.insert("by", true);
    stopwords.insert("down", true);
    stopwords.insert("during", true);
    stopwords.insert("for", true);
    stopwords.insert("from", true);
    stopwords.insert("in", true);
    stopwords.insert("inside", true);
    stopwords.insert("into", true);
    stopwords.insert("near", true);
    stopwords.insert("of", true);
    stopwords.insert("off", true);
    stopwords.insert("on", true);
    stopwords.insert("onto", true);
    stopwords.insert("out", true);
    stopwords.insert("outside", true);
    stopwords.insert("over", true);
    stopwords.insert("through", true);
    stopwords.insert("to", true);
    stopwords.insert("toward", true);
    stopwords.insert("under", true);
    stopwords.insert("until", true);
    stopwords.insert("up", true);
    stopwords.insert("upon", true);
    stopwords.insert("with", true);
    stopwords.insert("within", true);
    stopwords.insert("without", true);

    // Pronouns
    stopwords.insert("i", true);
    stopwords.insert("me", true);
    stopwords.insert("my", true);
    stopwords.insert("mine", true);
    stopwords.insert("we", true);
    stopwords.insert("our", true);
    stopwords.insert("ours", true);
    stopwords.insert("you", true);
    stopwords.insert("your", true);
    stopwords.insert("yours", true);
    stopwords.insert("he", true);
    stopwords.insert("him", true);
    stopwords.insert("his", true);
    stopwords.insert("she", true);
    stopwords.insert("her", true);
    stopwords.insert("hers", true);
    stopwords.insert("it", true);
    stopwords.insert("its", true);
    stopwords.insert("they", true);
    stopwords.insert("them", true);
    stopwords.insert("their", true);
    stopwords.insert("theirs", true);

    // Auxiliary verbs
    stopwords.insert("am", true);
    stopwords.insert("is", true);
    stopwords.insert("are", true);
    stopwords.insert("was", true);
    stopwords.insert("were", true);
    stopwords.insert("be", true);
    stopwords.insert("been", true);
    stopwords.insert("being", true);
    stopwords.insert("do", true);
    stopwords.insert("does", true);
    stopwords.insert("did", true);
    stopwords.insert("have", true);
    stopwords.insert("has", true);
    stopwords.insert("had", true);
    stopwords.insert("can", true);
    stopwords.insert("could", true);
    stopwords.insert("shall", true);
    stopwords.insert("should", true);
    stopwords.insert("will", true);
    stopwords.insert("would", true);
    stopwords.insert("may", true);
    stopwords.insert("might", true);
    stopwords.insert("must", true);

    // Common adverbs
    stopwords.insert("again", true);
    stopwords.insert("also", true);
    stopwords.insert("always", true);
    stopwords.insert("almost", true);
    stopwords.insert("already", true);
    stopwords.insert("ever", true);
    stopwords.insert("never", true);
    stopwords.insert("often", true);
    stopwords.insert("only", true);
    stopwords.insert("quite", true);
    stopwords.insert("rather", true);
    stopwords.insert("really", true);
    stopwords.insert("still", true);
    stopwords.insert("then", true);
    stopwords.insert("there", true);
    stopwords.insert("therefore", true);
    stopwords.insert("thus", true);
    stopwords.insert("very", true);

    // Demonstratives & Determiners
    stopwords.insert("all", true);
    stopwords.insert("another", true);
    stopwords.insert("any", true);
    stopwords.insert("both", true);
    stopwords.insert("each", true);
    stopwords.insert("either", true);
    stopwords.insert("every", true);
    stopwords.insert("few", true);
    stopwords.insert("many", true);
    stopwords.insert("more", true);
    stopwords.insert("most", true);
    stopwords.insert("much", true);
    stopwords.insert("neither", true);
    stopwords.insert("no", true);
    stopwords.insert("none", true);
    stopwords.insert("some", true);
    stopwords.insert("such", true);
    stopwords.insert("this", true);
    stopwords.insert("that", true);
    stopwords.insert("these", true);
    stopwords.insert("those", true);

    // Question words
    stopwords.insert("what", true);
    stopwords.insert("when", true);
    stopwords.insert("where", true);
    stopwords.insert("which", true);
    stopwords.insert("who", true);
    stopwords.insert("whom", true);
    stopwords.insert("whose", true);
    stopwords.insert("why", true);
    stopwords.insert("how", true);

    // Miscellaneous
    stopwords.insert("if", true);
    stopwords.insert("than", true);
    stopwords.insert("because", true);
    stopwords.insert("while", true);
    stopwords.insert("whether", true);
    stopwords.insert("here", true);
    stopwords.insert("there", true);
}