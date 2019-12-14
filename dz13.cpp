/*
 Написать алгоритм для решения игры в “пятнашки”. Решением задачи является приведение к виду: [ 1 2 3 4 ] [ 5 6 7 8 ] [ 9 10 11 12] [ 13 14 15 0 ], где 0 задает пустую ячейку. Достаточно найти хотя бы какое-то решение. Число перемещений костяшек не обязано быть минимальным.
 */

#include <vector>
#include <iostream>
#include <unordered_map>
#include <map>
#include <unordered_set>


const size_t HashParameter = 37;

struct Position {
  std::vector<char> chips;
  char zero_place;
    
  bool IsFinish() const;
  std::vector<Position> Siblings() const;

  bool operator==(const Position& other) const;
  bool operator!=(const Position& other) const { return !operator==(other); }
};

namespace std {
  template <>
  struct hash<Position> {
    size_t operator()(const Position& key) const {
      size_t value = 0;
      for (auto chip : key.chips) {
        value = HashParameter * value + static_cast<size_t>(chip);
      }
      return value;
    }
  };
}

const Position FinishPosition{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0}, 15};

bool Position::IsFinish() const {
  return operator==(FinishPosition);
}

std::vector<Position> Position::Siblings() const {
  std::vector<Position> result;
  if (zero_place < 12) {
    Position sibling = *this;
    std::swap(sibling.chips[zero_place], sibling.chips[zero_place + 4]);
    sibling.zero_place += 4;
    result.emplace_back(sibling);
  }
  if (zero_place >= 4) {
    Position sibling = *this;
    std::swap(sibling.chips[zero_place], sibling.chips[zero_place - 4]);
    sibling.zero_place -= 4;
    result.emplace_back(sibling);
  }
  if (zero_place % 4 != 0) {
    Position sibling = *this;
    std::swap(sibling.chips[zero_place], sibling.chips[zero_place - 1]);
    sibling.zero_place -= 1;
    result.emplace_back(sibling);
  }
  if (zero_place % 4 != 3) {
    Position sibling = *this;
    std::swap(sibling.chips[zero_place], sibling.chips[zero_place + 1]);
    sibling.zero_place += 1;
    result.emplace_back(sibling);
  }
  return result;
}

bool Position::operator==(const Position& other) const {
  for (size_t i = 0; i < chips.size(); ++i ) {
    if (chips[i] != other.chips[i]) return false;
  }
  return true;
}

char GetMoveSymbol(const Position& from, const Position& to) {
  char zero_diff = to.zero_place - from.zero_place;
  switch (zero_diff) {
    case 1:
      return 'L'; // Ноль вправо -> фишка влево
    case -1:
      return 'R';
    case 4:
      return 'U';
    case -4:
      return 'D';
  }
  return 0;
}

bool has_solution(const Position& position) {
    const std::vector<char>& chips = position.chips;
    int inv = 0;
    for (int i = 0; i < 16; ++i)
        if (chips[i])
            for (int j = 0; j<i; ++j)
                if (chips[j] > chips[i])
                    ++inv;

    inv += 1 + position.zero_place / 4;
     
    return !(inv & 1);
}

size_t manhattan(const Position& position) {
    size_t dist = 0;
    for (int i=0; i < 16; ++i) {
        if (position.chips[i] == 0) continue;
        
        int x = i % 4;
        int y = i / 4;
        int i_eta = position.chips[i] - 1;
        int x_eta = i_eta % 4;
        int y_eta = i_eta / 4;
        dist += abs(x - x_eta) + abs(y - y_eta);
    }
    return dist;
}


struct PositionScore {
    size_t distance;
    size_t h;
    size_t hash;
};

bool operator < (const PositionScore& p1, const PositionScore& p2) {
    if (p1.distance + p1.h == p2.distance + p2.h)
        return p1.hash < p2.hash;
    return p1.distance + p1.h < p2.distance + p2.h;
}

std::unordered_map<Position, Position> a_star(const Position& start) {
    std::map<PositionScore, Position> positions_queue;
    std::unordered_map<Position, Position> parents;
    std::unordered_map<Position, PositionScore> scores;
    std::unordered_set<Position> visited;
    
    PositionScore start_score {0, manhattan(start), std::hash<Position>()(start)};
    positions_queue.emplace(start_score, start);
    scores.emplace(start, start_score);
    
    while (!positions_queue.empty()) {
        auto current_it = positions_queue.begin();
        Position current = current_it->second;
        if (current.IsFinish()) {
            break;
        }
        positions_queue.erase(current_it);
        visited.emplace(current);
        
        const PositionScore& cur_score = scores[current];
        for (auto sibling : current.Siblings()) {
            if (visited.find(sibling) != visited.end()) {
                continue;
            }

            auto score_it = scores.find(sibling);
            if (score_it != scores.end()) {
                PositionScore score = score_it->second;
                size_t new_distance = std::min(score.distance, cur_score.distance + 1);
                if (new_distance < score.distance) {
                    auto queue_it = positions_queue.find(score);
                    if (queue_it != positions_queue.end())
                        positions_queue.erase(queue_it);
                    score.distance = new_distance;
                    scores[sibling] = score;
                    positions_queue.emplace(score, sibling);
                    parents[sibling] = current;
                }
            } else {
                PositionScore score {cur_score.distance + 1, manhattan(sibling), std::hash<Position>()(sibling)};
                scores.emplace(sibling, score);
                positions_queue.emplace(score, sibling);
                parents.emplace(sibling, current);
            }
            if (positions_queue.size() > 1000) {
                positions_queue.erase(--positions_queue.end());
            }
        }
    }
    return parents;
}

template<typename T>
void reverse_vec(T first, T last) {
    while ((first != last) && (first != --last)) {
        std::iter_swap(first++, last);
    }
}

std::vector<char> GetPath(const Position& start,
                          const std::unordered_map<Position, Position>& parents) {
  std::vector<char> result;

  Position current = FinishPosition;
  while (current != start) {
    const Position& parent = parents.at(current);
    result.push_back(GetMoveSymbol(parent, current));
    current = parent;
  }
  reverse_vec(result.begin(), result.end());
  return result;
}


std::pair<bool, std::vector<char>> solve(const Position& start) {
  if (has_solution(start)) {
    return std::make_pair(true, GetPath(start, a_star(start)));
  }
  return std::make_pair(false, std::vector<char>());
}


int main(int argc, const char * argv[]) {
    char zero_pos = 0;
    std::vector<char> start(16);
    for (size_t i = 0; i < 16; ++i) {
        int point;
        std::cin >> point;
        start[i] = point;
        if (point == 0) {
            zero_pos = i;
        }
    }
     
    Position start_position{start, zero_pos};
    
    auto result = solve(start_position);
    if (result.first) {
        std::cout << result.second.size() << '\n';
        for (char move : result.second) {
          std::cout << move;
        }
    } else {
        std::cout << -1;
    }
    
    return 0;
}
