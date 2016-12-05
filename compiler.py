from lex import Lex
from parse import Parse
def main():
    for line in open('test.txt','r'):
        #line = "(a>b)||(c<d)&&(e!=f);\n"
        line = line[:len(line)-1]   #skip the /n
        print("Original lex:{line}".format(**locals()))
        statement = line
        lex = Lex(statement)  
        parse = Parse(lex)


main()
