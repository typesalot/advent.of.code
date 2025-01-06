#include "test.h"

using namespace std;

class Day22 : public aoc_2024 {
  protected:
    vector<uint64_t> seeds;
    const uint32_t   iterations = 2000;

    void LoadInput(istringstream& input) override {
      string s;
      while (getline(input, s))
        seeds.push_back(atoi(s.c_str()));
    }

    uint64_t mix(uint64_t a, uint64_t b) const {
      return a ^ b;
    }

    uint64_t prune(uint64_t a) const {
      return a % 16777216ull;
    }

    uint64_t next(uint64_t a) const {
      // step 1
      uint64_t n = a;
      uint64_t b = n * 64;
      n          = mix(n, b);
      n          = prune(n);

      // step 2
      b = n / 32;
      n = mix(n, b);
      n = prune(n);

      // step 3
      b = n * 2048;
      n = mix(n, b);
      n = prune(n);

      return n;
    }

    uint32_t price(uint64_t a) const {
      return a % 10;
    }

    vector<uint64_t> getSecrets(uint64_t seed, uint32_t count) {
      vector<uint64_t> secrets{seed};

      uint64_t n = seed;

      while (count) {
        n = getSecret(n, 1);
        secrets.emplace_back(n);
        count--;
      }

      return secrets;
    }

    vector<uint64_t> getPrices(const vector<uint64_t>& secrets) {
      vector<uint64_t> result = secrets;
      for (auto& r : result)
        r = price(r);
      return result;
    }

    uint64_t getSecret(uint64_t seed, uint32_t count) {
      uint64_t n = seed;

      while (count) {
        n = next(n);
        count--;
      }

      return n;
    }

    uint64_t getSecretsSum() {
      uint64_t result = 0;

      for (auto s : seeds)
        result += getSecret(s, iterations);

      return result;
    }

    uint64_t getMaxBananas() {
      uint64_t result = 0;

      for (auto s : seeds) {
        auto            secrets = getSecrets(s, iterations);
        auto            prices  = getPrices(secrets);
        vector<int32_t> changes(prices.size());

        int32_t cur_max = 0;
        for (int i = 0; i < prices.size(); i++) {
          changes[i] = prices[i + 1] - prices[i];

          if (i > 3)
            cur_max = max(cur_max, changes[i]);
        }
      }

      return result;
    }
};

TEST_F(Day22, SecretsTest) {
  EXPECT_EQ(mix(42, 15), 37);
  EXPECT_EQ(prune(100000000), 16113920);

  auto             answer = getSecrets(123, 10);
  vector<uint64_t> expect =
      {123, 15887950, 16495136, 527345, 704524, 1553684, 12683156, 11100544, 12249484, 7753432, 5908254};

  EXPECT_EQ(answer.size(), expect.size());

  if (answer.size() == expect.size()) {
    for (int i = 0; i < answer.size(); i++)
      EXPECT_EQ(answer[i], expect[i]);

    vector<uint64_t> expect = {3, 0, 6, 5, 4, 4, 6, 4, 4, 2, 4};
    answer                  = getPrices(answer);
    if (answer.size() == expect.size()) {
      for (int i = 0; i < answer.size(); i++)
        EXPECT_EQ(answer[i], expect[i]);
    }
  }
}

TEST_F(Day22, Part1Example) {
  input = "1\n"
          "10\n"
          "100\n"
          "2024";
  SetUp();

  auto answer = getSecretsSum();
  EXPECT_EQ(answer, 37327623);
}

TEST_F(Day22, Part1) {
  auto answer = getSecretsSum();
  EXPECT_EQ(answer, 19150344884);
}

TEST_F(Day22, Part2Example) {
  input = "1\n"
          "2\n"
          "3\n"
          "2024";
  SetUp();

  auto answer = getMaxBananas();
  EXPECT_EQ(answer, 23);
}