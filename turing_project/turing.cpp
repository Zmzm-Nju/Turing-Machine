#include "turing.h"
Turing_Machine::Turing_Machine(FILE*fp, bool mode)
{
    verbose_mode = mode;
    int count = 0;
    char Buffer[1024], Processed_Buffer[1024];
    while(!feof(fp))
    {
        fgets(Buffer,1024,fp);
        int i = 0;
        //Skip Space in the front of a line
        while(Buffer[i] == ' ') i++;
        //Skip blank lines and comments
        if(Buffer[i]=='\n'||Buffer[i]==';');
        else if(Buffer[i] == '#')
        {
            //Eliminate spaces
            int j = i + 1, k = 0;
            for(; Buffer[j] != ';' && Buffer[j] != '\n' && Buffer[j] != '\0';j++)
                if(Buffer[j] != ' ')
                    Processed_Buffer[k++] = Buffer[j];
            Processed_Buffer[k] = '\0';
            //std::cout<< Processed_Buffer << std::endl;
            if(strncmp(Processed_Buffer,"Q={",3) == 0)
            {
                int n = 3;
                while(Processed_Buffer[n] != '}')
                {
                    char* temp = new char[128];
                    int m = 0;
                    while(Processed_Buffer[n] != ',' &&  Processed_Buffer[n] != '}'&&  Processed_Buffer[n] != '\0')
                    {
                        temp[m] = Processed_Buffer[n];
                        m++, n++;
                    }
                    temp[m] = '\0';
                    Q.push_back(temp);
                    if(Processed_Buffer[n] == ',') n++;
                }
            }
            else if(strncmp(Processed_Buffer,"S={",3) == 0)
            {
                int n = 3;
                while(Processed_Buffer[n] != '}')
                {
                    S.push_back(Processed_Buffer[n++]);
                    if(Processed_Buffer[n] == ',') n++;
                }
            }
            else if(strncmp(Processed_Buffer,"G={",3) == 0)
            {
                int n = 3;
                while(Processed_Buffer[n] != '}')
                {
                    G.push_back(Processed_Buffer[n++]);
                    if(Processed_Buffer[n] == ',') n++;
                }
            }
            else if(strncmp(Processed_Buffer,"q0=",3) == 0)
            {
                int n = 3, m = 0;
                char* temp = new char[128];
                while(Processed_Buffer[n] != '\0')
                {
                    temp[m] = Processed_Buffer[n];
                    m++, n++;
                }
                temp[m] = '\0';
                q0 = temp;
            }
            else if(strncmp(Processed_Buffer,"B=",2) == 0)
                B = Processed_Buffer[2];
            else if(strncmp(Processed_Buffer,"F={",3) == 0)
            {
                int n = 3;
                while(Processed_Buffer[n] != '}')
                {
                    char* temp = new char[128];
                    int m = 0;
                    while(Processed_Buffer[n] != ',' &&  Processed_Buffer[n] != '}'&&  Processed_Buffer[n] != '\0')
                    {
                        temp[m] = Processed_Buffer[n];
                        m++, n++;
                    }
                    temp[m] = '\0';
                    F.push_back(temp);
                    if(Processed_Buffer[n] == ',') n++;
                }
            }
            else if(strncmp(Processed_Buffer,"N=",2) == 0)
            {
                int n = 2, m = 0;
                char* temp = new char[128];
                while(Processed_Buffer[n] != '\0')
                {
                    temp[m] = Processed_Buffer[n];
                    m++, n++;
                }
                temp[m] = '\0';
                N = atoi(temp);
            }
            else continue;
            count++;
        }
        else //Input the transfer functions
        {
            if(count != 7)
            {
                parsed_successfully = false;
                return;
            }
            char** temp = new char*[5];
            for(int j = 0;j <= 4; j++)
            {
                temp[j] = new char[128];
                int k = 0;
                while(Buffer[i]!=' ' && Buffer[i]!=';' && Buffer[i]!='\n' && Buffer[i]!='\0')
                {
                    temp[j][k] = Buffer[i];
                    i++, k++;
                }
                temp[j][k] = '\0';
                while(Buffer[i] == ' ') i++;
            }
            delta.push_back(temp);
        }
    }
    //Show();
    parsed_successfully = true;
}
bool Turing_Machine::Run(char* input)
{
    if(verbose_mode)
        std::cout << "Input: " << input << std::endl;
    //Check if the input is legal
    for(int i = 0; input[i] != '\0'; i++)
    {
        bool find = false;
        for(int j = 0; j < S.size(); j++)
            if(S[j] == input[i])
            {
                find = true;
                break;
            }
        if(!find)
        {
            if(verbose_mode)
            {
                std::cout << "==================== ERR ====================" << std::endl;
                std::cout << "error: '" << input[i] << "' was not declared in the set of input symbols" << std::endl;
                std::cout << "Input: " << input << std::endl;
                std::cout << "       ";
                for(int k = 0; k < i; k++) std::cout << " ";
                std::cout << "^" << std::endl;
                std::cout << "==================== END ====================" << std::endl;
            }
            else
                std::cerr << "illegal input" << std::endl;
            return false;
        }
    }
    int step = 0;
    char* cur_state = q0;
    Tape tapes[N];
    tapes[0].set_input(input);
    if(verbose_mode)
    {
        std::cout << "==================== RUN ====================" << std::endl;
        std::cout << "Step   : " << step << std::endl;
        for(int i = 0; i < N; i++)
            tapes[i].show_tape_v(i);
        std::cout << "State  : " << cur_state << std::endl;
        std::cout << "---------------------------------------------" << std::endl;
    }
    char* write_tape = NULL;
    char* transfer_direction = NULL;
    char* next_state = NULL;
    while(!In_final_state(cur_state))
    {
        //int debug = 0;
        char* cur_tape = new char[N+1];
        for(int i = 0;i < N; i++)
            cur_tape[i] = tapes[i].cur_symbol();
        cur_tape[N] = '\0';
        //std::cout << cur_tape << std::endl;
        next_state = NULL;
        Look_up_transfer_function(cur_state,cur_tape,write_tape,transfer_direction,next_state);
        if(next_state == NULL)
        {
            if(verbose_mode)
            {
                std::cout << "Result: ";
                tapes[0].show_tape();
                std::cout << "==================== END ====================" << std::endl;
            }
            else tapes[0].show_tape();
            return true;
        }
        else{
            for(int i = 0; i < N; i++)
                tapes[i].move(write_tape[i],transfer_direction[i]);
            cur_state = next_state;
            step++;
            if(verbose_mode)
            {
                std::cout << "Step   : " << step << std::endl;
                for(int i = 0; i < N; i++)
                    tapes[i].show_tape_v(i);
                std::cout << "State  : " << cur_state << std::endl;
                std::cout << "---------------------------------------------" << std::endl;
            }
        }
        //std::cin >> debug;
    }
    if(verbose_mode)
    {
        std::cout << "Result: ";
        tapes[0].show_tape();
        std::cout << "==================== END ====================" << std::endl;
    }
    else tapes[0].show_tape();
    return true;
}
void Turing_Machine::Show()
{
    std::cout << "Q: ";
    for(int n = 0; n < Q.size(); n++)
        std::cout << Q[n] << " ";
    std::cout << std::endl;

    std::cout << "S: ";
    for(int n = 0; n < S.size(); n++)
        std::cout << S[n] << " ";
    std::cout << std::endl;

    std::cout << "G: ";
    for(int n = 0; n < G.size(); n++)
        std::cout << G[n] << " ";
    std::cout << std::endl;

    std::cout << "q0: " << q0 << std::endl;
    std::cout << "B: " << B << std::endl;
    std::cout << "N: " << N << std::endl;

    std::cout << "F: ";
    for(int n = 0; n < F.size(); n++)
        std::cout << F[n] << " ";
    std::cout << std::endl;

    std::cout << "delta:" << std::endl;
    for(int n = 0; n < delta.size(); n++)
	    std::cout << delta[n][0] << " " << delta[n][1] << " " << delta[n][2] << " " << delta[n][3] << " " << delta[n][4] << std::endl;
}
bool Turing_Machine::In_final_state(char* state)
{
    for(int i = 0; i < F.size(); i++)
        if(strcmp(state, F[i]) == 0)
            return true;
    return false;
}
void Turing_Machine::Look_up_transfer_function(char* cur_state,char* cur_tape,char* &write_tape,char* &transfer_direction,char* &next_state)
{
    for(int i = 0; i < delta.size(); i++)
    {
        //std::cout << delta[i][0] << " " << delta[i][1] << " " << delta[i][2] << " " << delta[i][3] << " " << delta[i][4] << std::endl;
        if(strcmp(cur_state, delta[i][0]) == 0)
        {
            bool match = true;
            for(int j = 0; j < N; j++)
                if(delta[i][1][j] != cur_tape[j] && delta[i][1][j] != '*')
                {
                    match = false;
                    break;
                }
            if(match)
            {
                write_tape = delta[i][2];
                transfer_direction = delta[i][3];
                next_state = delta[i][4];
                //std::cout << next_state << std::endl;
                break;
            }
        }
    }
}

