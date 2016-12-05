from parse import Parser_LL1
from lex import Lex
for line in open('test.txt','r'):
    #skip the \n
    statement = line[:len(line)-1] 
    print("Original lex:{statement}".format(**locals()))
    statement += '%'#add the special end of line symbol
    lex = Lex(statement)
    print("Parsed lex:", end = "")
    parser = Parser_LL1(lex)





        
