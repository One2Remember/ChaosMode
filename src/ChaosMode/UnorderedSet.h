#ifndef _PPL_UNORDERED_SET_H
#define _PPL_UNORDERED_SET_H

#include <unordered_set>

#ifndef _CHAOS_MODE

template<typename key_type, typename... types>
using UnorderedSet = std::unordered_set<key_type, types ...>;

#else

#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <utility>

template<typename key_type, typename... types>
class UnorderedSet : public std::unordered_set<key_type, types ...>
{
  public:
    typedef typename std::unordered_set<key_type, types ...>  base;
    typedef typename base::iterator                           iterator;
    typedef typename base::const_iterator                     const_iterator;

    // inherit all constructors
    //  TODO: we want to shuffle if adding elements during construction, so we should override each constructor...ouch
    using base::base;

    std::pair<iterator, bool> insert(const key_type& value) {
      std::pair<iterator, bool> retVal = base::insert(value);
      this->shuffleSelf();
      return std::make_pair(this->find(value), retVal.second);
    }

    std::pair<iterator, bool> insert(key_type&& value) {
      std::pair<iterator, bool> retVal = base::insert(value);
      this->shuffleSelf();
      return std::make_pair(this->find(value), retVal.second);
    }

    iterator insert(const_iterator hint, const key_type& value) {
      base::insert(hint, value);
      this->shuffleSelf();
      return this->find(value);
    }

    iterator insert(const_iterator hint, key_type&& value) {
      base::insert(hint, value);
      this->shuffleSelf();
      return this->find(value);
    }

    template<class InputIt>
    void insert(InputIt first, InputIt last) {
      base::insert(first, last);
      this->shuffleSelf();
    }

    void insert(std::initializer_list<key_type> ilist) {
      for(auto& it : ilist) {
        base::insert(it);
      }
      this->shuffleSelf();
    }


    // emplace element then shuffle
    // TODO: also return pair properly, currently returns original broken iterator
    template< class... Args >
    std::pair<iterator,bool> emplace( Args&&... args ) {
      std::pair<iterator, bool> retVal = base::emplace(args...);
      this->shuffleSelf();
      return retVal;  // TODO: return item after finding it...not sure how to do that yet
    }


    // emplace element with hint then shuffle
    // TODO: also return pair properly, currently returns original broken iterator
    template <class... Args>
    iterator emplace_hint( const_iterator hint, Args&&... args ) {
      std::pair<iterator, bool> retVal = base::emplace_hint(args...);
      this->shuffleSelf();
      return retVal;  // TODO: return item after finding it...not sure how to do that yet
    }

    // update [] operator

  protected:
    void shuffleSelf() noexcept {
      // make vectors to hold keys and values
      std::vector<key_type> m_keys;
      std::vector<key_type> m_keysShuffled;

      // copy keys, values from self to vectors
      for(auto it = base::begin(); it != base::end(); ++it) {
        m_keys.push_back(std::move(*it));
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
      }

      // populate self with shuffled ordering
      for(size_t i = 0; i < m_keysShuffled.size(); i++) {
        base::insert(std::move(m_keysShuffled.at(i)));
      }
    }
};
#endif  // _CHAOS_MODE

#endif  // _PPL_UNORDERED_SET_H