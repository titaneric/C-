from lex import Lex
from typing import NewType

class Parse():
    def __init__(self,lex):
        self.tokenList = lex.tokenList
        self.id_list = lex.id_table
        self.lex_list = list(lex.lexDict.values())
        self.index = 0
        self.lookahead = self.tokenList[self.index]
        self.start()

    def start(self):
        print("Parsed lex:",end = '')
        self.List()
        self.match('\n')
    
    def List(self):
        if (self.lookahead in self.lex_list) or (self.lookahead == '(') or (self.lookahead == ')'):
            self.BoolExp()
            self.match(';')
            self.List()
        elif self.endParse():
            self.dump_symbol_table()
    
    def Rel(self):
        self.Factor()
        self.RelR()

    def RelR(self):
        if self.lookahead == '<':
            self.match('<')
            self.Factor()
            print('<',end = ' ')
            self.RelR()
        elif self.lookahead == '>':
            self.match('>')
            self.Factor()
            print('>',end = ' ')
            self.RelR()

        elif self.lookahead == '>=':
            self.match('>=')
            self.Factor()
            print('>=',end = ' ')
            self.RelR()

        elif self.lookahead == '<=':
            self.match('<=')
            self.Factor()
            print('<=',end = ' ')
            self.RelR()
        else:
            self.empty()

   
    def Equality(self):
        self.Rel()
        self.EqualityR()

    def EqualityR(self):
        if self.lookahead == '==':
            self.match('==')
            self.Rel()
            print('==',end = ' ')
            self.EqualityR()
        elif self.lookahead == '!=':
            self.match('!=')
            self.Rel()
            print('!=',end = ' ')
            self.EqualityR()
        else:
            self.empty()
    def Join(self):
        self.Equality()
        self.JoinR()

    def JoinR(self):
        if self.lookahead == '&&':
            self.match('&&')
            self.Equality()
            print('&&',end = ' ')
            self.JoinR()
        else:
            self.empty()
  
    def BoolExp(self):
        self.Join()
        self.BoolR()

    def BoolR(self):
        if self.lookahead == '||':
            self.match('||')
            self.Join()
            print('||',end = ' ')
            self.BoolR() 
        else:
            self.empty()

    def Factor(self):
        if self.lookahead == '(':
            self.match('(')
            self.BoolExp()
            self.match(')')
        else:
            if self.lookahead in self.lex_list:
                matchLex = self.lookahead
                self.match(self.lookahead)
                print(matchLex,end = ' ')

    def match(self, token):
        if self.lookahead == token:
            self.index += 1
            if self.index == len(self.tokenList):
                self.endParse()
            else:
                self.lookahead = self.tokenList[self.index]
            

    def endParse(self): #return true when scan finished
        return True

    def empty(self): #return
        return

    def dump_symbol_table(self): #print all of the ID
        print()
        print("Identifier in this statement are below:")
        for i in self.id_list:
            print(i, end = ' ')

        print()

    
    


        