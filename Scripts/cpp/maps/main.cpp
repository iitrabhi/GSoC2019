#include <iostream>
#include<map>
#include<vector>
int main()
{
       // Some map.
    std::map<std::vector<int>, int> info;

    std::vector<std::vector<int>> points{ { 1, 3, 1 } , { 2, 3, 1 }};
    std::cout<<points.size()<< std::endl;
    for  (int i=0;i<points.size();++i){
        std::vector<int> ve = points[i];
        info.insert(std::make_pair(ve, i));
    }

    auto itr = info.find({ 2, 3, 1 }); // Find element
    if (itr != std::end(info)) {       // Only if found
        int v = itr->second;         // Iterator returns std::pair (key, value)
        //for (auto i : v) {             // Print result or do what you want.
        std::cout << v << std::endl;
        //}
    }
return 0;
}