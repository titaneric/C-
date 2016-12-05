#Build the reserved word set
tag_word = set()
tag_word.add('true')
tag_word.add('false')
class Lex():
    def __init__(self, statement):
        self.id_table = set()
        self.num_table = set()
        self.bool_table = set()
        self.lexDict = dict()
        self.token(statement)
        self.getToken(statement)
        

    def token(self, statement):
        copy = statement
        temp = statement.replace(' ', '') #skip the space
        for i in '&|<>=!();':
            temp = temp.replace(i, ' ') #skip all the terminal symbol
        
        split = temp.split(' ') #skip the space
        split = [i for i in split if(i.isalnum())] #catch all the number or English term
        for i in split:
            #If i is number, then add it to number list and store its position
            if i.isdigit():
                self.num_table.add(i)
                pos = len(statement.partition(i)[0]) #the position of the lex
                assert pos >= 0
                self.lexDict[pos] = i
                statement = statement[:pos] + '#'*len(i) + statement[pos + len(i):] if pos > 0 else '#'*len(i) + statement[pos + len(i):] #replace the lex to other chars
                assert len(statement) == len(copy)
            #If i is identifier, then add it to id list and store its pos
            if i.isidentifier():
                #for the non reversed word
                if i not in tag_word:
                    self.id_table.add(i)
                    pos = len(statement.partition(i)[0]) 
                    assert pos >= 0
                    self.lexDict[pos] = i
                    statement = statement[:pos] + '*'*len(i) + statement[pos + len(i):] if pos > 0 else '*'*len(i) + statement[pos + len(i):]
                    assert len(statement) == len(copy)
                #for the reversed word
                else:
                    self.bool_table.add(i)
                    pos = len(statement.partition(i)[0])
                    assert pos >= 0
                    self.lexDict[pos] = i
                    statement = statement[:pos] + '-'*len(i) + statement[pos + len(i):] if pos > 0 else '-'*len(i) + statement[pos + len(i):]
                    assert len(statement) == len(copy)

    def getToken(self, statement: str):
        
        tokenList = []
        i = 0
        gap = 1
        #Double check each token attribute and push it to tokenList
        while i < len(statement):
            s = statement[i]
            temp = list(self.lexDict.keys())
            if i in temp:
                tokenList.append(self.lexDict[i])
                gap = len(self.lexDict[i])
            elif s == '(':
                tokenList.append(s)
                gap = 1
            elif s ==')':
                tokenList.append(s)
                gap = 1
            elif s =='='and statement[i+1] == '=':
                tokenList.append('==')
                gap = 2
            elif s =='!' and statement[i+1] == '=':
                tokenList.append('!=')
                gap = 2
            elif s == '|' and statement[i+1] == '|':
                tokenList.append('||')
                gap = 2
            elif s == '&' and statement[i+1] == '&':
                tokenList.append('&&')
                gap = 2
            elif s == '<' and statement[i+1] == '=':
                tokenList.append('<=')
                gap = 2
            elif s == '>' and statement[i+1] == '=':
                tokenList.append('>=')
                gap = 2
            elif s == '<':
                tokenList.append(s)
                gap = 1
            elif s == '>':
                tokenList.append(s)
                gap = 1
            elif s == ';':
                tokenList.append(s)
                gap = 1
            elif s == '%':
                tokenList.append(s)
                gap = 1
            else:
                gap = 1

            i += gap


        self.tokenList = tokenList

 





            





   
