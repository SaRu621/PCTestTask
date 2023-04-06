//Standart libraries
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <map>
//#pragma once

std::vector<std::string> Split(std::string line, char delimeter);
std::string CSV_Into_String(std::string path);

template<class T> std::ostream& operator<<(std::ostream& out, const std::vector<T>& a){
    out << "{  ";

    for(int i = 0; i < a.size(); i++)
        out << a[i] << "  ";
    
    out << '}';

    return out;
}

class CSV_Table{
    public:
        std::map<std::string, std::vector<std::string>> Data;
        std::vector<std::string> Headers = {};
        std::string FilePath;
        int Columns = 1;
        int Rows = 0;

        CSV_Table(){};

        CSV_Table(std::string path, char delimeter = ','){
            FilePath = path;

            std::ifstream file(path);

            std::string line;

            getline(file, line);
    
            for(int i = 0; i < line.size(); i++)
                if(line[i] == ',')
                    Columns++;
        
            std::string Temp = "";          //temporary string

            for(int i = 0; i < line.size() + 1; i++)
                if(line[i] != delimeter && i != line.size())
                    Temp = Temp + line[i];
                else{
                    Data.insert({Temp, {}});
                    Headers.push_back(Temp);
                    Temp = "";
                }

            std::vector<std::string> Buffer;             //Need for temporary container for each rows
            Buffer.resize(Headers.size());

            while(getline(file, line)){
                std::stringstream stream(line);
            
                for(int i = 0; i < Headers.size(); i++)
                    getline(stream, Buffer[i], delimeter); 

                for(int i = 0; i < Headers.size(); i++)
                    Data[Headers[i]].push_back(Buffer[i]);

                Rows++;
            }
        }

        void String_Into_CSV(std::string csv_string){
            std::string Temp;
            std::vector<std::string> Buffer;

            int i;
            for(i = 0; i < csv_string.size(); i++){
                if(csv_string[i] == '\n'){
                    Headers = Split(Temp, ',');
                    break;
                }
                Temp = Temp + csv_string[i];
            }
            Columns = Headers.size();
           
            while(i != csv_string.size() - 1){
                i++;
                Temp = "";
                for(; i < csv_string.size(); i++){
                    if(csv_string[i] == '\n'){
                        Buffer = Split(Temp, ',');
                        Rows++;
                        break;
                    }   
                    Temp = Temp + csv_string[i];                    
                }

                for(int j = 0; j < Headers.size(); j++)
                    Data[Headers[j]].push_back(Buffer[j]);
            }
        }

        std::string CSV_Into_String(){
            std::string table = "";
            for(int i = 0; i < Columns - 1; i++)
                table = table + Headers[i] + ',';
            table = table + Headers[Columns - 1] + '\n';

            for(int i = 0; i < Rows; i++){
                for(int j = 0; j < Columns - 1; j++)
                    table = table + Data[Headers[j]][i] + ',';
                table = table + Data[Headers[Columns - 1]][i] + '\n';    
            }

            return table;
        }

        void Head(){
            for(int i = 0; i < Headers.size(); i++)
                std::cout << Headers[i] << ' '; 
            std::cout << std::endl;
        }

        std::string Get_Path(){ return FilePath; };

        void Info(){ this -> Head(); std::cout << "Rows: " << Rows << "\nColumns:" << Columns << std::endl; };
        
        void Show(std::string Parameter){
            bool flag = false;
            for(int i = 0; i < Headers.size(); i++)
                if(Headers[i] == Parameter)
                    flag = true;
             
            if(!flag){
                std::cout << "Not in table!\n";
                return;
            }

            std::cout << Parameter << "\n\n";
    
            for(int i = 0; i < Rows; i++)
                std::cout << Data[Parameter][i] << std::endl;

            std::cout << std::endl;
        }

