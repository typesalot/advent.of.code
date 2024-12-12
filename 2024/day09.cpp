#include "common.h"

class Day9 : public testing::Test {
  protected:
    string compressed;

    static constexpr uint32_t FREE = numeric_limits<uint32_t>::max();

    struct block {
        uint32_t id   = FREE;
        uint32_t size = 0;

        block* next = nullptr;
        block* prev = nullptr;
    };

    block*         head = nullptr;
    block*         tail = nullptr;
    vector<block*> free_list;

    void SetUp() override {
      auto fname = getInputFile(2024, 9);
      auto f     = ifstream(fname);
      getline(f, compressed);
    }

    void TearDown() override {
      block* cur_node = head;
      while (cur_node) {
        block* tmp = cur_node->next;
        delete cur_node;
        cur_node = tmp;
      }
    }

    void printDisk() {
      if (!g_config.debug)
        return;
      block* cur_block = head;
      while (cur_block) {
        for (int i = 0; i < cur_block->size; i++) {
          if (cur_block->id != FREE)
            cout << cur_block->id;
          else
            cout << ".";
        }
        cur_block = cur_block->next;
      }
      cout << endl;
    }

    void decompressDisk() {
      bool     is_file    = false;
      uint32_t file_id    = 0;
      block*   prev_block = nullptr;
      for (auto c : compressed) {
        is_file       = !is_file;
        uint32_t size = c - '0';
        if (size == 0)
          continue;

        block* new_block = new block();

        new_block->size = c - '0';

        if (is_file) {
          new_block->id = file_id;
          file_id++;
        } else {
          free_list.emplace_back(new_block);
        }

        if (prev_block) {
          prev_block->next = new_block;
          new_block->prev  = prev_block;
        } else
          head = new_block;

        prev_block = new_block;
      }

      // put a free sentinal node at the end to make
      // the decompress logic simpler
      tail             = new block();
      prev_block->next = tail;
      tail->prev       = prev_block;
    }

    void defrag() {
      block* cur_node   = tail->prev;
      block* free_block = nullptr;

      printDisk();

      while (free_list.size()) {
        if (free_block == nullptr || free_block->id != FREE)
          free_block = free_list.front();

        assert(free_block->id == FREE);

        if (free_block->size < cur_node->size) {
          // use up the entire free block
          free_block->id = cur_node->id;
          cur_node->size -= free_block->size;
          tail->size += free_block->size;
          free_list.erase(free_list.begin());
        } else if (free_block->size == cur_node->size) {
          free_block->id = cur_node->id;
          cur_node->id   = FREE;
          cur_node->size -= free_block->size;
          tail->size += free_block->size;
          free_list.erase(free_list.begin());
        } else {
          // split the free block
          block* new_block      = new block();
          new_block->size       = free_block->size - cur_node->size;
          new_block->next       = free_block->next;
          new_block->prev       = free_block;
          new_block->next->prev = new_block;

          // update free_block ( becomes cur_node )
          free_block->id   = cur_node->id;
          free_block->size = cur_node->size;
          free_block->next = new_block;

          // update the free_list
          free_list.erase(free_list.begin());
          free_list.insert(free_list.begin(), new_block);

          // update the tail sentinel
          tail->size += cur_node->size;

          // update cur_node
          cur_node->id   = FREE;
          cur_node->size = 0;
        }

        if (cur_node->id == FREE) {
          do {
            cur_node = cur_node->prev;

            if (cur_node->id == FREE) {
              auto itr =
                  find_if(free_list.cbegin(), free_list.cend(), [&cur_node](const block* b) { return b == cur_node; });
              assert(itr != free_list.cend());
              free_list.erase(itr);
            }
          } while (cur_node && cur_node->id == FREE);
        }

        printDisk();
      }
    }

    uint64_t getChecksum() {
      decompressDisk();
      defrag();

      uint64_t checksum = 0;

      block* cur_block = head;
      int    i         = 0;

      while (cur_block) {
        if (cur_block->id != FREE)
          for (int j = 0; j < cur_block->size; j++)
            checksum += (cur_block->id * i++);
        cur_block = cur_block->next;
      }

      return checksum;
    }
};

TEST_F(Day9, Part1Examples) {
  compressed        = "2333133121414131402";
  uint64_t checksum = getChecksum();
  EXPECT_EQ(checksum, 1928);
}

TEST_F(Day9, Part1) {
  uint64_t checksum = getChecksum();
  EXPECT_EQ(checksum, 6242766523059);
  cout << "Answer = " << checksum << endl;
}