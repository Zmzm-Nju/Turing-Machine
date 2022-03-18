#ifndef _TURING_H_
#define _TURING_H_
#define Default_Size 32*2
#include<vector>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<iostream>

class Turing_Machine
{
public:
    //构造函数，根据输入文件和verbose_mode标志建立图灵机
    Turing_Machine(FILE* fp, bool mode);
    //根据输入input，运行该图灵机
    bool Run(char* input);
    //展示当前图灵机内所有数据，用于观察该图灵机的建立是否正确
    void Show();
    //判断某个状态是否处于接受状态
    bool In_final_state(char* state);
    //查询迁移函数，根据cur_state和cur_tape，得到需要写的符号、磁头移动方向和下一状态
    void Look_up_transfer_function(char* cur_state, char* cur_tape, char* &write_tape, char* &transfer_direction, char* &next_state);
    //判断当前解析是否正确，即.tm文件的格式是否存在语法错误
    bool Parsed_successfully(){return parsed_successfully;}
private:
    std::vector<char*> Q; //State set
    std::vector<char> S;  //Input symbol set
    std::vector<char> G;  //Tape symbol set
    char* q0;             //Initial state
    char B;               //Space symbol
    std::vector<char*> F; //Final state set
    int N;                //Number of tapes
    std::vector<char**> delta;//Transfer function
    bool verbose_mode;
    bool parsed_successfully;
};

class Tape
{
public:
    Tape();
    //设置当前磁带的输入
    void set_input(char* input);
    //返回当前磁头所指向的符号
    char cur_symbol() {return tape[cur_index];}
    //根据需要写下的符号和移动方向，进行磁头移动
    void move(char write_symbol, char direction);
    //展示当前磁带上的内容
    void show_tape();
    //verbose模式下展示当前磁带上的内容
    void show_tape_v(int n);
private:
    int size;       //当前磁带大小
    int left_index; //最左的非空白符符号的下标，如果磁头越过了最左的非空白符，则是磁头下标
    int right_index;//最右的非空白符符号的下标，如果磁头越过了最右的非空白符，则是磁头下标
    int mid_index;  //0位置的下标
    int cur_index;  //当前磁头所指位置
    char* tape;     //磁带
    void extend_left(); //往左扩充磁带
    void extend_right();//往右扩充磁带
};

#endif