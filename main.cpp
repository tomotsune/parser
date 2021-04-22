#include<iostream>
#include<vector>
#include<string>
#include<stack>

#define START_SYMBOL 'E'

class Tab {                   //预测分析表中单个产生式
public:
    Tab(char n,char e,std::string p):nonterminal_syb(n),terminal_syb(e),production(p) {};

    char nonterminal_syb;
    char terminal_syb;
    std::string production;
};

std::vector<Tab>  pTab;       //预测分析表
std::stack<char>  pStack;     //栈
std::string       pStr;       //输入串
int               index = 0;  //输入串的下标
int               num = 0;    //下标
char              X;          //从栈顶取出的符号
char              a;          //字符串目前分析到的位置
std::vector<char> VT{'i','+','*','(',')','#'};    //终结符号集合
std::vector<char> VN{'E','Z','T','Y','F'};    //非终结符号集合

int
isPartVT(char ch)
{
    for(auto u:VT) {
        if(u == ch) {
            return 1;
        }
    }
    return 0;
}

int
isPartVN(char ch)
{
    for(auto u:VN) {
        if(u == ch) {
            return 1;
        }
    }
    return 0;
}

void
initpTab()
{
    pTab.push_back(Tab('E','i',"TZ"));       //Z表示E'
    pTab.push_back(Tab('E','(',"TZ"));
    pTab.push_back(Tab('Z','+',"+TZ"));
    pTab.push_back(Tab('Z',')',"$"));
    pTab.push_back(Tab('Z','#',"$"));
    pTab.push_back(Tab('T','i',"FY"));
    pTab.push_back(Tab('T','(',"FY"));       //Y表示T'
    pTab.push_back(Tab('Y','+',"$"));
    pTab.push_back(Tab('Y','*',"*FY"));
    pTab.push_back(Tab('Y',')',"$"));
    pTab.push_back(Tab('Y','#',"$"));
    pTab.push_back(Tab('F','i',"i"));
    pTab.push_back(Tab('F','(',"(E)"));
}

void
printStack()
{
    std::stack<char> pTemp1(pStack);
    std::stack<char> pTemp; // 反转Stack用于输出.
    while(pTemp1.size()!=0){
        pTemp.push(pTemp1.top());
        pTemp1.pop();
    }
    while(pTemp.size() != 0) {
        if(pTemp.top()=='Z'){
            std::cout << "E'" << " ";
        }else if (pTemp.top()=='Y'){
            std::cout << "T'" << " ";
        }else{
            std::cout << pTemp.top() << " ";
        }
        pTemp.pop();
    }
    std::cout << "         ";
}

int
synaly()
{
    pStack.push(pStr[index++]);
    pStack.push(START_SYMBOL);
    a = pStr[index++];
    bool flag = true;
    while(flag) {
        std::cout << num++ << "        ";  //输出行号
        printStack();
        if(pStack.size() != 0) {
            X = pStack.top();              //将符号栈顶给X
            pStack.pop();
        }

        if(isPartVT(X)) {                  //如果是终结符
            if(X == '#' && a == '#') {
                flag = false;
            }else if(X == a){           //第一次栈内为E'T'i 时进入.
                std::cout << std::endl;
                a = pStr[index++];      //指向下非终结符
            }else {
                std::cout << "EROOR!" << std::endl;
                return 0;
            }

        }else if (X == '#') {
            if(X == a) {
                flag = false; //循环结束, 完成分析.
            }else {
                std::cout << "EROOR!" << std::endl;
                return 0;
            }

        }else if (isPartVN(X) && isPartVT(a)) {  //如果非终结符
            int type = 0; // 如果遍历预测分析表后没有找到对应产生式则报错.
            for(auto u:pTab) {
                if(u.nonterminal_syb == X && u.terminal_syb == a) {
                    std::string tempProd = u.production;
                    std::cout << X << "->" << tempProd << std::endl;
                    if(tempProd == "$") { // 判断完成.
                        type = 1;
                        break;
                    }else {
                        for(int i = tempProd.size()-1;i >= 0;--i) {
                            pStack.push(tempProd[i]);
                        }
                        type = 1;
                        break;
                    }
                }
            }
            if(type != 1) {
                std::cout << "EROOR!" << std::endl;
                return 0;

            }

        }else {
            std::cout << "EROOR!" << std::endl;
            return 0;
        }

    }
    return 1;
}

int main(int argc,char *argv[])
{
    initpTab();
    std::cout << "Input a sentence :";
    std::cin >> pStr;
    std::cout << "Step" << "     " << "symbol stack" << "      " << "production used" << std::endl;
    int flag = synaly();
    std::cout << std::endl;
    if(flag == 1) {
        std::cout << "success" << std::endl;
    }else {
        std::cout << "fail" << std::endl;
    }

    return 0;
}