#include "turing.h"
int main (int argc,char *argv[])
{
    int i = 1;
    bool verbose_mode = false;
    bool legal_parameters = true;
    if(argc == 2 && (strcmp(argv[i],"-h") == 0||strcmp(argv[i],"--help") == 0))
    {
        std::cout << "usage:" << std::endl
                << "turing -h|--help" << std::endl
                << "turing [-v|--verbose] <tm> <input>" << std::endl;
        return 0;  
    }
    if(strcmp(argv[i],"-v") == 0||strcmp(argv[i],"--verbose") == 0)
        verbose_mode = true, i++;
    if(i + 1 < argc)
    {
        char* file = argv[i];
        char* input = argv[i+1];
        FILE* fp = fopen(file, "r");
        if(fp != NULL){
            Turing_Machine Tm(fp, verbose_mode);
            if(Tm.Parsed_successfully())
            { 
                bool run_successfully = Tm.Run(input);
                if(!run_successfully)
                    return -3;
            }
            else{
                std::cerr << "syntax error" << std::endl;
                return -2;
            }
        }
        else legal_parameters = false;
    }
    else legal_parameters = false;
    if(!legal_parameters)
    {
        std::cerr << "Illegal parameters" << std::endl;
        std::cout << "usage:" << std::endl
                << "turing -h|--help" << std::endl
                << "turing [-v|--verbose] <tm> <input>" << std::endl;
        return -1;
    }
    return 0;
}