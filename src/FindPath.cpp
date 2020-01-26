//
// Created by lix on 2020/1/22.
//

#include "FindPath.h"


void DFS_FindPath::Step() {
    static std::vector<size_t> TempPath;
    static size_t PathLength;
    auto pre_index = history_.top();
    auto nex_index = std::numeric_limits<size_t>::max();
    for(size_t i=0; i< num_point_; i++){
        if(map_[pre_index][i] < N_ACC && !is_visited_[pre_index][i]){
            nex_index = i;
            history_.push(nex_index);
            is_visited_[pre_index][nex_index] = true;
            is_visited_[nex_index][nex_index] = true;
            PathLength += map_[pre_index][i];
            temp_path_.push_back(nex_index);
            break;
        }
    }

    if(nex_index == end_){
        temp_path_.push_back(PathLength);
        path_.push_back(temp_path_);
        temp_path_.erase(temp_path_.end() - 2, temp_path_.end());
        history_.pop();
    }else if(nex_index == std::numeric_limits<size_t>::max()){
        history_.pop();

    }
}
//////////////////////////////////////////////////
void DijkstraFindPath::Step() {
     size_t min_dist = std::numeric_limits<size_t>::max();
     size_t min_id = std::numeric_limits<size_t>::max();

     for (size_t i = 0; i < num_point_; i++) {
         if (!is_visited_[i] && all_path_[i].path_len < min_dist) {
             min_id = i;
             min_dist = all_path_[i].path_len;
         }
     }

     // choose the min path len let the id as visited
     is_visited_(min_id);

     // update the unvisited path, compare new path with origin path
     UpdatePath(min_id);
}

bool DijkstraFindPath::IsFinish() {
    return num_point_ == is_visited_.visited_count;
}

void DijkstraFindPath::UpdatePath(size_t min_id) {
    for(size_t i = 0; i< num_point_; i++){
        if(!is_visited_[i] && map_[min_id][i] < N_ACC){
            S_Path new_path;
            new_path = all_path_[min_id];
            new_path.Add(i,map_[min_id][i]);
            if(new_path < all_path_[i])
                all_path_[i] = new_path;
        }
    }
}
