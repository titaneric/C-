from lex import Lex
class Parser_LL1():
    def __init__(self, lex):
        self.tokenList = lex.tokenList
        self.idList = lex.id_table
        self.tokenList.append('$')
        self.role = []
        self.first = dict()
        self.followSource = []
        self.follow = dict()
        self.selectionSet = dict()
        self.pointer = ""
        self.stack = []
        self.stack.append('$')
        self.terminalSet = set()
        self.buildRole()
        self.buildFirst()
        self.buildFollow()
        self.buildSelectionSet()
        self.parseStatement()

    def buildRole(self):
        self.role.append(('Start', 'List %'))
        self.role.append(('List', 'BoolExp ; List'))
        self.role.append(('List', 'empty {self.dump_symbol_table()}'))
        self.role.append(('BoolExp', 'Join BoolR'))
        self.role.append(('BoolR', '|| Join {print("||")} BoolR'))
        self.role.append(('BoolR', 'empty'))
        self.role.append(('Join', 'Equality JoinR'))
        self.role.append(('JoinR', '&& Equality {print("&&")} JoinR'))
        self.role.append(('JoinR', 'empty'))
        self.role.append(('Equality', 'Rel EqualityR'))
        self.role.append(('EqualityR', '== Rel {print("==")} EqualityR'))
        self.role.append(('EqualityR', '!= Rel {print("!=")} EqualityR'))
        self.role.append(('EqualityR', 'empty'))
        self.role.append(('Rel', 'Factor RelR'))
        self.role.append(('RelR', '< Factor {print("<")} RelR'))
        self.role.append(('RelR', '> Factor {print(">")} RelR'))
        self.role.append(('RelR', '<= Factor {print("<=")} RelR'))
        self.role.append(('RelR', '>= Factor {print(">=")} RelR'))
        self.role.append(('RelR', 'empty'))
        self.role.append(('Factor', '( BoolExp )'))
        self.role.append(('Factor', 'lexWord {print("lexWord")}'))


        self.terminalSet = {"(", ")", "<", ">", "<=", ">=", "==", "!=", "&&", "||", "lexWord", '%', ";", "empty"}
        
        
        self.nonterminalList = ["Start", "List", "BoolExp", "BoolR", "Join", "JoinR", "Equality", "EqualityR", "Rel", "RelR", "Factor"]

        self.startSymbol = "Start"
        
        #There are some tests below
        #self.role.append(("C", "P F class identifier X Y"))
        #self.role["P"] = ["public", "empty"]
        #self.role["F"] = ["final", "empty"]
        #self.role["X"] = ["extends identifier", "empty"]
        #self.role["Y"] = ["implements I", "empty"]
        #self.role["I"] = ["identifier J"]
        #self.role["J"] = [", I", "empty"]
        #self.terminalSet = {"class", "identifier", "public", "final", "extends", "implements", ",", "empty"}
        #self.nonterminalList = ["C", "P", "F", "X", "Y", "I", "J"]
        #self.startSymbol = "C"

        #build the set of first and follow
        for nonterminal in self.nonterminalList:
            self.first[nonterminal] = set()
            self.follow[nonterminal] = set()
        #update the first of terminal
        self.first.update({terminal : {terminal} for terminal in self.terminalSet})

        return

    def findRoleIndex(self, nonTerminal):
        #find the given nonTerminal's grammar index and return
        indexList = []
        for index, grammar in enumerate(self.role):
            if grammar[0] == nonTerminal:
                indexList.append(index)

        return indexList

    def findnonTerminalIndex(self, nonT):
        #find the given nonTerminal's index and return
        for index, g in enumerate(self.nonterminalList):
            if g == nonT:
                return index

    def findFirst(self, attemptFind, nonTerminal):
            #if attemptFind is terminal, then add this to first of nonTerminal
            if attemptFind in self.terminalSet:
                self.first[nonTerminal].add(attemptFind)
            #else if attemptFind is nonTerminal, then try to find the first of attemptFind
            else:
                #if the first of nonTerminal is empty set, try to find the first of attemptFind
                if len(self.first[attemptFind]) == 0:
                    for roleIndex in self.findRoleIndex(attemptFind):
                        otherg = self.role[roleIndex][1]
                        index = 0
                        tempFirst = otherg.split()[index]
                        #recursively find the first of attemptFind
                        self.findFirst(tempFirst, attemptFind)
                        self.first[nonTerminal].update(self.first[attemptFind])
                #Contrarily, then update the first of attemptFind to first of nonTerminal
                else:
                    self.first[nonTerminal].update(self.first[attemptFind])

            return

    def buildFirst(self):
        
        
        for nonTerminal, g in self.role:
            
            index = 0
            firstWord = g.split()[index]
            self.findFirst(firstWord, nonTerminal)
                
            #while the FIRST of current index contains empty
            while (firstWord not in self.terminalSet) and ('empty' in self.first[firstWord]) and (index + 1 < len(g.split())): 
                index += 1 
                firstWord = g.split()[index]
                #update the FIRST of next index into nonterminal
                self.findFirst(firstWord, nonTerminal)
                #ignore the empty if the current index != length of grammar
                if index != len(g.split()):
                    self.first[nonTerminal].discard("empty")   
            #when all of grammar's token contains empty, add the empty into nonterminal
            if index == len(g.split()):
                self.first[nonTerminal].add("empty")

        return

    def syncFollow(self):
        #for those follow of followSource is updated, updated those follow of nonTerminal
        for sub in self.followSource:
            for nonT, subsource in sub.items():
                for subset in subsource:
                    if len(self.follow[subset] - self.follow[nonT]) > 0:
                        self.follow[nonT].update(self.follow[subset])

        return

    def buildFollow(self):
        
        #emptyFollowDict = {nonT : dict() for nonT in self.nonterminalList}
        for nonT in self.nonterminalList:
            self.followSource.append({nonT: set()})
        self.follow[self.startSymbol].add("$")
        for nonTerminal, g in self.role:
            g = g.split()
            if len(g)>1:
                for id, token in enumerate(g, 0):
                    #ignore the semantics actions and update the follow
                    #Ex: JoinR -> && Equality {print (“&&”)} JoinR 
                    if id + 1 < len(g) and (g[id+1] in self.terminalSet or g[id+1] in self.nonterminalList) and id-1 > 0 and g[id-1] not in self.terminalSet and '{' in token:
                        self.follow[g[id-1]].update(self.first[g[id+1]])
                        self.follow[g[id-1]].discard("empty")
                    #check the token for nonTerminal
                    elif token not in self.terminalSet and '{' not in token:
                        #consider the normal condition
                        if id + 1 < len(g) and (g[id+1] in self.terminalSet or g[id+1] in self.nonterminalList):
                            self.follow[token].update(self.first[g[id+1]])
                            tempIndex = id + 1
                            #consider the next token containing empty
                            while tempIndex < len(g) - 1 and "empty" in self.first[g[tempIndex]]:
                                tempIndex += 1
                                self.follow[token].update(self.first[g[tempIndex]])
                            self.follow[token].discard("empty")

                        #if the token is the last element in the g, then put the follow of nonTerminal in the follow of token
                        elif (id + 1 == len(g)) and (token not in self.terminalSet) and (token != nonTerminal):
                            listIndex = self.findnonTerminalIndex(token)
                            self.followSource[listIndex][token].add(nonTerminal)
                            self.follow[token].update(self.follow[nonTerminal])
                #consider the A -> aB or  A -> aBC where first of C containing empty
                #try to find the token containing empty from back of grammar 
                index = -1
                token = g[index]
                while ((index - 1) > - (len(g) + 1)) and (g[index - 1] not in (self.terminalSet)) and (token not in self.terminalSet) and ('{' not in token) and ('{' not in g[index-1]) and ("empty" in self.first[token]):
                    listIndex = self.findnonTerminalIndex(g[index - 1])
                    self.followSource[listIndex][g[index - 1]].add(nonTerminal)
                    self.follow[g[index - 1]].update(self.follow[nonTerminal])
                    index -= 1
                    token = g[index]
                    if index - 1 > - (len(g) + 1) and '{' in token:
                        index -= 1
                        token = g[token]
        #sync the follow
        self.syncFollow()
        return
        
    def buildSelectionSet(self):
        self.selectionSet = { nonT:dict() for nonT in self.nonterminalList}
        for nonT, g in self.role:
            g = g.split()
            token = g[0]
            setPool = self.first[token].copy() # notice that assign operator can't copy the collections, but copy does!

            #if the empty not in setPool, put all of first of token in the selection set of nonTerminal
            if 'empty' not in setPool:
                for subFirst in self.first[token]:
                    self.selectionSet[nonT][subFirst] = (nonT, g)
            #else, put the union of follow of (nonT \ empty) and first of token in the selection set of nonTerminal
            else:
                setPool.update(self.follow[nonT])
                setPool.discard('empty')
                for subSet in setPool:
                        self.selectionSet[nonT][subSet] = (nonT, g)


        return

    def parseStatement(self):
        #copy the tokenList to lexList
        lexList = self.tokenList.copy()
        lexWord_list = []
        
        #replace all of lexWord into 'lexWord' and push them into lexWord_list
        for i, token in enumerate(lexList):
            if token not in self.terminalSet and token != '$':
                lexList[i] = 'lexWord'
                lexWord_list.append(token)
        #put the startSymbol into stack
        self.stack.append(self.startSymbol)
        #assign the pointer to the first token of lexList
        self.index = 0
        self.pointer = lexList[self.index]
        #assign the lexword_ptr to the first token of lexIndex(to get the lexWord)
        lexIndex = 0
        lexword_ptr = lexWord_list[lexIndex]
        while True:
            X = self.stack[-1]
            #When accounting the $ symbol, end of algorithm
            if X == '$':
                break
            #When accounting the empty, pop it from stack
            elif X == 'empty':
                self.stack.pop()
            #When accounting the terminal, pop it and advance the pointer's
            elif X in self.terminalSet and X != 'empty':# or self.pointer == '$':
                if X == self.pointer:
                    self.stack.pop()
                    self.index += 1
                    if self.index < len(lexList):
                        self.pointer =lexList[self.index]
                else:
                    print("Terminal Error")
            #When accounting the nonTerminal, pop it and assign the reversed grammar of it to the stack 
            elif X in self.nonterminalList:
                if self.selectionSet[X][self.pointer][0] == X:
                    self.stack.pop()
                    self.stack.extend(reversed(self.selectionSet[X][self.pointer][1]))
                else:
                    print("Nonterminal Error")
            #execute the subprogram
            else:
                #print the nonTerminal 
                if 'lexWord' not in X and 'symbol' not in X:
                    X = X.replace('{', '')
                    X = X.replace('}', '')
                    X = X.replace(')', '')
                    X = X + ", end = \' \' )"
                    eval(X)
                #print the lexWord
                elif 'lexWord' in X:
                    print(lexword_ptr, end = ' ')
                    lexIndex += 1
                    if lexIndex < len(lexWord_list):
                        lexword_ptr = lexWord_list[lexIndex]
                #execute the dump_symbol_table function
                else:
                    X = X.replace('{', '')
                    X = X.replace('}', '')
                    print()
                    eval(X)
                self.stack.pop()



        return

    def dump_symbol_table(self):
        #print all of ID
        print("ID are below:")
        for i in self.idList:
            print(i, end = ' ')
        print()