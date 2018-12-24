#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <stack>
using namespace std;

enum{CONST = 1, VAR, PROCEDURE, ODD, IF, THEN, ELSE, WHILE, DO, CALL, BEGIN, END, READ, WRITE, REPEAT, UNTIL,IDENTIFIER, UINT,COMMA, SEMICOLON, ASSIGN, LP, RP, POINT,PLUSO, MULTIPLYO, RELATIONO };

string keywords[] = {"const", "var", "procedure", "odd", "if","then", "else", "while", "do", "call","begin", "end", "read", "write", "repeat", "until" };

const int keywordsLen = 16;

class FileOperator
{
private:
public:
	static void operation(int addr)
	{
	    ofstream objectCode("../output.txt", ios::app);
        objectCode << "OPR\t0\t" << addr << endl;
	}
	static void load(int l, int addr)
	{
        ofstream objectCode("../output.txt", ios::app);
        objectCode << "LOD\t" << l << "\t" << addr << endl;
	}
	static void store(int l, int addr)
	{
	    ofstream objectCode("../output.txt", ios::app);
        objectCode << "STO\t" << l << "\t" << addr << endl;
	}
	static void call(int l, int addr)
	{
        ofstream objectCode("../output.txt", ios::app);
        objectCode << "CAL\t" << l << "\t" << addr << endl;
	}
	static void increaseTop(int addr)
	{
        ofstream objectCode("../output.txt", ios::app);
        objectCode << "INT\t0\t" << addr << endl;
	}
	static void jump(int addr)
	{
        ofstream objectCode("../output.txt", ios::app);
        objectCode << "JMP\t0\t" << addr << endl;
	}
	static void jumpCompare(int addr)
	{
        ofstream objectCode("../output.txt", ios::app);
        objectCode << "JPC\t0\t" << addr << endl;
	}
	static void read(int l, int addr)
	{
	    ofstream objectCode("../output.txt", ios::app);
        objectCode << "RED\t" << l << "\t" << addr << endl;
	}
	static void write()
	{
        ofstream objectCode("../output.txt", ios::app);
        objectCode << "WRT\t0\t0" << endl;
	}
};

class SymbolTable
{
public:
	SymbolTable()
	{
        setKind(-1);
        setValue(NULL);
        setLevel(-1);
        setAddress(-1);
	}
    SymbolTable(string = NULL, int = -1, string = NULL, int = -1, int = -1);
	~SymbolTable()
	{
	    setName(NULL);
        setKind(-1);
        setValue(NULL);
        setLevel(-1);
        setAddress(-1);
	}

private:
	string name;
	int kind;
	string value;
	int level;
	int address;

public:
	void setName(string n)
	{
	    name = n;
	}
	void setKind(int k)
	{
	    kind = k;
	}
	void setValue(string v)
	{
	    value = v;
	}
	void setLevel(int l)
	{
	    level = l;
	}
	void setAddress(int addr)
	{
	    address = addr;
	}
	string getName() const
    {
        return name;
    }
	int getKind() const
	{
	    return kind;
    }
	string getValue() const
	{
	    return value;
    }
	int getLevel() const
	{
	    return level;
    }
	int getAddress() const
	{
	    return address;
    }
};

