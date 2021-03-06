// BlockchainInterview.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct block {
    uint64_t id;
    uint64_t prev_id;

    const bool operator==(uint64_t other) const {
        return prev_id == other;
    }
};

class blockchain {
private:
    struct block_impl {
        vector<shared_ptr<block_impl>> mNext;
        uint64_t id;

        block_impl( uint64_t Id) : id(Id) {}
    };

    shared_ptr<block_impl> mRoot;
public:
    blockchain(const vector<block>& blocks) {
        mRoot = make_shared<block_impl>(0);
        
        vector<block> tmp = blocks;
        vector<shared_ptr<block_impl>> currentBlocks = { mRoot };
        while (tmp.begin() != tmp.end()) {
            vector<block>::iterator it = tmp.begin();
            auto searchBlocks = currentBlocks;
            currentBlocks.clear();
            for (auto& block: searchBlocks) {
                while ((it = find(tmp.begin(), tmp.end(), block->id)) != tmp.end()) {
                    if (it != tmp.end()) {
                        block->mNext.emplace_back(
                            make_shared<block_impl>(
                                it->id
                                )
                        );
                        currentBlocks.push_back(block->mNext.back());
                        tmp.erase(it);
                        it = tmp.begin();
                    }
                }
            }
        }
    }

    int getDepth() const {
        int depthCounter = 0;
        vector<shared_ptr<block_impl>> next = {mRoot};

        while (next.size() != 0) {
            auto blocksToTraverse = next;
            next.clear();

            for (auto& b : blocksToTraverse) {
                if (b->mNext.size() != 0) {
                    next.insert(next.end(), b->mNext.begin(), b->mNext.end());
                }
            }
            if (next.size() > 0)
                depthCounter++;
        }

        return depthCounter;
    }
};

int getDepth(const vector<block>& blocks) {
    blockchain bc(blocks);
    return bc.getDepth();
}

int main(int argc, char* argv[])
{
    vector<block> blocks;
    for (int i = 1; i < argc ; i += 2) {
        block b;
        b.id = stoi(argv[i]);
        b.prev_id = stoi(argv[i + 1]);
        blocks.push_back(b);
    }

    cout << getDepth( blocks) << endl;
    string s;
    cin >> s;
    return 0;
}
