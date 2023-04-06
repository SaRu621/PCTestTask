#include "/home/rus/Desktop/TestTask/Load.h"

std::vector<std::string> Split(std::string line, char delimeter){
    std::vector<std::string> Result = {};
    std::string Temp = "";

    for(int i = 0; i < line.size() + 1; i++)
        if(line[i] != delimeter && i != line.size())
            Temp = Temp + line[i];
        else{
            Result.push_back(Temp);
            Temp = "";
        }
    
    return Result;
}

std::string CSV_Into_String(std::string path){
    std::string table;
    std::string temp;
    std::ifstream file(path);

    while(getline(file, temp))
        table = table + temp + "\n";

    return table;
}

std::ostream& operator<<(std::ostream& out, CSV_Table a){
    for(int i = 0; i < a.Columns; i++)
        out << a.Headers[i] << " ";

    out << "\n";

    for(int i = 0; i < a.Rows; i++){
        for(int j = 0; j < a.Columns; j++)
            out << a.Data[a.Headers[j]][i] << "\t";

        out << "\n";
    }

    return out;
}

std::ostream& operator<<(std::ostream& out, Files a){
    out << "Number of files: " << a.Num_Of_Files << std::endl;

    for(auto it = a.Data.begin(); it != a.Data.end(); it++)
        out << it -> first << std::endl;

    return out;
}