class SymbolTableManager
{
public:
	SymbolTableManager()
	{
        setCurrentLevel(1);
        setCurrentOffset(3);
	}

private:
	stack<SymbolTable*> table;
	string curName;
	int curType;
	int curLevel;
	int curOffset;
	string curValue;
public:
	bool insert(SymbolTable* t)
	{
        try
        {
		table.push(t);
		curName = " ";
		curValue = " ";
        }
        catch (exception e)
        {
            return false;
        }
        return true;
	}
	void pop()
	{
	    table.pop();
	}
	SymbolTable* top() const
	{
        SymbolTable* entry = table.top();
        return entry;
	}
	bool find(string s)
	{
	    stack<SymbolTable*> tableBuff = table;
        int len = tableBuff.size();
        for (int i = 0; i < len; i++)
        {
            if (s == tableBuff.top()->getName()) return true;
            tableBuff.pop();
        }
        return false;
	}
    SymbolTable* getTableEntry(string s)
	{
	    stack<SymbolTable*> tableBuff = table;
        int len = tableBuff.size();
        for (int i = 0; i < len; i++)
        {
            if (s == tableBuff.top()->getName()) return tableBuff.top();
            tableBuff.pop();
        }
        return NULL;
	}
public:
	void setCurrentName(string n)
	{
	    curName = n;
	}
	void setCurrentType(int t)
	{
	    curType = t;
	}
	void setCurrentLevel(int l)
	{
	    curLevel = l;
	}
	void setCurrentOffset(int os)
	{
	    curOffset = os;
	}
	void setCurrentValue(string v)
	{
	    curValue = v;
	}
	string getCurrentName() const { return curName; }
	int getCurrentType() const { return curType; }
	int getCurrentLevel() const { return curLevel; }
	int getCurrentOffset() const{ return curOffset; }
	string getCurrentValue() const { return curValue; }
	stack<SymbolTable*> getSymbolTable() const { return table; }
	void increaseLevel() { curLevel++; }
	void increaseOffset() { curOffset++; }
	void decreaseLevel() { curLevel--; }
	void decreaseOffset() { curOffset--; }
};

