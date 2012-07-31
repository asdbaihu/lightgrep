#include <algorithm>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
#include <vector>
#include <utility>

#include "basic.h"
#include "pattern.h"
#include "stest.h"

void longTest() {
  uint32 count = 0;

  uint32 len, patcount;
  while (std::cin.peek() != -1) {
    // read number of patterns
    std::cin.read(reinterpret_cast<char*>(&patcount), sizeof(patcount));
    std::vector<Pattern> patterns;
    patterns.reserve(patcount);

    for (uint32 i = 0; i < patcount; ++i) {
      // read pattern
      std::cin.read(reinterpret_cast<char*>(&len), sizeof(len));
      std::string pattern(len, '\0');
      std::cin.read(&pattern[0], len);

      // read fixed
      bool fixed;
      std::cin.read(reinterpret_cast<char*>(&fixed), 1);

      // read case-insensitive
      bool case_insensitive;
      std::cin.read(reinterpret_cast<char*>(&case_insensitive), 1);

      // read encoding
      std::cin.read(reinterpret_cast<char*>(&len), sizeof(len));
      std::string encoding(len, '\0');
      std::cin.read(&encoding[0], len);

      patterns.emplace_back(pattern, fixed, case_insensitive, 0, encoding);

      std::cout << pattern << ' ' << encoding << ' ';
    }

    // read text
    std::cin.read(reinterpret_cast<char*>(&len), sizeof(len));
    std::string text(len, '\0');
    std::cin.read(&text[0], len);

    // NB: Yes, we are reading data directly into the storage for a
    // std::vector. We are trained professionals. Don't try this at
    // home, boys and girls.

    // read hits
    std::cin.read(reinterpret_cast<char*>(&len), sizeof(len));
    std::vector<SearchHit> expected(len);
    std::cin.read(reinterpret_cast<char*>(&expected[0]), len*sizeof(SearchHit));

    // run lg on the text with the patterns
    STest test(patterns);
    const byte* text_ptr = reinterpret_cast<const byte*>(text.data());
    test.search(text_ptr, text_ptr + text.length(), 0);

    std::vector<SearchHit>& actual = test.Hits;

    std::sort(expected.begin(), expected.end());
    std::sort(actual.begin(), actual.end());

/*
    std::copy(expected.begin(), expected.end(),
              std::ostream_iterator<SearchHit>(std::cout, " "));

    std::cout << '\n';

    std::copy(actual.begin(), actual.end(),
              std::ostream_iterator<SearchHit>(std::cout, " "));
*/

    if (actual == expected) {
      std::cout << "ok.\n";
    }
    else {
      // Houston, we have a problem
      std::cout << "failed!\n";

      std::cout << "missing: ";

      std::set_difference(
        expected.begin(), expected.end(),
        actual.begin(), actual.end(),
        std::ostream_iterator<SearchHit>(std::cout, " ")
      );

      std::cout << '\n';
      std::cout << "extra: ";

      std::set_difference(
        actual.begin(), actual.end(),
        expected.begin(), expected.end(),
        std::ostream_iterator<SearchHit>(std::cout, " ")
      );

      std::cout << '\n';
    }

    if (++count % 1000 == 0) {
      std::cerr << count << std::endl;
    }
  }

  std::cerr << count << std::endl;
}
