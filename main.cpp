#include <cstdlib>
#include <iostream>
#include <time.h>
#include <fstream>
#include<map>
#include <clocale>
#include <sstream>
#include <vector>
#include<algorithm>
#include <list>
#include"Tree.h"
#include "mpi.h"
//#define FillRandom
using namespace std;


int main(int argc, char **argv) {
    //setlocale(NULL, "Russian");
    ///////////////////////////////////////////////////////работа с файлами

    ofstream  file_encoding_Huffman;


    //setlocale(LC_ALL, "ru_RU");
    std::locale::global(std::locale("en_US.UTF-8"));


    file_encoding_Huffman.open("encodingHuffman.txt", ios::out);

    if (!file_encoding_Huffman.is_open())
    {
        cout<<"file error";
        return 0;
    }
    ifstream file_in_first("Original.txt");
    if (!file_in_first) {
        cerr << "File error." << endl;
        return 1;
    }

#ifdef FillRandom
    wfstream file_original;
    file_original.open("Original.txt", ios::out );//app - добавляем в конец файла
    if (!file_original.is_open())
    {
        cout<<"file error";
        return 0;
    }
    FillFile(file_original);                  //составление первого файла
    file_original.close();
#endif
    //////////////////////////////////////////////////////составляем таблицу для вероятностей
    string str_from_file;
    map<char,double>probability= FillPropability(file_in_first, str_from_file);
    Node*root= CreatTree(probability);
    map<string,vector<bool>> table_map= CreateTable(root);


    ////////////////////////////////Кодируем файл
    for(int i=0;i<str_from_file.size();i++){
        for_each(table_map[string(1,str_from_file[i])].begin(),
                 table_map[string(1,str_from_file[i])].end(),
                 [&file_encoding_Huffman](bool a){file_encoding_Huffman << a;});
                 }
    file_encoding_Huffman.close();
    ///////////////////////////////Декодируем файл
    ifstream file_encodingHuffman_in("encodingHuffman.txt");
    if (!file_encodingHuffman_in) {
        cerr << "File error." << endl;
        return 1;
    }
    string str_encoding;
    if( !getline(file_encodingHuffman_in, str_encoding)) {
        cout << "error reading" << endl;
        return 0;
    }
    ofstream  file_decodingHuffman;
    file_decodingHuffman.open("decodingHuffman.txt", ios::out );
    file_decodingHuffman << DecodingHuffman(str_encoding, root);
    file_decodingHuffman.close();
    file_encodingHuffman_in.close();

    wcout<<L"Цена кодирования:  "<<CodePrice(table_map,probability)<<endl;
////////////////////////////////////////////////////////////Задание 2 RLE
    ofstream file_encoding_RLE;
    file_encoding_RLE.open("encodingRLE.txt",ios::out);
    string strRLE= RLEGetStr(str_from_file);
    file_encoding_RLE<<strRLE;
    file_encoding_RLE.close();
    ifstream file_encoding_RLE_from("encodingRLE.txt");
    if (!file_encoding_RLE_from) {
        cerr << "File error." << endl;
        return 1;
    }
    ofstream file_decoding_RLE;
    file_decoding_RLE.open("decodingRLE.txt",ios::out);

        stringstream all1;
        all1 << file_encoding_RLE_from.rdbuf();
        string all_str1;
        all_str1 = all1.str();

    file_decoding_RLE<<DecodingRLE(all_str1);
    file_decoding_RLE.close();
    file_encoding_RLE_from.close();
////////////////////////////////////////////////////////////Задание 3 RLE+Хемминг
/////////////////////////////////////////////////////// Кодируем RLE+Хемминг:
    ifstream file_encoding_RLE1("encodingRLE.txt");
    ofstream file_encoding_Huffman_RLE;
    file_encoding_Huffman_RLE.open("encodingHuffman_RLE.txt", ios::out);
    string str_from_file1;
    map<char,double>probability1= FillPropability(file_encoding_RLE1, str_from_file1);
    Node*root1= CreatTree(probability1);
    map<string,vector<bool>> table_map1= CreateTable1(root1);

    stringstream str_stream1;
    for(int i=0;i<str_from_file1.size();i++){
        for_each(table_map1[string(1,str_from_file1[i])].begin(),
                 table_map1[string(1,str_from_file1[i])].end(),
                 [&str_stream1](bool a){str_stream1 << a;});
    }

    string strRLE1=str_stream1.str();
    file_encoding_Huffman_RLE << strRLE1;
    file_encoding_Huffman_RLE.close();
//////////////////////////////////////////////////////////Декодируем RLE+Хемминг+RLE:
    ifstream max_encoding_from("encodingHuffman_RLE.txt");
    ofstream max_decoding_in("decodingHuffman_RLE.txt",ios::out);
    stringstream all2;
    all2 << max_encoding_from.rdbuf();
    string all_str2;
    all_str2 = all2.str();
    string help_str;
    help_str.reserve(100000);
    help_str=DecodingRLE(all_str2);
    string help_str1;
    help_str1.reserve(100000);
    help_str1= DecodingHuffman(help_str, root1);
    string help_str2;
    help_str2.reserve(100000);
    help_str2=DecodingRLE(help_str1);
    max_decoding_in<<help_str2;
    max_decoding_in.close();
    max_encoding_from.close();

    return 0;
}
//{у, ф, х, ц, ч, ш, пробел, 0-9, &, .} Алгоритм B