        void Sort(std::string Parameter, int Begin, int End){       //Sort in slice  
            bool flag = false;                  
            for(int i = 0; i < Headers.size(); i++)
                if(Headers[i] == Parameter)
                    flag = true;
           
            if(!flag){
                std::cout << "Not in table!\n";
                return;
            }
            
            bool flag1 = false;

            for(int i = 0; i < Data[Parameter][0].size(); i++)      //Check on string or double type
                if((Data[Parameter][0][i] != '0') and \
                   (Data[Parameter][0][i] != '1') and \
                   (Data[Parameter][0][i] != '2') and \
                   (Data[Parameter][0][i] != '3') and \
                   (Data[Parameter][0][i] != '4') and \
                   (Data[Parameter][0][i] != '5') and \
                   (Data[Parameter][0][i] != '6') and \
                   (Data[Parameter][0][i] != '7') and \
                   (Data[Parameter][0][i] != '8') and \
                   (Data[Parameter][0][i] != '9') and \
                   (Data[Parameter][0][i] != '.'))
                    flag1 = true;

            if(flag1){                                              //Sort for string
                for(int i = Begin; i < End; i++)
                    for(int j = i; j < End; j++)
                        if(Data[Parameter][i] > Data[Parameter][j]){
                            std::swap(Data[Parameter][i], Data[Parameter][j]);
                            for(int k = 0; k < Headers.size(); k++)
                                if(Headers[k] != Parameter)
                                    std::swap(Data[Headers[k]][i], Data[Headers[k]][j]);
                        }
            }
            else{                                                   //Sort for double
                for(int i = Begin; i < End; i++){
                    if(Data[Parameter][i] == "N/A" or Data[Parameter][i] == "")
                            continue;
                    for(int j = i; j < End; j++)
                        if(Data[Parameter][j] == "N/A" or Data[Parameter][j] == "")
                            continue;
                        else if(std::stod(Data[Parameter][i]) > std::stod(Data[Parameter][j])){
                            std::swap(Data[Parameter][i], Data[Parameter][j]);
                            for(int k = 0; k < Headers.size(); k++)
                                if(Headers[k] != Parameter)
                                    std::swap(Data[Headers[k]][i], Data[Headers[k]][j]);
                        }
                }
            }
        }

        void Sort(std::string Parameters){ Sort(Parameters, 0, Rows); };    //Sort in all table

        void Sort(std::vector<std::string> Parameters){                     //Sort by few parameters
            std::vector<int> Displs;                            //vector of displacement 
            
            Sort(Parameters[0]);                                //Sort by general parameter

            for(int j = 1; j < Parameters.size(); j++){         //Sort by other parameters
                std::string Temp = Data[Parameters[j - 1]][0];
                Displs.push_back(0);

                for(int i = 1; i < Rows; i++){
                    if(Temp != Data[Parameters[j - 1]][i]){
                        Displs.push_back(i);
                        Temp = Data[Parameters[j - 1]][i];
                    }
                }

                Displs.push_back(Rows);

                for(int i = 0; i < Displs.size() - 1; i++)
                    Sort(Parameters[j], Displs[i], Displs[i + 1]);

                Displs.clear();
            }
        }

        ~CSV_Table(){ Headers.clear(); Data.clear(); };

        friend std::ostream& operator<<(std::ostream& out, CSV_Table a);
};

std::ostream& operator<<(std::ostream& out, CSV_Table a);/*{
    for(int i = 0; i < a.Columns; i++)
        out << a.Headers[i] << " ";
    
    out << "\n";

    for(int i = 0; i < a.Rows; i++){
        for(int j = 0; j < a.Columns; j++)
            out << a.Data[a.Headers[j]][i] << "\t"; 
        
        out << "\n";    
    }   

    return out;
}*/

class Files{
    public:
        std::map<std::string, CSV_Table> Data;
        int Num_Of_Files = 0;

        Files(){};

        void Add(CSV_Table a){ Data.insert({a.FilePath, a}); Num_Of_Files++; };

        void Delete(std::string Path){ Data.erase(Path); Num_Of_Files--; };

        std::string Show(std::string Path){ return Data[Path].CSV_Into_String(); };

        void Head(std::string Path){ Data[Path].Head(); };

        void Info(std::string Path){ Data[Path].Info(); };

        void Sort(std::string Path, std::vector<std::string> Parameters){ Data[Path].Sort(Parameters); };

        void Sort(std::string Path, std::string Parameter){ Data[Path].Sort(Parameter); };

        std::string Get_List(){
            std::string list = "";

            auto it = Data.begin();
            for(; it != Data.end(); it++){
                list = list + (it -> first) + '\n';
            }

            return list;
        }

        std::string Get_Header(std::string Path){
            std::string headers = "";

            for(int i = 0; i < Data[Path].Headers.size() - 1; i++)
                headers = headers + Data[Path].Headers[i] + ' ';
            headers = headers + Data[Path].Headers[Data[Path].Headers.size() - 1];

            return headers;
        }

        ~Files(){ Data.clear(); };

        friend std::ostream& operator<<(std::ostream& out, Files a);
};

std::ostream& operator<<(std::ostream& out, Files a);/*{
    out << "Number of files: " << a.Num_Of_Files << std::endl; 
    
    for(auto it = a.Data.begin(); it != a.Data.end(); it++)
        out << it -> first << std::endl;

    return out;
}*/
