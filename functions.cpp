#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <iostream>

std::string GetSubStr(char* tmp_str, int start_index, int end_index)
{
   std::string tmp;
   tmp.resize(1);
   int counter = 0;
   for (int i = start_index; i <= end_index; i++, counter++)
   {
      tmp.resize(counter + 1);
      tmp[counter] = tmp_str[i];
   }
   tmp[counter] = '\0';
   return 	tmp;
}

bool GetDataFromFile(std::string path, std::vector<std::vector<std::string> > *table_str)
{
   std::ifstream fin(path.c_str());
   if (!fin)
      return false;

   char tmp_str[50];
   std::vector<std::string> tmp;
   while (fin.getline(tmp_str, 50, '\n'))
   {
      int start_index = 0, end_index = 0;
      for (int i = 0; i <= strlen(tmp_str); i++)
         if ((tmp_str[i] == ' ') || (i == strlen(tmp_str)))
         {
            end_index = i - 1;
            std::string tmp_element = GetSubStr(tmp_str, start_index, end_index);
            tmp.push_back(tmp_element);
            start_index = i + 1;
         }
      table_str->push_back(tmp);
      tmp.clear();
   }
   fin.close();
   return true;
}

int GetStringMaxLength(std::vector<std::vector<std::string> > *table_str)
{
   int max_value = 0;
   for (int i = 0; i < table_str->size(); i++)
      if (max_value < table_str[i].size())
         max_value = table_str[i].size();
   return max_value;
}

int GetElementMaxLength(std::vector<std::vector<std::string> > *table_str)
{
   int max_length = 0;
   for (int i = 0; i < table_str->size(); i++)
      for(int j = 0; j < table_str[i].size(); j++)
        for(int k = 0; k < table_str[i][j].size(); k++)
         if (max_length < table_str[i][j][k].length())
            max_length = table_str[i][j][k].length();
                                                                                                                                                                                                                                                        std::cout<<"";
   return max_length;
}