class WordAnalyzer
{
private:
	int type;
	int num;
	char* token;
	char* symbol;
	vector<string> symbols;
	static int cur ;
public:
	WordAnalyzer()
	{
	    initialData();
	}
	~WordAnalyzer()
	{
        delete token;
        delete symbol;
	}
public:
	void setType(int t)
	{
	    type = t;
	}
	void setNum(int n)
	{
	    num = n;
	}
	void setSymbol()
	{
        cur++;
        if (cur >= (int)symbols.size()) { cout << "\nanalyze complete!" << endl; exit(1); }
        string curSymbol(symbols[cur]);
        int length = symbols[cur].length();
        char str[20] = { " " };
        symbols[cur].copy(str, length, 0);
        setSymbol(str);
        analyze();
	}
	int getType() const
	{
	    return type;
	}
	int getNum() const
	{
	    return num;
	}
	string getToken() const
	{
        string str(token);
        return str;
	}
	string getSymbol() const
	{
        string str(symbol);
        return str;
	}
	bool fillSymbols()
	{
		char* fileName;
		cout<<"Please input filename(full name):"<<endl;
		cin>>fileName;
		cout<<"File name is "<<fileName<<endl;
        ifstream sourceFile(fileName, ios::in);
        if (!sourceFile)
        {
            cerr << "File could not be opened!" << endl;
            return false;
        }
        string c;
        while (sourceFile >> c)
        {
            symbols.push_back(c);
        }
        return true;
	}
	void initialData()
	{
	    setToken("");
        setSymbol("");
        setNum(-1);
        setType(-1);
	}
	void analyze()
	{
	    if (isPlus()) { setType(PLUSO); setToken(symbol); }
        else if (isMul()) { setType(MULTIPLYO); setToken(symbol); }
        else if (isRel()) { setType(RELATIONO); setToken(symbol); }
        else if (isComma()) { setType(COMMA); setToken(symbol); }
        else if (isSemicolon()) { setType(SEMICOLON); setToken(symbol); }
        else if (isAssign()) { setType(ASSIGN); setToken(symbol); }
        else if (isLP()) { setType(LP); setToken(symbol); }
        else if (isRP()) { setType(RP); setToken(symbol); }
        else if (isLetter(*symbol))
        {
            int length = strlen(symbol);
            bool flag = true;
            for (int i = 1; i < length; i++)
            {
                if (isLetter(symbol[i]) || isDigit(symbol[i])) flag = true;
                else { flag = false; break; }
            }
            if (flag)
            {
                string str(symbol);
                int* t = new int(0);
                if (isKeywords(str,t)) setType(*t);
                else setType(IDENTIFIER);
                setToken(symbol);
            }
            else
            {
                cout << "\nerror:  Declaration of token \" " << this->getSymbol() << " \" is invalid\n" << endl;
            }
        }
        else if (isDigit(*symbol))
        {
            int length = strlen(symbol);
            bool flag = true;
            for (int i = 1; i < length; i++)
            {
                if (isDigit(symbol[i]))
                {
                    flag == true;
                }
                else
                {
                    flag = false;
                    break;
                }
            }
            if (flag)
            {
                setType(UINT);
                setToken(symbol);
            }
            else
            {
                cout << "\nerror:  Declaration of token \" " << this->getSymbol() << " \" is invalid\n" << endl;
            }
        }
        else
        {
            cout << "\nerror:  Declaration of token \" " << this->getSymbol() << " \" is invalid\n" << endl;
        }
    }
	void back()
	{
        cur--;
        if (cur < 0) return;
	}
private:
	bool isPlus()
	{
	    int length = strlen(symbol);
        if (length == 1)
        {
            if ((*symbol == '+') || (*symbol == '-'))
                return true;
		return false;
        }
        return false;
	}
	bool isMul()
	{
        int length = strlen(symbol);
        if (length == 1)
        {
            if ((*symbol == '*') || (*symbol == '/'))
                return true;
            return false;
        }
        return false;
	}
	bool isRel()
	{
        int length = strlen(symbol);
        if (length == 1)
        {
            if (isEqual(*symbol) || (*symbol=='<') || (*symbol=='>'))
            {
                return true;
            }
            return false;
        }
        else if (length == 2)
        {
            if (((*symbol == '<') && (*(symbol + 1) == '>')) ||((*symbol == '<') && (*(symbol + 1) == '=')) ||((*symbol == '>') && (*(symbol + 1) == '=')))
            {
                return true;
            }
            return false;
        }
        return false;
	}
	bool isComma()
	{
	    int length = strlen(symbol);
        if (length == 1)
        {
            if (*symbol == ',')
                return true;
            return false;
        }
        return false;
	}
	bool isColon(char c)
	{
        if (c == ':')
            return true;
        return false;
	}
	bool isSemicolon()
	{
        int length = strlen(symbol);
        if (length == 1)
        {
            if (*symbol == ';')
                return true;
            return false;
        }
        return false;
	}
	bool isEqual(char c)
	{
	    if (c == '=')
            return true;
        return false;
	}
	bool isAssign()
	{
        int length = strlen(symbol);
        if (length == 2)
        {
            if (isColon(*symbol) && isEqual(*(symbol + 1)))
                return true;
            return false;
        }
        return false;
	}
	bool isLP()
	{
	    int length = strlen(symbol);
        if (length == 1)
        {
            if (*symbol == '(')
                return true;
            return false;
        }
        return false;
	}
	bool isRP()
	{
	    int length = strlen(symbol);
        if (length == 1)
        {
            if (*symbol == ')')
                return true;
            return false;
        }
        return false;
	}
	bool isPoint()
	{
        int length = strlen(symbol);
        if (length == 1)
        {
            if (*symbol == '.')
                return true;
            return false;
        }
        return false;
	}
	bool isLetter(char c)
	{
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
            return true;
        return false;
	}
	bool isDigit(char c)
	{
	    if (c >= '0' && c <= '9')
            return true;
        return false;
	}
	void setSymbol(char* s)
	{
        int len = strlen(s);
        symbol = new char[len+1];
        for (int i = 0; i < len; i++)
        {
            symbol[i] = s[i];
        }
        symbol[len] = '\0';
	}
	void setToken(char* t)
	{
        int len = strlen(t);
        token = new char[len + 1];
        for (int i = 0; i < len; i++)
        {
            token[i] = t[i];
        }
            token[len] = '\0';
    }
	bool isKeywords(string str,int* t)
	{
        for (int i = 0; i < keywordsLen; i++)
        {
            if (str == keywords[i])
            {
                *t = i + 1;
                return true;
                break;
            }
        }
        return false;
	}

};

