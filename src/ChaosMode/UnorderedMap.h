#ifndef _PPL_UNORDERED_MAP_H
#define _PPL_UNORDERED_MAP_H

#include <unordered_map>

#ifndef _CHAOS_MODE

template<typename key_type, typename value_type, typename... types>
using UnorderedMap = std::unordered_map<key_type, value_type, types ...>;

#else 

#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <utility>

#include <iostream>

template<typename key_type, typename value_type, typename... types>
class UnorderedMap : public std::unordered_map<key_type, value_type, types ...>
{
  public:
    typedef typename std::unordered_map<key_type, value_type, types ...>  base;
    typedef typename base::iterator                                       iterator;
    typedef typename base::const_iterator                                 const_iterator;
    typedef typename std::pair<key_type, value_type>                      map_pair;

    // inherit all constructors
    //  TODO: we want to shuffle if adding elements during construction, so we should override each constructor...ouch
    using base::base;

    // update all insert methods

    // insert value into map, shuffle map, and return pair in accordance with C++11 standard
    std::pair<iterator, bool> insert(const value_type& value) {
      std::pair<iterator, bool> retVal = base::insert(value);
      this->shuffleSelf();
      return std::make_pair(this->findByValue(value), retVal.second);
    }

    // insert value into map, shuffle map, and return pair in accordance with C++11 standard
    template<class P = std::pair<key_type, value_type>> // default allows for brace enclosed initializer lists
    std::pair<iterator, bool> insert(P&& value) {
      std::pair<iterator, bool> retVal = base::insert(value);
      this->shuffleSelf();
      return std::make_pair(this->findByValue(value), retVal.second);
    }

    // insert value into map, shuffle map, and return iterator in accordance with C++11 standard
    iterator insert(const_iterator hint, const value_type& value) {
      base::insert(hint, value);
      this->shuffleSelf();
      return this->findByValue(value);
    }

    // insert value into map, shuffle map, and return iterator in accordance with C++11 standard
    template<class P>
    iterator insert(const_iterator hint, P&& value) {
      base::insert(hint, value);
      this->shuffleSelf();
      return this->findByValue(value);
    }

    // insert value into map, shuffle map
    template<class InputIt>
    void insert(InputIt first, InputIt last) {
      base::insert(first, last);
      this->shuffleSelf();
    }

    // insert value into map, shuffle map
    void insert(std::initializer_list<value_type> ilist) {
      for(auto& it : ilist) {
        base::insert(it);
      }
      this->shuffleSelf();
    }

    // emplace element then shuffle
    // TODO: also return pair properly, currently returns original broken iterator
    template<class... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
      std::pair<iterator, bool> retVal = base::emplace(args...);
      this->shuffleSelf();
      return retVal;  // TODO: return item after finding it...not sure how to do that yet
    }

    // emplace element with hint then shuffle
    // TODO: also return pair properly, currently returns original broken iterator
    template<class... Args>
    std::pair<iterator, bool> emplace_hint(Args&&... args) {
      std::pair<iterator, bool> retVal = base::emplace_hint(args...);
      this->shuffleSelf();
      return retVal;  // TODO: return item after finding it...not sure how to do that yet
    }

    // update [] operator

  protected:

    iterator findByValue(value_type& value){
      for(auto it = base::begin(); it != base::end(); ++it) {
        if(it->second == value) 
          return it;
      } 
      return base::end();
    }

    template<class P>
    iterator findByValue(P&& value){
      for(auto it = base::begin(); it != base::end(); ++it) {
        if(it->second == value.second) 
          return it;
      } 
      return base::end();
    }

    void shuffleSelf() noexcept {
      // make vectors to hold keys and values
      std::vector<key_type> m_keys;
      std::vector<value_type> m_values;
      std::vector<key_type> m_keysShuffled;
      std::vector<value_type> m_valuesShuffled;

      // copy keys, values from self to vectors
      for(auto it = base::begin(); it != base::end(); ++it) {
        m_keys.push_back(std::move(it->first));
        m_values.push_back(std::move(it->second));
      }
      // clear self now that it is empty
      base::clear();

      // shuffle both vectors in the same way by moving into shuffled vector
      std::vector<int> indices;
      indices.reserve(m_keys.size());
      for(size_t i = 0; i < m_keys.size(); ++i)
          indices.push_back(i);
      unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
      std::default_random_engine e(seed);
      std::shuffle(indices.begin(), indices.end(), e);
      for(auto& it : indices){
        m_keysShuffled.push_back(std::move(m_keys.at(it)));
        m_valuesShuffled.push_back(std::move(m_values.at(it)));
      }

      // populate self with shuffled ordering
      for(size_t i = 0; i < m_keysShuffled.size(); i++) {
        base::insert(std::move(std::make_pair(m_keysShuffled.at(i), m_valuesShuffled.at(i))));
      }
    }
};
#endif  // _CHAOS_MODE

#endif  // _PPL_UNORDERED_MAP_H