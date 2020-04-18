//
// Created by fenix on 4/17/20.
//

#include "../../includes/counting/word_count.h"
#include <boost/locale.hpp>


// 5 statements
template<class InputIt, class ForwardIt, class BinOp>
static void for_each_token(InputIt first, InputIt last,
                           ForwardIt s_first, ForwardIt s_last,
                           BinOp binary_op) {
    while (first != last) {
        const auto pos = find_first_of(first, last, s_first, s_last);
        binary_op(first, pos);
        if (pos == last) break;
        first = next(pos);
    }
}


// 2 statements
void fast_count_words(const std::string &str, std::map<std::string, size_t> *result_map) {
    using namespace std;
    constexpr char delims[] = " \n\t\r\f";
    for_each_token(cbegin(str), cend(str),
                   cbegin(delims), cend(delims),
                   [result_map](auto first, auto second) {
                       if (first != second) {
                           std::string element = string(first, second);
                           element = boost::locale::to_lower(
                                   boost::locale::fold_case(boost::locale::normalize(element)));
                           element.erase(std::remove_if(element.begin(), element.end(),
                                                        [](const unsigned &c) { return !isspace(c) && !isalpha(c); }),
                                         element.end());
                           (*result_map)[element] += 1;
                       }
                   });
}