SymbolTable::SymbolTable(string s, int k, string v, int l, int addr)
{
	setName(s);
	setKind(k);
	setValue(v);
	setLevel(l);
	setAddress(addr);
}

int WordAnalyzer::cur = -1;

class SyntaxAnalyzer
{
private:
	WordAnalyzer wordAnalyzer;
	SymbolTableManager* tableManager;

public:
	SyntaxAnalyzer()
	{
        setWordAnalyzer();
        tableManager = new SymbolTableManager();
	}
	~SyntaxAnalyzer()
	{
        delete tableManager;
	}

private:
	void program()
	{
	    partialProgram();
	}
	void partialProgram()
	{
	    if (wordAnalyzer.getType() == CONST) { constPart(); partialProgram(); }
        else if (wordAnalyzer.getType() == VAR) { varPart(); partialProgram(); }
        else if (wordAnalyzer.getType() == PROCEDURE) procedurePart();
        else { sentence(); partialProgram(); }
        wordAnalyzer.setSymbol();
	}
	void constPart()
	{
	    tableManager->setCurrentType(wordAnalyzer.getType());
        wordAnalyzer.setSymbol();
        constDef();
        while (wordAnalyzer.getType() == COMMA)
        {
            wordAnalyzer.setSymbol();
            constDef();
        }
	}
	void constDef()
	{
	    identifier(0);
        if (wordAnalyzer.getType() == RELATIONO && wordAnalyzer.getToken() == "=")
        {
            wordAnalyzer.setSymbol();
            if (wordAnalyzer.getType() == UINT)
            {
                tableManager->setCurrentValue(wordAnalyzer.getToken());
                wordAnalyzer.setSymbol();
                if ((tableManager->getCurrentName()) != " ")
                    tableManager->insert(new SymbolTable(tableManager->getCurrentName(),tableManager->getCurrentType(),tableManager->getCurrentValue(),tableManager->getCurrentLevel()));
            }
        }
    }
	bool identifier(int checkStatus)
	{
	    if (wordAnalyzer.getType() == IDENTIFIER)
        {
            tableManager->setCurrentName(wordAnalyzer.getToken());
            if (checkStatus)
            {
                if (!tableManager->find(wordAnalyzer.getToken()))
                {
                    cout << "identifier " << wordAnalyzer.getToken() << " not declared!" << endl;
                }
            }
		wordAnalyzer.setSymbol();
		return true;
        }
        else
        {
            wordAnalyzer.setSymbol();
        }
        return false;
	}
	void varPart()
	{
        tableManager->setCurrentType(wordAnalyzer.getType());
        wordAnalyzer.setSymbol();
        identifier(0);
        if ((tableManager->getCurrentName()) != " ")
            tableManager->insert(new SymbolTable(tableManager->getCurrentName(),tableManager->getCurrentType(),tableManager->getCurrentValue(),tableManager->getCurrentLevel(),tableManager->getCurrentOffset()));
        while (wordAnalyzer.getType() == COMMA)
        {
            wordAnalyzer.setSymbol();
            identifier(0);
            tableManager->increaseOffset();
            if ((tableManager->getCurrentName()) != " ")
                tableManager->insert(new SymbolTable(tableManager->getCurrentName(),tableManager->getCurrentType(),tableManager->getCurrentValue(),tableManager->getCurrentLevel(),tableManager->getCurrentOffset()));
        }
        tableManager->setCurrentOffset(3);
        printSymbleTable();
	}
	void procedurePart()
	{
	    tableManager->setCurrentType(wordAnalyzer.getType());
        procedureHeader();
        if(wordAnalyzer.getType() == SEMICOLON)
        {
        }
        else
        {
        }
	}
	void procedureHeader()
	{
	    wordAnalyzer.setSymbol();
        identifier(0);
        if (wordAnalyzer.getType() == SEMICOLON)
        {
            if ((tableManager->getCurrentName()) != " ")
			tableManager->insert(new SymbolTable(tableManager->getCurrentName(),tableManager->getCurrentType(),tableManager->getCurrentValue(),tableManager->getCurrentLevel()));
            tableManager->increaseLevel();
            wordAnalyzer.setSymbol();
            partialProgram();
		while (tableManager->top()->getLevel() == tableManager->getCurrentLevel())
		{
			tableManager->pop();
		}
            tableManager->decreaseLevel();
        }
	}
	void sentence()
	{
        if (wordAnalyzer.getType() == IDENTIFIER) assignSentence();
        else if(wordAnalyzer.getType()==IF) conditionSentence();
        else if (wordAnalyzer.getType() == WHILE) whileSentence();
        else if (wordAnalyzer.getType() == CALL) procedureCallSentence();
        else if (wordAnalyzer.getType() == READ) readSentence();
        else if (wordAnalyzer.getType() == WRITE) writeSentence();
        else if (wordAnalyzer.getType() == BEGIN) uniteSentence();
        else if (wordAnalyzer.getType() == REPEAT) repeatSentence();
        wordAnalyzer.setSymbol();
	}
	void assignSentence()
	{
	    if (!tableManager->find(wordAnalyzer.getToken()))
        {
            cout << "identifier " << wordAnalyzer.getToken() << " not declared!" << endl;
        }
            else
            {
                SymbolTable* st = tableManager->getTableEntry(wordAnalyzer.getToken());
                int l = tableManager->getCurrentLevel() - st->getLevel();
                FileOperator::store(l, st->getAddress());
            }
        wordAnalyzer.setSymbol();
        if (wordAnalyzer.getType() == ASSIGN)
        {
            expression();
        }
    }
	void expression()
	{
	    wordAnalyzer.setSymbol();
        bool isPlus = wordAnalyzer.getType() == PLUSO ? true : false;
        if (!isPlus) wordAnalyzer.back();
        wordAnalyzer.setSymbol();
        item();
        while (wordAnalyzer.getType() == RELATIONO)
        {
            if (wordAnalyzer.getToken() == "+") FileOperator::operation(2);
            else FileOperator::operation(3);
            item();
            wordAnalyzer.setSymbol();
        }
	}
	void item()
	{
	    factor();
        while (wordAnalyzer.getType() == MULTIPLYO)
        {
            if (wordAnalyzer.getToken() == "*") FileOperator::operation(4);
            else FileOperator::operation(5);
            wordAnalyzer.setSymbol();
            factor();
        }
	}
	void factor()
	{
	    if (wordAnalyzer.getType() == LP)
        {
            expression();
            wordAnalyzer.setSymbol();
            if (wordAnalyzer.getType() == RP)
            {
                wordAnalyzer.setSymbol();
            }
        }
        else if (wordAnalyzer.getType()==IDENTIFIER)
        {
            if (!tableManager->find(wordAnalyzer.getToken()))
            {
                cout << "identifier " << wordAnalyzer.getToken() << " not declared!" << endl;
            }
            else
            {
                SymbolTable* st = tableManager->getTableEntry(wordAnalyzer.getToken());
                int l = tableManager->getCurrentLevel() - st->getLevel();
                FileOperator::load(l, st->getAddress());
            }
            wordAnalyzer.setSymbol();
        }
        else if (wordAnalyzer.getType() == UINT)
        {
            wordAnalyzer.setSymbol();
        }
	}