Tape::Tape()
{
    size = Default_Size;
    left_index = Default_Size/2;
    right_index = Default_Size/2;
    mid_index = Default_Size/2;
    cur_index = Default_Size/2;
    tape = new char[size + 1];
    memset(tape, 0x5F, size);
    tape[size] = '\0';
}
void Tape::set_input(char* input)
{
    int len = strlen(input);
    while(size - cur_index < len)
        extend_right();
    strncpy(tape + cur_index, input, len);
    right_index = mid_index + len - 1;
    /*
    std::cout << left_index << " " << right_index << " " << cur_index << " " << mid_index << std::endl;
    std::cout << size << std::endl;
    std::cout << tape << std::endl;
    */
}
void Tape::move(char write_symbol, char direction)
{
    if(write_symbol == '*')
        write_symbol = tape[cur_index]; 
    if(direction == '*')
        tape[cur_index] = write_symbol;
    else if(direction == 'l')
    {
        if(cur_index == 0)
            extend_left();
        //在最左端放下了空白符
        if(cur_index == right_index && write_symbol == '_')
            right_index--;
        tape[cur_index--] = write_symbol;
        if(cur_index < left_index) 
            left_index = cur_index;
    }
    else if(direction == 'r')
    {
        if(cur_index + 1 >= size)
            extend_right();
        //在最左端放下了空白符
        if(cur_index == left_index && write_symbol == '_')
            left_index++;
        tape[cur_index++] = write_symbol;
        if(cur_index > right_index) 
            right_index = cur_index;
    }
}
void Tape::show_tape()
{
    for(int i = left_index; i <= right_index; i++)
        std::cout << tape[i];
    std::cout << std::endl;
}
/*
    Index0 : 0 1 2 3 4 5 6 7 8 9 10 11 12
    Tape0  : 1 1 1 0 0 0 0 0 1 1 1  0  0
    Head0  : ^
*/
void Tape::show_tape_v(int n)
{
    std::cout << "Index" << n << " : ";
    for(int i = left_index; i <= right_index; i++)
    {
        int index = i >= mid_index ? (i-mid_index):(mid_index-i);
        std::cout << index << " ";
    }
    std::cout << std::endl;
    std::cout << "Tape" << n << "  : ";
    for(int i = left_index; i <= right_index; i++)
    {
        int index = i >= mid_index ? (i-mid_index):(mid_index-i);
        std::cout << tape[i];
        int bits = 0;
        if(index == 0) bits = 1;
        else{
            while(index > 0){
                index /= 10;
                bits++;
            }
        }
        for(int j = 0; j < bits; j++)
            std::cout << " ";
    }
    std::cout << std::endl;
    std::cout << "Head" << n << "  : ";
    for(int i = left_index; i < cur_index; i++)
    {
        int index = i >= mid_index ? (i-mid_index):(mid_index-i);
        int bits = 0;
        if(index == 0) bits = 1;
        else{
            while(index > 0){
                index /= 10;
                bits++;
            }
        }
        for(int j = 0; j <= bits; j++)
            std::cout << " ";
    }
    std::cout << "^" << std::endl;
}
void Tape::extend_left()
{
    size = 2*size;
    char* temp = new char[size + 1];
    strcpy(temp + size/2, tape);
    memset(temp, 0x5F, size/2);
    left_index = left_index + size/2;
    right_index = right_index + size/2;
    mid_index = mid_index + size/2;
    cur_index = cur_index + size/2;
}
void Tape::extend_right()
{
    size = 2*size;
    char* temp = new char[size + 1];
    strcpy(temp, tape);
    memset(temp + size/2,0x5F,size/2);
    temp[size + 1] = '\0';
    delete tape;
    tape = temp;
}