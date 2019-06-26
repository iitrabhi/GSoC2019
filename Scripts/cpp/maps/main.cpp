#include <iostream>
#include<map>
#include<vector>
int main()
{   auto run_map = false;
    auto run_understand_send = true;

    if(run_map) {
        // Some map.
        std::map<std::vector<int>, int> info;

        std::vector<std::vector<int>> points{{1, 3, 1},
                                             {2, 3, 1}};
        std::cout << points.size() << std::endl;
        for (int i = 0; i < points.size(); ++i) {
            std::vector<int> ve = points[i];
            info.insert(std::make_pair(ve, i));
        }

        auto itr = info.find({2, 3, 1}); // Find element
        if (itr != std::end(info)) {       // Only if found
            int v = itr->second;         // Iterator returns std::pair (key, value)
            //for (auto i : v) {             // Print result or do what you want.
            std::cout << v << std::endl;
            //}
        }
    }

    if(run_understand_send){
        std::vector<std::vector<std::int32_t>> send_entities(2);
        std::vector<std::int32_t> v{31,22};
        send_entities[0].push_back(1);
        send_entities[0].insert(send_entities[0].end(), v.begin(), v.end());
        std::cout<<"Edge: ";
    }

return 0;
}