	void conditionSentence()
	{
	    condition();
        wordAnalyzer.setSymbol();
        if (wordAnalyzer.getType() == THEN)
        {
            sentence();
            if (wordAnalyzer.getType() == ELSE)
            {
                wordAnalyzer.setSymbol();
                sentence();
            }
        }
	}
	void condition()
	{
        wordAnalyzer.setSymbol();
        if (wordAnalyzer.getType() == ODD)
        {
            expression();
        }
        else
        {
            expression();
            wordAnalyzer.setSymbol();
            if (wordAnalyzer.getType() == RELATIONO)
            {
                expression();
            }
        }
	}

	void whileSentence()
	{
        condition();
        wordAnalyzer.setSymbol();
        if (wordAnalyzer.getType() == DO)
        {
            sentence();
        }
	}
	void procedureCallSentence()
	{
        wordAnalyzer.setSymbol();
        if (wordAnalyzer.getType() == IDENTIFIER)
        {
            if (!tableManager->find(wordAnalyzer.getToken()))
            {
                cout << "identifier " << wordAnalyzer.getToken() << " not declared!" << endl;
            }
            else
            {
                SymbolTable* st = tableManager->getTableEntry(wordAnalyzer.getToken());
                int l = tableManager->getCurrentLevel() - st->getLevel();
                FileOperator::call(l, st->getAddress());
            }
        }
	}
	void readSentence()
	{
	    wordAnalyzer.setSymbol();
        if (wordAnalyzer.getType() == LP)
        {
            wordAnalyzer.setSymbol();
            identifier(1);
            SymbolTable* st = tableManager->getTableEntry(tableManager->getCurrentName());
            int l = tableManager->getCurrentLevel() - st->getLevel();
            FileOperator::read(l,st->getAddress());
            while (wordAnalyzer.getType() == COMMA)
            {
                wordAnalyzer.setSymbol();
                identifier(1);
                SymbolTable* st = tableManager->getTableEntry(tableManager->getCurrentName());
                int l = tableManager->getCurrentLevel() - st->getLevel();
                FileOperator::read(l, st->getAddress());
            }
        }
	}
	void writeSentence()
	{
	    wordAnalyzer.setSymbol();
        if (wordAnalyzer.getType() == LP)
        {
            wordAnalyzer.setSymbol();
            identifier(1);
            FileOperator::write();
            while (wordAnalyzer.getType() == COMMA)
            {
                wordAnalyzer.setSymbol();
                identifier(1);
                FileOperator::write();
            }
        }
	}
	void uniteSentence()
	{
        wordAnalyzer.setSymbol();
        sentence();
        while (wordAnalyzer.getType() == SEMICOLON)
        {
            wordAnalyzer.setSymbol();
            sentence();
        }
        if (wordAnalyzer.getType() == END)
        {
			cout<<"Analyzer meets '#'"<<endl;
        }
    }
	void repeatSentence()
	{
        sentence();
        while (wordAnalyzer.getType() == SEMICOLON)
        {
            wordAnalyzer.setSymbol();
            sentence();
        }
        if (wordAnalyzer.getType() == UNTIL)
        {
            condition();
        }
	}
public:
	void setWordAnalyzer()
	{
        wordAnalyzer.fillSymbols();
	}
	WordAnalyzer getWordAnalyzer() const
	{
	    return wordAnalyzer;
	}
	void analyze()
	{
	    wordAnalyzer.setSymbol();
        program();
	}
	void printSymbleTable()
	{
	    cout << "\nPrinting Symbol Table...\n";
        cout << "\tname" << "\ttype" << "\tvalue" << "\tlayer" << "\toffset" << endl;
        stack<SymbolTable*> ts = tableManager->getSymbolTable();
        int len = ts.size();
        for (int i = 0; i < len; i++)
        {
            SymbolTable* t = ts.top();
            cout << "\t" << t->getName() << "\t" << t->getKind() << "\t" << t->getValue() << "\t"<< t->getLevel() << "\t" << t->getAddress() << " \n";
            ts.pop();
        }
        cout << endl;
	}
};

int main(void)
{
	cout << "\n----------Program start----------\n" << endl;

	SyntaxAnalyzer* syntaxAnalyzer = new SyntaxAnalyzer();
	syntaxAnalyzer->analyze();
	delete syntaxAnalyzer;
}
