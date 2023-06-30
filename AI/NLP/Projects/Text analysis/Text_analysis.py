import requests
from bs4 import BeautifulSoup
import pandas as pd
import numpy as np
import glob
from pathlib import Path
from nltk.corpus import stopwords
from nltk.tokenize import sent_tokenize, word_tokenize
import string
import re
import spacy
from textstat.textstat import textstatistics,legacy_round

headers = {
    'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10.12; rv:55.0) Gecko/20100101 Firefox/55.0',
}


stop_words = []
p = Path(r"C:\Users\SOUMYA\@DS&AI\stop")
path_of_the_directory = p
print("Files and directories in a specified path:")
file = Path(path_of_the_directory ).glob('*')
for i in file:
    print(i)
    file = open(i, 'rt')
    text = file.read()
    file.close()
    # split into words by white space
    words = text.split()
    #print(words)
    print(len(words))
    for word in words:
        stop_words.append(word)
#len(stop_words)


df=pd.read_csv("Loughran-McDonald_MasterDictionary_1993-2021.csv",nrows=100000)


positive = []
negative = []
for i in range(0, df.shape[0]):
    if df['Word'][i] not in stop_words:
        if df['Negative'][i] != 0:
            negative.append(df['Word'][i])
        elif df['Positive'][i] != 0:
            positive.append(df['Word'][i])

for i in range(len(positive)):
    positive[i] = positive[i].lower()
for i in range(len(negative)):
    negative[i] = negative[i].lower()

input_data = pd.read_csv('input.csv')

# ---------------------------------------------
total_stopwords = set(stopwords.words('english'))
HTMLTAGS = re.compile('<.*?>')
table = str.maketrans(dict.fromkeys(string.punctuation))
remove_digits = str.maketrans('', '', string.digits)
MULTIPLE_WHITESPACE = re.compile(r"\s+")
# ---------------------------------------------
def preprocessor(review):
    # remove html tags
    review = HTMLTAGS.sub(r'', review)

    # remove puncutuation
    review = review.translate(table)
    
    # remove digits
    review = review.translate(remove_digits)
    
    # lower case all letters
    review = review.lower()
    
    # replace multiple white spaces with single space
    review = MULTIPLE_WHITESPACE.sub(" ", review).strip()
    
    # remove stop words
    review = [word for word in review.split()
              if word not in total_stopwords]
    
    # stemming
    #review = ' '.join([stemmer.stem(word) for word in review])
    review = ' '.join([word for word in review])
    
    return review


def break_sentences(text):
    nlp = spacy.load('en_core_web_sm')
    doc = nlp(text)
    return list(doc.sents)


def word_count(sentences_data):
    words = 0
    for sentence in sentences_data:
        words += len([token for token in sentence])
    return words


def sentence_count(text):
    return len(sentences)


def avg_sentence_length(x, y):
    average_sentence_length = float(x / y)
    return average_sentence_length


def syllables_count(word):
    return textstatistics().syllable_count(word)


def avg_syllables_per_word(text, words):
    syllable = syllables_count(text)
    ASPW = float(syllable) / float(words)
    return legacy_round(ASPW, 1)


def complex_words(text):
     
    nlp = spacy.load('en_core_web_sm')
    doc = nlp(text)
    # Find all words in the text
    words = []
    sentences = break_sentences(text)
    for sentence in sentences:
        words += [str(token) for token in sentence]
 
    # difficult words are those with syllables >= 2
    # easy_word_set is provide by Textstat as
    # a list of common words
    com_words_set = set()
     
    for word in words:
        syllable_count = syllables_count(word)
        if word not in nlp.Defaults.stop_words and syllable_count > 2:
            com_words_set.add(word)
 
    return len(com_words_set)


def avg_word_len(txt):
    words = txt.split()
    avg = sum(len(word) for word in words) / len(words)
    return avg


def pronoun_count(txt):
    pronounRegex = re.compile(r'(I|we|my|ours|(?-i:us))',re.I)
    pronouns = pronounRegex.findall(txt)
    return len(pronouns)


#f = open('URL_ID.txt', 'w')
output = []
for i in range(0, len(input_data)):
    out = []
    print(i)
    static_url = input_data['URL'][i]
    out.append(input_data['URL_ID'][i])
    out.append(input_data['URL'][i])
    #print(static_url)
    response = requests.get(static_url,  headers=headers)
    print(response)
    soup = BeautifulSoup(response.text, "html.parser")
    soup.title
    for title in soup.find_all('title'):
        print(title.get_text())  
    t = []
    for txt in soup.find_all('p'):
        #print(txt.get_text())
        t.append(txt.get_text())
    raw_txt = ' '.join([a for a in t])
    sentences = break_sentences(raw_txt)
    preprocesed_sent = []
    for sentence in sentences:
        preprocesed_sent.append(preprocessor(sentence.text))
    #print(preprocesed_sent)
    prep = ' '.join([a for a in preprocesed_sent])
    p = 0
    n = 0
    l = []
    for i in range(0, len(preprocesed_sent)):
        l.append(word_count(preprocesed_sent[i]))
        for j in preprocesed_sent[i].split():
            if j in positive:
                p += 1
            elif j in negative:
                n += 1
    tot_processed = sum(a for a in l)
    polarity = (p - n)/((p+n) + 0.000001)
    sub = (p + n)/((tot_processed) + 0.000001)
    complex_count = complex_words(prep)
    avg_sent = avg_sentence_length(tot_processed,sentence_count(preprocesed_sent))
    per_complex = complex_count/tot_processed
    Fog_Index = 0.4 * (avg_sent + per_complex)
    avg_word = tot_processed/sentence_count(preprocesed_sent)
    avg_syll = avg_syllables_per_word(prep, tot_processed)
    pronouns_count = pronoun_count(prep)
    
    out.append(p)
    out.append(n)
    out.append(polarity)
    out.append(sub)
    out.append(avg_sent)
    out.append(per_complex)
    out.append(Fog_Index)
    out.append(avg_word)
    out.append(complex_count)
    out.append(tot_processed)
    out.append(avg_syll)
    out.append(pronouns_count)
    out.append(avg_word_len(prep))
    print(out)
    output.append(out)



output_data = pd.DataFrame(output, columns=['URL_ID','URL','POSITIVE SCORE','NEGATIVE SCORE',
                                            'POLARITY SCORE','SUBJECTIVITY SCORE','AVG SENTENCE LENGTH',
                                            'PERCENTAGE OF COMPLEX WORDS','FOG INDEX','AVG NUMBER OF WORDS PER SENTENCE',
                                            'COMPLEX WORD COUNT','WORD COUNT','SYLLABLE PER WORD','PERSONAL PRONOUNS','AVG WORD LENGTH'])



output_data


output_data.to_csv('output_data.csv')
        


