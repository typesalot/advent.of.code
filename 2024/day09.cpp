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

        bool is_file() const {
          return id != FREE;
        }

        bool is_free() const {
          return id == FREE;
        }
    };

    block*         head = nullptr;
    block*         tail = nullptr;
    vector<block*> free_list;

    void remove(block* b) {
      if (b->prev)
        b->prev->next = b->next;
      if (b->next)
        b->next->prev = b->prev;
      if (head == b)
        head = b->next;
      if (tail == b)
        tail = b->prev;
      delete b;
    }

    // b1 += b2; delete b2;
    void merge(block* b1, block* b2) {
      if (b1 == nullptr || b2 == nullptr)
        return;
      assert(b1->id == b2->id);
      if (b1->id == b2->id)
        b1->size += b2->size;
    }

    void contiguous_freespace() {
      block* c = head;
      while (c && c->next) {
        if (c->id == FREE)
          assert(c->id != c->next->id);
        c = c->next;
      }
    }

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

    void inflateDisk() {
      bool     is_file    = false;
      uint32_t file_id    = 0;
      block*   prev_block = nullptr;
      for (auto c : compressed) {
        is_file       = !is_file;
        uint32_t size = c - '0';

        block* new_block = new block();

        new_block->size = size;

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

      // tail free sentinel node
      tail             = new block();
      tail->prev       = prev_block;
      prev_block->next = tail;
    }

    void compress() {
      block* cur_node   = tail->prev;
      block* free_block = head->next;

      printDisk();

      while (free_block != cur_node) {
        contiguous_freespace();

        assert(free_block->id == FREE);

        if (free_block->size < cur_node->size) {
          free_block->id = cur_node->id;
          cur_node->size -= free_block->size;

          assert(cur_node->next->is_free());
          cur_node->next->size += free_block->size;
        } else if (free_block->size == cur_node->size) {
          free_block->id = cur_node->id;

          cur_node->id = FREE;
          block* tmp   = cur_node->prev;
          merge(cur_node, cur_node->next);
          merge(cur_node->prev, cur_node);
          remove(cur_node->next);
          remove(cur_node);
          cur_node = tmp;
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
          free_block       = new_block;

          // update cur_node
          cur_node->id = FREE;
          block* tmp   = cur_node->prev;
          merge(cur_node, cur_node->next);
          merge(cur_node->prev, cur_node);
          remove(cur_node->next);
          remove(cur_node);
          cur_node = tmp;
        }

        // get next file block
        while (cur_node->is_free()) {
          if (free_block == cur_node)
            break;
          cur_node = cur_node->prev;
        }

        // get next free block
        while (free_block->is_file()) {
          if (free_block == cur_node)
            break;
          free_block = free_block->next;
        }
        printDisk();
      }

      contiguous_freespace();
    }

    uint64_t getChecksum() {
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

TEST_F(Day9, Part1Example) {
  compressed = "2333133121414131402";

  inflateDisk();
  compress();

  uint64_t checksum = getChecksum();
  EXPECT_EQ(checksum, 1928);
}

TEST_F(Day9, Part1) {
  inflateDisk();
  compress();

  uint64_t checksum = getChecksum();
  EXPECT_EQ(checksum, 6242766523059);
  cout << "Answer = " << checksum << endl;
}