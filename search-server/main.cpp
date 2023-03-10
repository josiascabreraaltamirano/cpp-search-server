// Решите загадку: Сколько чисел от 1 до 1000 содержат как минимум одну цифру 3?
// Напишите ответ здесь:
#include <iostream>
#include <set>
#include <string>

using namespace std;

bool Finder(int current_number, int number_to_filter){
  string cn = to_string(current_number);
  string ntf = to_string(number_to_filter);
  for(char n : cn){
    if(n == ntf[0]){
      return true;
    }
  }
  return false;
}

set<int> CounterNumbers(int min_num_range, int max_num_range, int number_to_filter){
  set<int> result;
  for(int i = min_num_range; i <= max_num_range; i++){
    if(Finder(i, number_to_filter)){
      result.insert(i);
    }
  }
  return result;
}
int main() {
  cout << CounterNumbers(1, 1000, 3).size() << endl; 
  return 0;
}


// Закомитьте изменения и отправьте их в свой репозиторий.
