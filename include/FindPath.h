//
// Created by lix on 2020/1/22.
//

#pragma once

#include <utility>
#include <limits>
#include "Maze.h"

struct S_Path{
    std::vector<size_t> path_vec;
    size_t path_len=std::numeric_limits<size_t >::max();
    bool operator < (const S_Path &rhs){
        return this->path_len < rhs.path_len;
    }
    void Add(size_t id, size_t weight = 1){
        path_vec.push_back(id);
        path_len += weight;
    }
};

class AbstractFindPath{
public:
    explicit AbstractFindPath(S_Maze::AdjacencyMapType map):map_(std::move(map)){

    };

    virtual void Step() = 0;

    virtual bool IsFinish() = 0;

protected:
    S_Maze::AdjacencyMapType map_;
};

class DFS_FindPath : public AbstractFindPath{
public:
    explicit DFS_FindPath(S_Maze::AdjacencyMapType map, size_t start_index = 0, size_t end_index = 0) :
            AbstractFindPath(std::move(map)), start_(start_index), end_(map.size()), num_point_(map_.size()) {
            Reset(map, start_,end_);
    }

    void Reset(const S_Maze::AdjacencyMapType& map, size_t start_index, size_t end_index){
        num_point_ = map_.size();
        map_ = map;
        end_ = end_index;
        start_ = start_index;

        is_visited_ = std::vector<std::vector<bool>>(num_point_, std::vector<bool>(num_point_,false));

        history_.push(start_);
        is_visited_[start_][start_] = true;
        temp_path_.push_back(start_);
    }

    void Step() override ;


private:
    std::vector<std::vector<bool>> is_visited_; // is_visited_[idx1][idx2] the path between the two index
    std::stack<size_t> history_;

    std::vector<std::vector<size_t>> path_; // the first index store every path, the path vec last index store path length
    std::vector<size_t> temp_path_;
    size_t end_, start_;
    size_t num_point_;
};

class DijkstraFindPath: public AbstractFindPath{
public:
    explicit DijkstraFindPath(const S_Maze::AdjacencyMapType& map, size_t start_index = 0, size_t end_index = 0) :
            AbstractFindPath(map), start_(start_index), end_(map.size() - 1), num_point_(map_.size()) {
        Reset(map, start_,end_);};

    void Reset(const S_Maze::AdjacencyMapType& map, size_t start_index, size_t end_index){
        map_ = map;
        num_point_ = map_.size();
        start_ = start_index;
        end_ = end_index;

        is_visited_.Reset(num_point_);
        is_visited_(start_);

        S_Path path;
        path.path_vec.reserve(num_point_);
        path.path_vec.push_back(start_);
        all_path_.resize(num_point_,path);
        all_path_[start_].path_len = 0;

        UpdatePath(start_);
    }

    void Step() override;

    bool IsFinish() override;

    std::vector<size_t> MinPath(){
        return all_path_[end_].path_vec;
    }

private:
    struct S_IsVisited{
        S_IsVisited() = default;
        void Reset(size_t num_point){
            check_table.resize(num_point, false);
            visited_count = 0;
        }

        inline bool IsAllVisited(){
            return visited_count==check_table.size();
        };

        inline bool operator [](size_t id){
            return check_table[id];
        }

        inline void operator () (size_t id){
            if(check_table[id])
                throw std::domain_error("This Id had been visited");
            check_table[id] = true;
            visited_count++;
        }
        size_t visited_count = 0;
        std::vector<bool> check_table;
    };


    std::vector<S_Path> all_path_;
    size_t end_, start_;
    size_t num_point_;
    S_IsVisited is_visited_;

    void UpdatePath(size_t min_id);

};